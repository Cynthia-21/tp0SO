#ifndef KERNEL_PLANIFICADORES_H_
#define KERNEL_PLANIFICADORES_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <utils/utils.h>
#include <commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <unistd.h>
#include <pthread.h>

// Archivos locales
#include "io_kernel.h"
#include "kernel.h"

// Inicialización del primer proceso
void init_primer_proceso(char* nombre_archivo, char* size);

// Creación y gestión de PCB
t_pcb* crear_pcb(int pid, char* archivo, int tamanio);
void agregar_a_new(t_pcb* pcb);

// Planificadores
void planificador_largo_plazo();
void planificador_corto_plazo();

// Algoritmos de selección de procesos
t_pcb* obtener_siguiente_ready();
t_pcb* siguiente_fifo();
t_pcb* siguiente_sjf();
void actualizar_estimacion(t_pcb* pcb, int tiempo_real);

// Ejecución
void despachar_a_cpu();
void mover_a_ready(void);

// Syscalls y finalización
void manejar_syscall_init_proc(int pid, char* archivo, int tamanio);
void finalizar_proceso(t_pcb* pcb);

// Otros
void enviar_peticion_cpu(int socket, int PID, int PC);


#endif // KERNEL_PLANIFICADORES_H_