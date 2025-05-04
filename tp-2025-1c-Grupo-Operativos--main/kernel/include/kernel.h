#ifndef KERNEL_H_
#define KERNEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/utils.h>
#include <pthread.h>
#include "cpu_kernel_dispatch.h"
#include "cpu_kernel_interrupt.h"
#include "kernel_communications.h"
#include "io_kernel.h"
#include "memoria_kernel.h"

#include <commons/log.h>
#include <commons/config.h>
#include <commons/collections/list.h>

/* DEFINICIONES */

typedef enum {
    SYSCALL,
    INIT_PROC_SYSCALL,
    IO_SYSCALL,
    EXIT_SYSCALL,
    CPU_FINISHED
} t_kernel_op;

typedef enum {
    NEW,
    READY,
    EXEC,
    BLOCKED,
    EXIT
    //TODO: 7 Estados
} t_estado;

typedef struct {
    int pid;
    int pc;
    //TODO:
    //ME
    //MT
    t_estado estado_actual;
} t_pcb;

/* ESTRUCTURAS DE DATOS */

// Colas
extern t_list* cola_new;
extern t_list* cola_ready;
extern t_dictionary* dispositivos_io_bloqueados; // Clave: nombre IO, Valor: cola de PCBs

// CPU
extern t_list* cpus_libres; // Lista de sockets de CPU dispatch libres
extern t_list* cpus_ocupadas; // Lista de PCBs ejecutándose en CPUs

/* VARIABLES GLOBALES */
extern t_log* kernel_logger;
extern t_config* kernel_config;

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
extern int socket_kernel;
extern int socket_kernel_dispatch;
extern int socket_kernel_interrupt;
extern int socket_io;

// Diccionario de Dispositivos IO
extern t_dictionary* dispositivos_io_conectados;
extern t_dictionary* dispositivos_io_bloqueados;
extern t_dictionary* dispositivos_cpu_dispatch_conectados;
extern t_dictionary* dispositivos_cpu_interrupt_conectados;


/* DECLARACIONES DE FUNCIONES */

// Inicialización
void inicializar_kernel();
t_pcb* crear_pcb(int pid, char* archivo, int tamanio);

// Planificación de Largo Plazo
void planificador_largo_plazo();
void agregar_a_new(t_pcb* pcb);
void intentar_inicializar_proceso();

// Planificación de Corto Plazo
void planificador_corto_plazo();
t_pcb* obtener_siguiente_ready();
void despachar_a_cpu(t_pcb* pcb);

// Administración de IO (cyn)
/* void manejar_syscall_io(int pid, char* dispositivo, int tiempo);
void bloquear_proceso_por_io(t_pcb* pcb, char* dispositivo);
void atender_fin_io(int pid, char* dispositivo);
void registrar_io_conectado(char* nombre_io, int socket_io_cliente);
bool existe_dispositivo_io(char* dispositivo);*/

// Syscalls
void manejar_syscall_init_proc(int pid, char* archivo, int tamanio);

// Finalización de procesos
void finalizar_proceso(t_pcb* pcb);

#endif
