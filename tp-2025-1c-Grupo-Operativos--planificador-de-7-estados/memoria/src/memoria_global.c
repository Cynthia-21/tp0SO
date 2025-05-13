#include "../include/memoria.h"

t_log* memoria_logger = NULL;
t_config* memoria_config = NULL;
int socket_server = -1;
t_dictionary* dispositivos_cpu_conectados;

//Lista de instrucciones
t_list * lista_instrucciones_globales;


void* handle_connection(void* arg) {
    int client_socket = *((int*)arg);
    free(arg);  // Liberar la memoria para el socket

    int operacion = recibir_operacion(client_socket);

    // Si el que se conectó es Kernel --> Conexión efímera
    if (operacion >= K_M_HANDSHAKE && operacion <= K_M_MEMORY_DUMP) { 

        manejar_operacion_kernel(operacion, client_socket);
        close(client_socket);

    // Si el que se conectó es una CPU --> Conexión persistente
    } else if (operacion >= CPU_M_HANDSHAKE && operacion <= CPU_M_ELIMINAR_CACHE_POR_PROCESO) {

        // PENDIENTE Agregar CPU a la lista de CPUs conectados
        
        manejar_operacion_cpu(operacion, client_socket);
        
        while(true) {
            log_debug(memoria_logger, "Mandó un mensaje una CPU");

            int bytes = recv(client_socket, &operacion, sizeof(op_code_t), 0);
            if (bytes <= 0) {
                log_info(memoria_logger, "CPU desconectada.");
                break;
            }

            manejar_operacion_cpu(operacion, client_socket);
        }
        close(client_socket);

    } else {
        log_error(memoria_logger, "Código de operación desconocido: %d", operacion);
        close(client_socket);
    }

    return NULL;
}
