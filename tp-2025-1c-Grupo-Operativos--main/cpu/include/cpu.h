#ifndef CPU_H_
#define CPU_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>
#include <pthread.h>
#include "kernel_cpu.h"
#include "memoria_cpu.h"


/* VARIABLES GLOBALES */
extern t_log* cpu_logger;
extern t_config* cpu_config;

// File Descriptors
extern int socket_cpu;
extern int socket_memoria;
extern int socket_kernel_dispatch;
extern int socket_kernel_interrupt;

/* CONFIG */
// Conexiones
char* puerto_memoria();
char* ip_memoria();
char* puerto_kernel_dispatch();
char* puerto_kernel_interrupt();
char* ip_kernel();

char* entradas_tlb();
char* reemplazo_tlb();
char* entradas_cache();
char* reemplazo_cache();
char* retardo_cache();
char* log_level();

/* FUNCIONES */
void leer_config();
void inicializar_config();
t_log* inicializar_logger(char* nombre);
void cerrar_cpu();
void conexiones(char* cpu_id,t_log* cpu_logger);
void atender_memoria(t_log* cpu_logger);
void atender_kernel(t_log* cpu_logger);
void conectar_memoria(t_log* cpu_logger);
void conectar_kernel(char* cpu_id, t_log* cpu_logger); 

/* CICLO de INSTRUCCIONES */
t_instruccion* fetch(int pid, int pc, t_log* cpu_logger);
bool decode(t_instruccion* instruccion);
int execute (int pid, int* pc, t_instruccion* instruccion, t_log* cpu_logger);

t_instruccion* solicitar_instruccion_a_memoria(int pc, int pid);
t_instruccion* deserializar_instruccion_solicitada(t_buffer* buffer);

#endif