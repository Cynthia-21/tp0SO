#ifndef KERNEL_H_
#define KERNEL_H_

#define NUM_ESTADOS 7

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/utils.h>
#include <pthread.h>

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
    SUSP_READY,
    SUSP_BLOCKED,
    EXIT_P //el P es porque se chocaba con el EXIT de utils.h
} t_estado;
typedef struct {
    int PID;
    int PC;
    //orden: [NUEVO, READY, READY SUSPEND, BLOCKED, BLOCKED SUSPEND, EXEC, FINISH]
    int ME[NUM_ESTADOS]; //Métricas de Estado, cantidad de veces que el proceso estuvo en cada estado.
    time_t MT[NUM_ESTADOS]; //Métricas de Tiempo por estado, tiempo que el proceso estuvo en cada estado. (usar temporal.h de commons)   

    //Estas cosas no las pide el tp explicitamente
    double estimacion_actual;
    t_estado estado_actual;
    void* tabla_paginas;
    char* archivo;
} t_pcb;


// VARIABLES GLOBALES

extern t_pcb** colas_estado[NUM_ESTADOS];
extern int colas_sizes[NUM_ESTADOS]; // tamaño de cada cola (cantidad de procesos en cada cola)

/* ESTRUCTURAS DE DATOS */

extern t_dictionary* dispositivos; //key: socket
extern t_dictionary* ios; //key: nombre
typedef struct {
    t_pcb* pcb;
    int tiempo_io;
} t_pcb_io;
typedef struct {
    char* nombre_io;
    t_pcb_io* pcb_io;
} t_dispositivo_io;
typedef struct {
    t_queue* procesos;
    int conectados;
} t_io;

// Colas
extern t_queue* cola_new;
extern t_queue* cola_ready;
extern t_queue* cola_susp_ready;
extern t_queue* cola_susp_blocked;


// CPU
extern t_dictionary * cpus_libres; // Lista de sockets de CPU dispatch libres
extern t_dictionary * cpus_ocupadas; // Lista de PCBs ejecutándose en CPUs

/* VARIABLES GLOBALES */
extern t_log* kernel_logger;
extern t_config* kernel_config;
extern int PID;

// Para las conexiones //
extern char* IP_MEMORIA;
extern char* PUERTO_MEMORIA;
extern char* PUERTO_ESCUCHA_DISPATCH;
extern char* PUERTO_ESCUCHA_INTERRUPT;
extern char* PUERTO_ESCUCHA_IO;

// Para otras cositas 
extern char* ALGORITMO_CORTO_PLAZO;
extern char* ALGORITMO_INGRESO_A_READY;
extern double ALFA;
extern int TIEMPO_SUSPENSION;
extern char* LOG_LEVEL;

// File Descriptor de sockets
extern int socket_memoria;
extern int socket_cpu_dispatch;
extern int socket_cpu_interrupt;
extern int socket_kernel_io;
extern int socket_kernel_dispatch;
extern int socket_kernel_interrupt;

// Diccionario de Dispositivos IO
extern t_dictionary* dispositivos_cpu_dispatch_conectados;
extern t_dictionary* dispositivos_cpu_interrupt_conectados;


/* DECLARACIONES DE FUNCIONES */

// Inicialización
void inicializar_kernel();
t_pcb* crear_pcb(int pid, char* archivo, int tamanio);
void init_primer_proceso(char* nombre_archivo, char* size);
int init_proceso_memoria(char* nombre_archivo, char* size);

// Planificación de Largo Plazo
void planificador_largo_plazo();
void agregar_a_new(t_pcb* pcb);
void intentar_inicializar_proceso();

// Planificación de Corto Plazo
void planificador_corto_plazo();
t_pcb* obtener_siguiente_ready();
void despachar_a_cpu(t_pcb* pcb);

// Administración de Suspensión
void suspender_proceso(t_pcb* pcb);
void reanudar_proceso(t_pcb* pcb);
void verificar_procesos_suspendidos();

// Syscalls
void manejar_syscall_init_proc(int pid, char* archivo, int tamanio);

// Finalización de procesos
void finalizar_proceso(t_pcb* pcb);

// Algoritmos de planificación
t_pcb* siguiente_fifo();
t_pcb* siguiente_sjf();
void actualizar_estimacion(t_pcb* pcb, int tiempo_real);

#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "kernel_communications.h"
#include "io_kernel.h"
#include "memoria_kernel.h"

#endif
