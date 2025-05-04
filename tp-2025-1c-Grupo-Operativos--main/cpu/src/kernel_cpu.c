
#include "../include/kernel_cpu.h"
#include "../include/cpu.h"

/**
 @fn atender_kernel_cpu
 @brief Maneja las operaciones recibidas desde un socket específico del kernel, procesando diferentes códigos de operación.
 @param socket Descriptor del socket desde el cual se reciben las operaciones.
 @param socket_name Nombre descriptivo del socket para propósitos de logging.
 */
void atender_kernel_cpu(int socket, const char* socket_name, t_log* cpu_logger) {
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(socket);
        switch (cod_op) {
            case MENSAJE:
            // Manejar el caso MENSAJE si es necesario
            break;
            case PAQUETE:
            log_info(cpu_logger, "Me llegaron los siguientes valores desde %s:\n", socket_name);
            break;
            case -1:
            log_error(cpu_logger, "El kernel (%s) se desconecto. Terminando servidor", socket_name);
            control_key = 0;
            break;
            default:
            log_warning(cpu_logger, "Operacion desconocida de kernel (%s).", socket_name);
            break;
        }
    }
}

/**
 @fn atender_kernel_cpu_interrupt
 @brief Función específica para manejar las operaciones recibidas desde el socket de interrupciones del kernel.
 */
void atender_kernel_cpu_interrupt(t_log* cpu_logger) {
    
    atender_kernel_cpu(socket_kernel_interrupt, "socket_kernel_interrupt", cpu_logger);
}

/**
 @fn atender_kernel_cpu_dispatch
 @brief Función específica para manejar las operaciones recibidas desde el socket de despacho del kernel.
 */
void atender_kernel_cpu_dispatch(t_log* cpu_logger) {
    atender_kernel_cpu(socket_kernel_dispatch, "socket_kernel_dispatch", cpu_logger);
}
