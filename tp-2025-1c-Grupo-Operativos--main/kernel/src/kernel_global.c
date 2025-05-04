#include "kernel.h"


// Logger y config
t_log* kernel_logger = NULL;
t_config* kernel_config = NULL;

// Conexiones
char* IP_MEMORIA = NULL;
char* PUERTO_MEMORIA = NULL;
char* PUERTO_ESCUCHA_DISPATCH = NULL;
char* PUERTO_ESCUCHA_INTERRUPT = NULL;
char* PUERTO_ESCUCHA_IO = NULL;

// Configuraciones generales
char* ALGORITMO_CORTO_PLAZO = NULL;
char* ALGORITMO_INGRESO_A_READY = NULL;
double ALFA = 0.0;
int TIEMPO_SUSPENSION = 0;
char* LOG_LEVEL = NULL;

// File descriptors
int socket_memoria = -1;
int socket_cpu_dispatch = -1;
int socket_cpu_interrupt = -1;
int socket_kernel = -1;
int socket_kernel_dispatch = -1;
int socket_kernel_interrupt = -1;
int socket_io = -1;


// Estructuras de datos
t_list* cola_new = NULL;
t_list* cola_ready = NULL;
t_list* cpus_libres = NULL;
t_list* cpus_ocupadas = NULL;


int proximo_pid = 0;

// Diccionario de Dispositivos IO
t_dictionary * dispositivos_io_conectados = NULL;
t_dictionary * dispositivos_io_bloqueados = NULL;

t_dictionary * dispositivos_cpu_dispatch_conectados = NULL;
t_dictionary * dispositivos_cpu_interrupt_conectados = NULL;

