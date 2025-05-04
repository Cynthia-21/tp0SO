/*
#include "kernel_connections.h"

int conexion_memoria = -1;
int conexion_cpu_dispatch = -1;
int conexion_cpu_interrupt = -1;
int socket_servidor_io = -1;
int socket_servidor_procesos = -1;

int conectar_memoria(t_config* config, t_log* logger) {
    char* ip_memoria = config_get_string_value(config, "IP_MEMORIA");
    char* puerto_memoria = config_get_string_value(config, "PUERTO_MEMORIA");

    conexion_memoria = crear_conexion(ip_memoria, puerto_memoria);

    if (conexion_memoria == -1) {
        log_error(logger, "Error al conectar con Memoria en %s:%s", ip_memoria, puerto_memoria);
    } else {
        log_info(logger, "Conexión exitosa con Memoria en %s:%s", ip_memoria, puerto_memoria);
    }

    free(ip_memoria); // Liberar la memoria obtenida de la configuración
    free(puerto_memoria);
    return conexion_memoria;
}

int conectar_cpu_dispatch(t_config* config, t_log* logger) {
    char* ip_cpu = config_get_string_value(config, "IP_KERNEL"); // x ahora asumi que la IP de CPU se define en la config del Kernel
    char* puerto_cpu_dispatch = config_get_string_value(config, "PUERTO_KERNEL_DISPATCH");

    conexion_cpu_dispatch = crear_conexion(ip_cpu, puerto_cpu_dispatch);

    if (conexion_cpu_dispatch == -1) {
        log_error(logger, "Error al conectar con CPU (Dispatch) en %s:%s", ip_cpu, puerto_cpu_dispatch);
    } else {
        log_info(logger, "Conexión exitosa con CPU (Dispatch) en %s:%s", ip_cpu, puerto_cpu_dispatch);
    }

    free(ip_cpu);
    free(puerto_cpu_dispatch);
    return conexion_cpu_dispatch;
}

int conectar_cpu_interrupt(t_config* config, t_log* logger) {
    char* ip_cpu = config_get_string_value(config, "IP_KERNEL"); // ver IP
    char* puerto_cpu_interrupt = config_get_string_value(config, "PUERTO_KERNEL_INTERRUPT");

    conexion_cpu_interrupt = crear_conexion(ip_cpu, puerto_cpu_interrupt);

    if (conexion_cpu_interrupt == -1) {
        log_error(logger, "Error al conectar con CPU (Interrupt) en %s:%s", ip_cpu, puerto_cpu_interrupt);
    } else {
        log_info(logger, "Conexión exitosa con CPU (Interrupt) en %s:%s", ip_cpu, puerto_cpu_interrupt);
    }

    free(ip_cpu);
    free(puerto_cpu_interrupt);
    return conexion_cpu_interrupt;
}

int iniciar_servidor_io(t_config* config, t_log* logger) {
    char* ip_escucha = config_get_string_value(config, "IP_KERNEL"); // El Kernel escucha las conexiones IO
    char* puerto_escucha_io = config_get_string_value(config, "PUERTO_ESCUCHA_IO");

    socket_servidor_io = iniciar_servidor(puerto_escucha_io, logger, "Módulo IO");

    if (socket_servidor_io == -1) {
        log_error(logger, "Error al iniciar el servidor para conexiones IO en %s:%s", ip_escucha, puerto_escucha_io);
    } else {
        log_info(logger, "Servidor IO iniciado y esperando conexiones en el puerto %s", puerto_escucha_io);
    }

    free(ip_escucha);
    free(puerto_escucha_io);
    return socket_servidor_io;
}

int iniciar_servidor_procesos(t_config* config, t_log* logger) {
    char* ip_escucha = config_get_string_value(config, "IP_KERNEL"); // El Kernel escucha las conexiones de nuevos procesos/consolas
    char* puerto_escucha_procesos = config_get_string_value(config, "PUERTO_ESCUCHA_DISPATCH"); // Usando el puerto dispatch para recibir nuevos procesos

    socket_servidor_procesos = iniciar_servidor(puerto_escucha_procesos, logger, "Nuevos Procesos/Consolas");

    if (socket_servidor_procesos == -1) {
        log_error(logger, "Error al iniciar el servidor para nuevos procesos en %s:%s", ip_escucha, puerto_escucha_procesos);
    } else {
        log_info(logger, "Servidor de procesos iniciado y esperando conexiones en el puerto %s", puerto_escucha_procesos);
    }

    free(ip_escucha);
    free(puerto_escucha_procesos);
    return socket_servidor_procesos;
}
*/