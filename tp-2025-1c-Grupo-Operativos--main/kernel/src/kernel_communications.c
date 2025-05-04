#include "../include/io_kernel.h"
#include "../include/kernel.h"
#include <commons/log.h>
#include <unistd.h>
#include "kernel_communications.h"

void* escuchar_io_kernel() {

    dispositivos_io_conectados = dictionary_create();

    while (1) {
        int socket_io_nuevo = esperar_cliente(socket_kernel, kernel_logger, "Nuevo I/O conectado a KERNEL");

        if (socket_io_nuevo == -1) {
            log_error(kernel_logger, "Fallo al aceptar una conexión IO.");
            continue;
        }

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = socket_io_nuevo;

        pthread_t hilo_io;
        pthread_create(&hilo_io, NULL, atender_io_kernel, socket_ptr);
        pthread_detach(hilo_io);

    }
    return NULL;
}

void simular_peticion_io(char* dispositivo) {
    if (dictionary_has_key(dispositivos_io_conectados, dispositivo)) {

        int socket_io_cliente = (intptr_t)dictionary_get(dispositivos_io_conectados, dispositivo);

        enviar_peticion_io(socket_io_cliente, 100, 2000);

    } else {
        log_error(kernel_logger, "Intento de I/O en dispositivo inexistente: %s", dispositivo);
    }
}

void enviar_peticion_io(int socket_io_cliente, int pid, int tiempo) {

    t_buffer* buffer = crear_buffer();

    cargar_int_al_buffer(buffer, pid);
    cargar_int_al_buffer(buffer, tiempo);

    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    enviar_paquete(paquete, socket_io_cliente);

    eliminar_paquete(paquete);

}

void atender_fin_io(int pid, char* dispositivo) {
    log_info(kernel_logger, "## (%d) finalizó IO en %s y pasa a READY", pid, dispositivo);
    /* t_list* cola_bloqueados = dictionary_get(dispositivos_io_bloqueados, dispositivo);
    if (cola_bloqueados != NULL) {
        for (int i = 0; i < list_size(cola_bloqueados); i++) {
            t_pcb* pcb = list_get(cola_bloqueados, i);
            if (pcb->pid == pid) {
                list_remove(cola_bloqueados, i);
                pcb->estado_actual = READY;
                list_add(cola_ready, pcb);
                log_info(kernel_logger, "## (%d) finalizó IO en %s y pasa a READY", pid, dispositivo);
                break;
            }
        }
    }
    // Intentar despachar si hay procesos en READY y CPUs libres
    if (!list_is_empty(cola_ready) && !list_is_empty(cpus_libres)) {
        t_pcb* siguiente_pcb = obtener_siguiente_ready();
        if (siguiente_pcb != NULL) {
            despachar_a_cpu(siguiente_pcb);
        }
    } */
}


void* escuchar_cpu_dispatch() {
    dispositivos_cpu_dispatch_conectados = dictionary_create();

    while (1) {
        int socket_nuevo = esperar_cliente(socket_kernel_dispatch, kernel_logger, "Nuevo CPU Dispatch conectado");

        if (socket_nuevo == -1) {
            log_error(kernel_logger, "Fallo al aceptar una conexión CPU Dispatch.");
            continue;
        }

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = socket_nuevo;

        pthread_t hilo_cpu;
        pthread_create(&hilo_cpu, NULL, atender_cpu_kernel_dispatch, socket_ptr);
        pthread_detach(hilo_cpu);
    }

    return NULL;
}

void* escuchar_cpu_interrupt() {
    dispositivos_cpu_interrupt_conectados = dictionary_create();

    while (1) {
        int socket_nuevo = esperar_cliente(socket_kernel_interrupt, kernel_logger, "Nuevo CPU Interrupt conectado");

        if (socket_nuevo == -1) {
            log_error(kernel_logger, "Fallo al aceptar una conexión CPU Interrupt.");
            continue;
        }

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = socket_nuevo;

        pthread_t hilo_cpu;
        pthread_create(&hilo_cpu, NULL, atender_cpu_kernel_interrupt, socket_ptr);
        pthread_detach(hilo_cpu);
    }

    return NULL;
}