#include "cpu.h"

t_log* cpu_logger = NULL;
t_config* cpu_config = NULL;

int socket_cpu = -1;
int socket_memoria = -1;
int socket_kernel_dispatch = -1;
int socket_kernel_interrupt = -1;
