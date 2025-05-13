#ifndef KERNEL_PLANIFICADORES_H_
#define KERNEL_PLANIFICADORES_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>
#include <commons/log.h>
#include <commons/collections/dictionary.h>
#include <unistd.h>
#include "../include/io_kernel.h"
#include "../include/kernel.h"

// Funciones principales del planificador de largo plazo
void init_primer_proceso(char* nombre_archivo, char* size);
t_pcb* crear_pcb(int pid, char* archivo, int tamanio);
void agregar_a_new(t_pcb* pcb);
void planificador_largo_plazo();
void planificador_corto_plazo();
t_pcb* obtener_siguiente_ready();
t_pcb* siguiente_fifo();
t_pcb* siguiente_sjf();
void actualizar_estimacion(t_pcb* pcb, int tiempo_real);
void despachar_a_cpu(t_pcb* pcb);
void manejar_syscall_init_proc(int pid, char* archivo, int tamanio);
void finalizar_proceso(t_pcb* pcb);

#endif