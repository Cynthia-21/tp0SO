#ifndef CPU_MEMORIA_H_
#define CPU_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

void* atender_cpu_memoria(void* arg);
void enviar_peticion_cpu(int socket_cpu_cliente, int pid, int tiempo);
void simular_peticion_cpu(char* dispositivo);

#endif