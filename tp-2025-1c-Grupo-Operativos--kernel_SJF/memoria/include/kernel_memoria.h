#ifndef KERNEL_MEMORIA_H_
#define KERNEL_MEMORIA_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

void atender_kernel_memoria();


// Gestión de Kernel
int manejar_operacion_kernel(op_code_t operacion, int client_socket);
void handle_handshake_kernel(int socket);
void handle_init_proceso(int socket);
void handle_suspender_proceso(int socket);
void handle_reanudar_proceso(int socket);
void handle_finalizar_proceso(int socket);
void handle_memory_dump(int socket);


#endif