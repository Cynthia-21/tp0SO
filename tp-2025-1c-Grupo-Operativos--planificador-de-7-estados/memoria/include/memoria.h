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

// Diccionario de CPUs
extern t_dictionary* dispositivos_cpu_conectados;

/* File Descriptors */
extern int socket_server;

/* FUNCIONES */
void inicializar_config(); 
void inicializar_logger();
void* handle_connection(void* arg);
char* puerto_escucha();

int tam_memoria(void);
int tam_pagina(void);
int entradas_por_tabla(void);
int cantidad_niveles(void);
int retardo_memoria(void);
char* path_swapfile(void);
int retardo_swap(void);
char* log_level(void);           
char* dump_path(void);
char* path_instrucciones(void);

// Gestión de CPU
int manejar_operacion_cpu(op_code_t operacion, int socket);
void handle_handshake_cpu(int socket);
void handle_solicitar_instruccion(int socket);
void handle_acceso_tabla_paginas(int socket);
void handle_leer_memoria(int socket);
void handle_escribir_memoria(int socket);
void handle_leer_pagina_completa(int socket);
void handle_escribir_pagina_completa(int socket);

void* escuchar_cpu_memoria();

//Funciones de memoria
t_operacion obtener_operacion_cpu(const char *operacion);
void leerInstruccionesDesdeArchivo(char *nombre_archivo,int pid_asociado);
t_list *obtener_instrucciones_proceso(int pid_enviado);
void serializar_instruccion(t_instruccion *instruccion, t_buffer *buffer);
void enviar_instruccion(int *socket_cpu, t_buffer *buffer);
int get_cant_parametros(t_operacion identificador); 


extern t_list * lista_instrucciones_globales;

typedef struct {
    t_list* lista_instrucciones;
    int pid;
}t_instrucciones_proceso;
#endif