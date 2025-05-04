#ifndef KERNEL_COMMUNICATIONS_H_
#define KERNEL_COMMUNICATIONS_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

void enviar_peticion_io(int socket_io_cliente, int pid, int tiempo);
void simular_peticion_io(char* dispositivo);
void atender_fin_io(int pid, char* dispositivo);
void* escuchar_io_kernel();

void* escuchar_cpu_dispatch();
void* escuchar_cpu_interrupt();

#endif

