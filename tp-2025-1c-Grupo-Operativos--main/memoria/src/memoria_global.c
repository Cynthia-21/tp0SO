#include "memoria.h"

t_log* memoria_logger = NULL;
t_config* memoria_config = NULL;

char* PUERTO_ESCUCHA = NULL;

int TAM_MEMORIA = 0;
int TAM_PAGINA = 0;
int ENTRADAS_POR_TABLA = 0;
int CANTIDAD_NIVELES = 0;
int RETARDO_MEMORIA = 0;
char* PATH_SWAPFILE = NULL;
int RETARDO_SWAP = 0;
char* LOG_LEVEL = NULL;
char* DUMP_PATH = NULL;
char* PATH_INSTRUCCIONES = NULL;

int socket_memoria = -1;
int socket_cpu = -1;
int socket_kernel = -1;

t_dictionary* dispositivos_cpu_conectados;