/*
#ifndef KERNEL_CONNECTIONS_H_
#define KERNEL_CONNECTIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <utils/utils.h>
// #include "utils.h" // Se incluyen las utilidades para manejo de sockets
#include "kernel.h" // Asumiendo un archivo de configuración del Kernel
#include "so_commons/log.h" // Asumiendo el uso de la librería de logs

// Declaraciones de funciones para establecer conexiones
int conectar_memoria(t_config* config, t_log* logger);
int conectar_cpu_dispatch(t_config* config, t_log* logger);
int conectar_cpu_interrupt(t_config* config, t_log* logger);
int iniciar_servidor_io(t_config* config, t_log* logger);
int iniciar_servidor_procesos(t_config* config, t_log* logger);

// Variables globales para almacenar los sockets de conexión
extern int conexion_memoria;
extern int conexion_cpu_dispatch;
extern int conexion_cpu_interrupt;
extern int socket_servidor_io;
extern int socket_servidor_procesos;

#endif

*/