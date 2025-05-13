#include "../include/kernel.h"
#include "kernel_planificadores.h"

// Función auxiliar para convertir estado a string
char* estado_a_string(t_estado_proceso estado) {
    char* estados[] = {"NEW", "READY", "EXEC", "BLOCKED", "EXIT", "SUSP_READY", "SUSP_BLOCKED"};
    if (estado >= 0 && estado <= 6) {
        return estados[estado];
    }
    return "DESCONOCIDO";
}

// Función mejorada para actualizar PCB
void actualizar_pcb_estado(t_pcb* pcb, t_estado_proceso nuevo_estado) {
    t_estado_proceso estado_anterior = pcb->estado_actual;
    pcb->estado_actual = nuevo_estado;
    
    // Registrar métrica de estado
    pcb->ME[nuevo_estado]++;
    pcb->MT[nuevo_estado] = time(NULL);
    
    log_info(kernel_logger, "## (%d) Pasa del estado %s al estado %s", 
             pcb->PID, estado_a_string(estado_anterior), estado_a_string(nuevo_estado));
}

// Mejorar la función para crear PCB
t_pcb* crear_pcb(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = malloc(sizeof(t_pcb));
    if (!nuevo_pcb) {
        log_error(kernel_logger, "No se pudo asignar memoria para el PCB del proceso %d", pid);
        return NULL;
    }

    nuevo_pcb->PID = pid;
    nuevo_pcb->PC = 0;
    nuevo_pcb->estimacion_actual = 0;
    nuevo_pcb->prioridad = 0;  // Prioridad por defecto
    nuevo_pcb->archivo = archivo ? strdup(archivo) : NULL;
    nuevo_pcb->tamanio = tamanio;

    // Inicializar métricas de estado y tiempo
    for (int i = 0; i < NUM_ESTADOS; i++) {
        nuevo_pcb->ME[i] = 0;
        nuevo_pcb->MT[i] = 0;
    }

    // Registrar ingreso al estado NEW
    nuevo_pcb->estado_actual = NEW;
    nuevo_pcb->ME[NEW] = 1;
    nuevo_pcb->MT[NEW] = time(NULL); 

    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);

    return nuevo_pcb;
}

void init_primer_proceso(char* nombre_archivo, char* size) {
    // Espera ENTER del usuario
    printf("Presione ENTER para crear y planificar el primer proceso...\n");
    getchar();
    int tamanio = atoi(size);
    t_pcb* PCB = crear_pcb(PID, nombre_archivo, tamanio);
    agregar_a_new(PCB);

    // Pedir confirmación a Memoria de que se puede pasar a READY el primer proceso
    int response = init_proceso_memoria(nombre_archivo, size);

    if (response < 0) {
        log_error(kernel_logger, "No se puede inicializar primer proceso");
        abort();
    }
}

// Algoritmos de planificación de corto plazo
t_pcb* obtener_siguiente_ready() {
    if (queue_is_empty(cola_ready)) return NULL;

    if (string_equals_ignore_case(ALGORITMO_CORTO_PLAZO, "FIFO")) {
        return siguiente_fifo();
    } else if (string_equals_ignore_case(ALGORITMO_CORTO_PLAZO, "SJF")) {
        return siguiente_sjf();
    } else if (string_equals_ignore_case(ALGORITMO_CORTO_PLAZO, "PRIORIDADES")) {
        return siguiente_prioridad();
    } else {
        log_warning(kernel_logger, "Algoritmo '%s' no válido. Se usa FIFO por defecto.", ALGORITMO_CORTO_PLAZO);
        return siguiente_fifo();
    }
}

t_pcb* siguiente_fifo() {
    return queue_pop(cola_ready);
}

t_pcb* siguiente_sjf() {
    if (queue_is_empty(cola_ready)) return NULL;
    
    t_list* procesos = cola_ready->elements;
    t_pcb* mejor_pcb = NULL;
    double menor_estimacion = -1;
    int indice_mejor = -1;
    
    void _encontrar_menor_tiempo(t_pcb* pcb, int indice) {
        if (mejor_pcb == NULL || pcb->estimacion_actual < menor_estimacion) {
            mejor_pcb = pcb;
            menor_estimacion = pcb->estimacion_actual;
            indice_mejor = indice;
        }
    }
    
    list_iterate_with_index(procesos, (void*)_encontrar_menor_tiempo);
    
    if (indice_mejor >= 0) {
        return list_remove(cola_ready->elements, indice_mejor);
    }
    
    return NULL;
}

t_pcb* siguiente_prioridad() {
    if (queue_is_empty(cola_ready)) return NULL;
    
    t_list* procesos = cola_ready->elements;
    t_pcb* mejor_pcb = NULL;
    int mayor_prioridad = -1;
    int indice_mejor = -1;
    
    void _encontrar_mayor_prioridad(t_pcb* pcb, int indice) {
        if (mejor_pcb == NULL || pcb->prioridad > mayor_prioridad) {
            mejor_pcb = pcb;
            mayor_prioridad = pcb->prioridad;
            indice_mejor = indice;
        }
    }
    
    list_iterate_with_index(procesos, (void*)_encontrar_mayor_prioridad);
    
    if (indice_mejor >= 0) {
        return list_remove(cola_ready->elements, indice_mejor);
    }
    
    return NULL;
}

void actualizar_estimacion(t_pcb* pcb, int tiempo_real) {
    double anterior = pcb->estimacion_actual;
    pcb->estimacion_actual = ALFA * tiempo_real + (1 - ALFA) * anterior;
    log_info(kernel_logger, "## (%d) Estimación actualizada: %.2f", pcb->PID, pcb->estimacion_actual);
}

// Funciones para mover procesos entre estados
void traer_de_new_a_ready() {
    t_pcb* pcb = queue_peek(cola_new);
    
    // Enviar INIT a Memoria
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pcb->PID);
    
    // Si el PCB tiene info del archivo y tamaño
    if (pcb->archivo != NULL) {
        cargar_string_al_buffer(buffer, pcb->archivo);
        cargar_int_al_buffer(buffer, pcb->tamanio);
    } else {
        cargar_string_al_buffer(buffer, "dummy");
        cargar_int_al_buffer(buffer, 0);
    }

    t_paquete* paquete = crear_paquete(CPU_K_INIT_PROC, buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);

    int cod_op = recibir_operacion(socket_memoria);

    if (cod_op == OK) {
        // Sacar de NEW y poner en READY
        queue_pop(cola_new); // Sacamos ahora que sabemos que memoria lo aceptó
        actualizar_pcb_estado(pcb, READY);
        queue_push(cola_ready, pcb);
    } else {
        log_warning(kernel_logger, "Memoria rechazó la inicialización del PID: %d", pcb->PID);
    }
}

void traer_de_susp_ready_a_ready() {
    if (queue_is_empty(cola_susp_ready)) return;
    
    t_pcb* pcb = queue_pop(cola_susp_ready);
    actualizar_pcb_estado(pcb, READY);
    queue_push(cola_ready, pcb);
}

void despachar_a_cpu(t_pcb* pcb) {
    if (dictionary_size(cpus_libres) > 0) {
        
        t_list* claves = dictionary_keys(cpus_libres);
        char* cpu_id = list_get(claves, 0); // Obtener la primera CPU libre
        list_destroy(claves); 

        int* socket_ptr = dictionary_remove(cpus_libres, cpu_id);
        int socket_cpu = *socket_ptr;

        actualizar_pcb_estado(pcb, EXEC);
        pcb->PC++; // opcional: avanzar PC si corresponde

        t_buffer* buffer = crear_buffer();
        cargar_int_al_buffer(buffer, pcb->PID);
        cargar_int_al_buffer(buffer, pcb->PC);

        t_paquete* paquete = crear_paquete(K_CPU_EXEC_PROCESO, buffer);
        enviar_paquete(paquete, socket_cpu);
        eliminar_paquete(paquete);

        dictionary_put(cpus_ocupadas, cpu_id, pcb);
        free(socket_ptr);
    } else {
        queue_push(cola_ready, pcb);
        log_info(kernel_logger, "## (%d) No hay CPU libre, proceso reencolado en READY", pcb->PID);
    }
}

// Planificadores principales
void planificador_largo_plazo() {
    while (1) {
        if (!queue_is_empty(cola_new) && !queue_is_empty(cola_susp_ready)) {
            // Implementar lógica para decidir si traer un proceso de NEW o de SUSP_READY
            if (string_equals_ignore_case(ALGORITMO_INGRESO_A_READY, "FIFO")) {
                // FIFO entre NEW y SUSP_READY
                long tiempo_new = -1;
                long tiempo_susp = -1;
                
                if (!queue_is_empty(cola_new)) {
                    t_pcb* pcb_new = queue_peek(cola_new);
                    tiempo_new = pcb_new->MT[NEW];
                }
                
                if (!queue_is_empty(cola_susp_ready)) {
                    t_pcb* pcb_susp = queue_peek(cola_susp_ready);
                    tiempo_susp = pcb_susp->MT[SUSP_READY];
                }
                
                if (tiempo_new <= tiempo_susp && tiempo_new != -1) {
                    // Traer de NEW
                    traer_de_new_a_ready();
                } else if (tiempo_susp != -1) {
                    // Traer de SUSP_READY
                    traer_de_susp_ready_a_ready();
                }
            } else if (string_equals_ignore_case(ALGORITMO_INGRESO_A_READY, "PRIORIDAD_SUSP_READY")) {
                // Prioridad a SUSP_READY
                if (!queue_is_empty(cola_susp_ready)) {
                    traer_de_susp_ready_a_ready();
                } else {
                    traer_de_new_a_ready();
                }
            } else {
                // Por defecto, prioridad a NEW
                if (!queue_is_empty(cola_new)) {
                    traer_de_new_a_ready();
                } else {
                    traer_de_susp_ready_a_ready();
                }
            }
        } else if (!queue_is_empty(cola_new)) {
            traer_de_new_a_ready();
        } else if (!queue_is_empty(cola_susp_ready)) {
            traer_de_susp_ready_a_ready();
        }

        usleep(100000); // Evitar busy waiting agresivo
    }
}

void planificador_corto_plazo() {
    while (1) {
        if (!queue_is_empty(cola_ready) && dictionary_size(cpus_libres) > 0) {
            t_pcb* pcb = obtener_siguiente_ready();
            if (pcb != NULL) {
                despachar_a_cpu(pcb);
            }
        }

        usleep(100000); // 100 ms
    }
}

// Funciones de manejo de syscalls y procesos
void manejar_syscall_init_proc(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = crear_pcb(pid, archivo, tamanio);
    agregar_a_new(nuevo_pcb);
}

void finalizar_proceso(t_pcb* pcb) {
    // Actualizar estado
    actualizar_pcb_estado(pcb, EXIT);
    
    // Informar a memoria
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pcb->PID);

    t_paquete* paquete = crear_paquete(K_M_FINALIZAR_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);

    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op == M_K_RESPUESTA_OK) {
        log_info(kernel_logger, "## (%d) - Finaliza el proceso correctamente", pcb->PID);

        // Liberar recursos
        if (pcb->archivo != NULL) {
            free(pcb->archivo);
        }
        free(pcb);
    } else {
        log_error(kernel_logger, "## (%d) Fallo al finalizar proceso", pcb->PID);
        // Reintentar o manejar el error
    }
}

/* void intentar_inicializar_proceso() {
    if (!queue_is_empty(cola_new)) {
        t_pcb* pcb = queue_peek(cola_new);

        t_buffer* buffer = crear_buffer();
        cargar_int_al_buffer(buffer, pcb->PID);
        cargar_string_al_buffer(buffer, pcb->archivo);
        cargar_int_al_buffer(buffer, pcb->tamanio);

        t_paquete* paquete = crear_paquete(INICIAR_PROCESO, buffer);
        enviar_paquete(paquete, socket_memoria);
        eliminar_paquete(paquete);

        int cod_op = recibir_operacion(socket_memoria);
        if (cod_op == OK) {
            t_buffer* respuesta_buffer = recibir_buffer(socket_memoria);
            void* tabla_paginas = extraer_void_del_buffer(respuesta_buffer); // Debés definir bien esto

            pcb = queue_pop(cola_new); // Ahora sí lo quitamos
            pcb->tabla_paginas = tabla_paginas;
            pcb->estado_actual = READY;
            queue_push(cola_ready, pcb);

            log_info(kernel_logger, "## (%d) Pasa de NEW a READY", pcb->PID);

            eliminar_buffer(respuesta_buffer);
        } else {
            log_warning(kernel_logger, "## (%d) Memoria rechazó la carga del proceso", pcb->PID);
        }
    }
}
 */
