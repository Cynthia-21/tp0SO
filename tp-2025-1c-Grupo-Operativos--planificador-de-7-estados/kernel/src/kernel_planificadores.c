#include "../include/kernel.h"
#include "kernel_planificadores.h"

void  init_primer_proceso(char* nombre_archivo, char* size){

    // Espera ENTER del usuario
    printf("Presione ENTER para crear y planificar el primer proceso...\n");
    getchar();
    int tamanio = atoi(size);
    t_pcb* PCB = crear_pcb(PID, nombre_archivo,tamanio);
    agregar_a_new(PCB);

    // Pedir confirmación a Memoria de que se puede pasar a READY el primer proceso
    int response = init_proceso_memoria(nombre_archivo, size);

    if (response < 0){
        log_error(kernel_logger, "No se puede inicializar primer proceso");
        abort();
    }

    //cambiar_estado_proceso(PID, NEW, READY)
}

t_pcb* crear_pcb(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = malloc(sizeof(t_pcb));
    if (!nuevo_pcb) {
        log_error(kernel_logger, "No se pudo asignar memoria para el PCB del proceso %d", pid);
        return NULL;
    }

    nuevo_pcb->PID = pid;
    nuevo_pcb->PC = 0;

    // Inicializar métricas de estado y tiempo
    for (int i = 0; i < NUM_ESTADOS; i++) {
        nuevo_pcb->ME[i] = 0;
        nuevo_pcb->MT[i] = 0;
    }

    // Registrar ingreso al estado NEW
    nuevo_pcb->ME[NEW] = 1;
    nuevo_pcb->MT[NEW] = time(NULL); 

    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);

    return nuevo_pcb;
}

void agregar_a_new(t_pcb* pcb) {
    queue_push(cola_new, pcb);
    pcb->ME[NEW]++;
    pcb->MT[NEW] = time(NULL);
    log_info(kernel_logger, "## (%d) Proceso agregado a cola NEW", pcb->PID);
}

void planificador_largo_plazo() {
    while (1) {
        if (!queue_is_empty(cola_new)) {
            t_pcb* pcb = queue_peek(cola_new); 

            // Enviar INIT a Memoria (usa utils comunes)
            t_buffer* buffer = crear_buffer();
            cargar_int_al_buffer(buffer, pcb->PID);

            // Para mantener compatibilidad, puedes decidir enviar valores dummy o ajustar Memoria
            cargar_string_al_buffer(buffer, "dummy"); // si Memoria requiere un archivo
            cargar_int_al_buffer(buffer, 0); // tamaño dummy

            t_paquete* paquete = crear_paquete(CPU_K_INIT_PROC, buffer);
            enviar_paquete(paquete, socket_memoria);
            eliminar_paquete(paquete);

            int cod_op = recibir_operacion(socket_memoria);

            if (cod_op == OK) {
                queue_pop(cola_new); // sacar de NEW
                pcb->ME[READY]++;
                pcb->MT[READY] = time(NULL);
                queue_push(cola_ready, pcb);

                log_info(kernel_logger, "## (%d) Pasa de NEW a READY", pcb->PID);
            } else {
                log_warning(kernel_logger, "Memoria rechazó la inicialización del PID: %d", pcb->PID);
                // Podrías aplicar retry o suspender el proceso, según lógica del TP
            }
        }

        usleep(100000); // Evita busy waiting agresivo
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

t_pcb* obtener_siguiente_ready() {
    if (queue_is_empty(cola_ready)) return NULL;

    if (strcmp(ALGORITMO_CORTO_PLAZO, "FIFO") == 0) {
        return siguiente_fifo();
    } else if (strcmp(ALGORITMO_CORTO_PLAZO, "SJF") == 0) {
        return siguiente_sjf();
    } else {
        log_warning(kernel_logger, "Algoritmo no válido. Se usa FIFO por defecto.");
        return siguiente_fifo();
    }
}

t_pcb* siguiente_fifo() {
    return queue_pop(cola_ready);
}

t_pcb* siguiente_sjf() {
    int size = queue_size(cola_ready);
    if (size == 0) return NULL;

    int index_min = 0;
    t_pcb* pcb_min = list_get(cola_ready->elements, 0);

    for (int i = 1; i < size; i++) {
        t_pcb* pcb_actual = list_get(cola_ready->elements, i);
        if (pcb_actual->estimacion_actual < pcb_min->estimacion_actual) {
            index_min = i;
            pcb_min = pcb_actual;
        }
    }

    list_remove(cola_ready->elements, index_min);
    return pcb_min;
}

void actualizar_estimacion(t_pcb* pcb, int tiempo_real) {
    double anterior = pcb->estimacion_actual;
    pcb->estimacion_actual = ALFA * tiempo_real + (1 - ALFA) * anterior;
    log_info(kernel_logger, "## (%d) Estimación actualizada: %.2f", pcb->PID, pcb->estimacion_actual);
}

void despachar_a_cpu(t_pcb* pcb) {
    if (dictionary_size(cpus_libres) > 0) {
        
        t_list* claves = dictionary_keys(cpus_libres);
        char* cpu_id = list_get(claves, 0); // Obtener la primera CPU libre
        list_destroy(claves); 

        int* socket_ptr = dictionary_remove(cpus_libres, cpu_id);
        int socket_cpu = *socket_ptr;

        pcb->estado_actual = EXEC;
        pcb->PC++; // opcional: avanzar PC si corresponde

        log_info(kernel_logger, "## (%d) Pasa de READY a EXEC - Asignado a %s", pcb->PID, cpu_id);

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

void manejar_syscall_init_proc(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = crear_pcb(pid, archivo, tamanio);
    agregar_a_new(nuevo_pcb);
}


void finalizar_proceso(t_pcb* pcb) {
    // Informar a memoria
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pcb->PID);

    t_paquete* paquete = crear_paquete(K_M_FINALIZAR_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);
    eliminar_paquete(paquete);

    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op == M_K_RESPUESTA_OK) {
        pcb->estado_actual = EXIT_P;
        log_info(kernel_logger, "## (%d) - Finaliza el proceso", pcb->PID);

        // Liberar recursos
        free(pcb->archivo);
        free(pcb);

        // Intentar inicializar otro proceso si hay en NEW
        //init_proceso_memoria();
    } else {
        log_error(kernel_logger, "## (%d) Fallo al finalizar proceso", pcb->PID);
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
