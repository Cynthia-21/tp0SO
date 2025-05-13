#include "../include/io_kernel.h"
#include "../include/kernel.h"
#include <commons/log.h>
#include <unistd.h>
#include "kernel_communications.h"

void* escuchar_io_kernel() {

    ios = dictionary_create();
    dispositivos = dictionary_create();

    while (1) {
        int socket_io_nuevo = esperar_cliente(socket_kernel_io, kernel_logger, "Nuevo I/O conectado a KERNEL");

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