#include "../include/memoria_kernel.h"
#include "../include/kernel.h"
#include "../include/kernel_planificadores.h"

void atender_memoria_kernel() {
    bool control_key = true;

    while (control_key) {
        int cod_op = recibir_operacion(socket_memoria);
        switch (cod_op) {

            case M_K_INIT_PROCESO_OK:
                t_buffer* buffer = recibir_buffer(socket_memoria);
                eliminar_buffer(buffer);

                log_info(kernel_logger, "Memoria aceptó la creación del proceso");
                mover_a_ready();
                despachar_a_cpu();
                break;

            case M_K_PROCESO_SUSPENDIDO:
                log_info(kernel_logger, "Memoria confirmó suspensión del proceso");
                break;

            case M_K_PROCESO_REANUDADO:
                log_info(kernel_logger, "Memoria confirmó reanudación del proceso");
                break;

            case M_K_PROCESO_FINALIZADO:
                log_info(kernel_logger, "Memoria confirmó finalización del proceso");
                break;

            case M_K_DUMP_FINALIZADO:
                log_info(kernel_logger, "Memoria confirmó que el dump fue completado");
                break;

            case M_K_RESPUESTA_OK:
                log_info(kernel_logger, "Memoria respondió OK");
                break;

            case M_K_RESPUESTA_ERROR:
                log_warning(kernel_logger, "Memoria respondió con un ERROR");
                break;
				
            case -1:
                log_warning(kernel_logger, "La memoria se desconectó");
                control_key = false;
                break;

            default:
                log_warning(kernel_logger, "Operación desconocida recibida de memoria (código %d)", cod_op);
                break;
        }
    }
}
