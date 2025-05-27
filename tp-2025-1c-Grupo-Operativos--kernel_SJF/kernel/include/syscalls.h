#ifndef SYSCALLS_H_
#define SYSCALLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <utils/utils.h>

void syscall_init_proc(int pid, char* archivo, int tamanio);
void syscall_exit(int pid);
void syscall_dump_memory(int pid);
void syscall_error(int pid);


//-----------------Entrada Salida---------------------
void enviar_peticion_io(int socket_io_cliente, int pid, int tiempo);
void atender_fin_io(int socket);
void syscall_io(char* dispositivo, int tiempo, int pid, int pc);
void atender_desconexion_io(int socket_io);
void ejecutar_siguiente(char* nombre_io);
void simular_llamada_syscall_io(int cpu_id, char* nombre_dispositivo);
void recibir_io(char* nombre_io, int socket);


#endif