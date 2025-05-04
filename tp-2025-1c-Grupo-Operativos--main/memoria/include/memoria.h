#ifndef MEMORIA_H_
#define MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>
#include <pthread.h>
#include "kernel_memoria.h"
#include "cpu_memoria.h"

/* VARIABLES GLOBALES */
extern t_log* memoria_logger;
extern t_config* memoria_config;

// Conexiones
extern char* PUERTO_ESCUCHA;

// Diccionario de CPUs
extern t_dictionary* dispositivos_cpu_conectados;

// Configuraciones
extern int TAM_MEMORIA;
extern int TAM_PAGINA;
extern int ENTRADAS_POR_TABLA;
extern int CANTIDAD_NIVELES;
extern int RETARDO_MEMORIA;
extern char* PATH_SWAPFILE;
extern int RETARDO_SWAP;
extern char* LOG_LEVEL;
extern char* DUMP_PATH;
extern char* PATH_INSTRUCCIONES;

// File Descriptors
extern int socket_memoria;
extern int socket_cpu;
extern int socket_kernel;

#endif