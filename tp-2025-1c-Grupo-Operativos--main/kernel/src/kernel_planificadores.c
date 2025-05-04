#include "../include/kernel.h"
#include <commons/string.h>

//Funciones comentadas, hay que modificarlas para poder implementarlas en nuestro tp.

extern int socket_memoria;
extern t_log* kernel_logger;
extern t_list* cola_new;
extern t_list* cola_ready;
extern t_dictionary* dispositivos_io_bloqueados;
extern t_list* cpus_libres;
extern t_list* cpus_ocupadas;
extern int proximo_pid;

t_pcb* crear_pcb(int pid, char* archivo, int tamanio) {
    /* t_pcb* nuevo_pcb = malloc(sizeof(t_pcb));
    nuevo_pcb->pid = pid;
    nuevo_pcb->pc = 0;
    nuevo_pcb->estado_actual = NEW;
    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);
    return nuevo_pcb; */
}

void agregar_a_new(t_pcb* pcb) {
    /* list_add(cola_new, pcb); */
}

void planificador_largo_plazo() {
    /* while (control_key) {
        if (!list_is_empty(cola_new)) {
            t_pcb* pcb = list_first(cola_new);
            // Simulación de pedir a memoria (hay q modificar probablemente)
            enviar_operacion(INICIAR_PROCESO, socket_memoria);
            enviar_entero(pcb->pid, socket_memoria);
            char* archivo = "dummy.pseudocode"; // debería venir del INIT_PROC
            enviar_string(archivo, socket_memoria);
            enviar_entero(1024, socket_memoria); // Tamaño dummy

            int respuesta = recibir_operacion(socket_memoria);
            if (respuesta == OK) {
                list_remove(cola_new, 0);
                pcb->estado_actual = READY;
                list_add(cola_ready, pcb);
                log_info(kernel_logger, "## (%d) Pasa del estado NEW al estado READY", pcb->pid);
            } else {
                log_warning(kernel_logger, "Memoria rechazo la inicialización del PID: %d", pcb->pid);
                // En FIFO, se espera
            }
        }
        usleep(100000); // 100ms de espera
    } */
}

void planificador_corto_plazo() {
    /* while (control_key) {
        if (!list_is_empty(cola_ready) && !list_is_empty(cpus_libres)) {
            t_pcb* pcb = obtener_siguiente_ready();
            if (pcb != NULL) {
                despachar_a_cpu(pcb);
            }
        }
        usleep(100000); // 100ms de espera
    } */
}

t_pcb* obtener_siguiente_ready() {
    /* if (!list_is_empty(cola_ready)) {
        return list_remove(cola_ready, 0); // FIFO
    }
    return NULL; */
}

void despachar_a_cpu(t_pcb* pcb) {
    /* if (!list_is_empty(cpus_libres)) {
        int socket_cpu = (intptr_t)list_remove(cpus_libres, 0);
        pcb->estado_actual = EXEC;
        log_info(kernel_logger, "## (%d) Pasa del estado READY al estado EXEC", pcb->pid);
        enviar_operacion(EJECUTAR_PROCESO, socket_cpu);
        enviar_entero(pcb->pid, socket_cpu);
        enviar_entero(pcb->pc, socket_cpu);
        list_add(cpus_ocupadas, pcb);
        // La respuesta de la CPU se manejará en atender_cpu_kernel_dispatch
    } else {
        list_add(cola_ready, pcb); // Si no hay CPU libre, vuelve a READY
    } */
}

void manejar_syscall_init_proc(int pid, char* archivo, int tamanio) {
    /* t_pcb* nuevo_pcb = crear_pcb(pid, archivo, tamanio);
    agregar_a_new(nuevo_pcb); */
}

void manejar_syscall_io(int pid, char* dispositivo, int tiempo) {
    /* t_pcb* proceso = NULL;
    for (int i = 0; i < list_size(cpus_ocupadas); i++) {
        t_pcb* p = list_get(cpus_ocupadas, i);
        if (p->pid == pid) {
            proceso = p;
            break;
        }
    }
    if (proceso == NULL) {
        // Buscar en READY o NEW si fuera necesario
        log_warning(kernel_logger, "Proceso %d no encontrado para syscall IO.", pid);
        return;
    }

    if (existe_dispositivo_io(dispositivo)) {
        log_info(kernel_logger, "## (%d) - Bloqueado por IO: %s", pid, dispositivo);
        proceso->estado_actual = BLOCKED;
        bloquear_proceso_por_io(proceso, dispositivo);

        int socket_io_cliente = (intptr_t)dictionary_get(dispositivos_io_conectados, dispositivo);
        enviar_operacion(IO_PEDIDO, socket_io_cliente);
        enviar_entero(pid, socket_io_cliente);
        enviar_entero(tiempo, socket_io_cliente);

        // El proceso se eliminará de cpus_ocupadas y se agregará a la cola de bloqueados
        for (int i = 0; i < list_size(cpus_ocupadas); i++) {
            t_pcb* p = list_get(cpus_ocupadas, i);
            if (p->pid == pid) {
                list_remove(cpus_ocupadas, i);
                break;
            }
        }
        // Intentar despachar otro proceso
        if (!list_is_empty(cola_ready) && !list_is_empty(cpus_libres)) {
            t_pcb* siguiente_pcb = obtener_siguiente_ready();
            if (siguiente_pcb != NULL) {
                despachar_a_cpu(siguiente_pcb);
            }
        }

    } else {
        log_error(kernel_logger, "## (%d) - Intento de IO en dispositivo inexistente: %s", pid, dispositivo);
        // esto podría llevar a EXIT
    } */
}

void bloquear_proceso_por_io(t_pcb* pcb, char* dispositivo) {
    /* t_list* cola_bloqueados = dictionary_get(dispositivos_io_bloqueados, dispositivo);
    if (cola_bloqueados == NULL) {
        cola_bloqueados = list_create();
        dictionary_put(dispositivos_io_bloqueados, dispositivo, cola_bloqueados);
    }
    list_add(cola_bloqueados, pcb); */
} 


void finalizar_proceso(t_pcb* pcb) {
    /* // Informar a memoria
    enviar_operacion(FINALIZAR_PROCESO, socket_memoria);
    enviar_entero(pcb->pid, socket_memoria);
    recibir_operacion(socket_memoria); // Esperar confirmación

    pcb->estado_actual = EXIT;
    log_info(kernel_logger, "## (%d) - Finaliza el proceso", pcb->pid);
    free(pcb);
    // Intentar inicializar un nuevo proceso desde NEW
    intentar_inicializar_proceso(); */
}

void intentar_inicializar_proceso() {
    /* if (!list_is_empty(cola_new)) {
        t_pcb* pcb = list_first(cola_new);
        //En vez de enviar asi lo tenemos que hacer con la funcion enviar_paquete como esta hecho en
        //enviar_peticion_io
        enviar_operacion(INICIAR_PROCESO, socket_memoria);
        enviar_entero(pcb->pid, socket_memoria);
        char* archivo = "dummy.pseudocode"; 
        enviar_string(archivo, socket_memoria);
        enviar_entero(1024, socket_memoria); 

        int respuesta = recibir_operacion(socket_memoria);
        if (respuesta == OK) {
            list_remove(cola_new, 0);
            pcb->estado_actual = READY;
            list_add(cola_ready, pcb);
            log_info(kernel_logger, "## (%d) Pasa del estado NEW al estado READY", pcb->pid);
        }
    } */
} 