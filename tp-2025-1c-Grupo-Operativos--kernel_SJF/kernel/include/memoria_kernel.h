#ifndef CPU_KERNEL_IO_H_
#define CPU_KERNEL_IO_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>


void atender_memoria_kernel();
int init_proceso_memoria(int pid, char* nombre_archivo, char* size);

#endif