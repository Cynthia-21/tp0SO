#include "../include/cpu_kernel_interrupt.h"
#include "../include/kernel.h"

void* atender_cpu_kernel_interrupt(void* arg) {
	int socket_cpu_interrupt = *((int*)arg);
    free(arg);
	bool control_key =1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket_cpu_interrupt);
			switch (cod_op) {
			case MENSAJE:
			
				break;
			case HANDSHAKE:
				t_buffer* buffer = recibir_buffer(socket_cpu_interrupt);
				char* cpu_id = extraer_string_del_buffer(buffer);

				log_info(kernel_logger, "CPU (interrupt) conectada. CPU ID: %s", cpu_id);

				// registrar_cpu_conectado(cpu_id, socket_cpu_interrupt);

				eliminar_buffer(buffer);
				free (cpu_id);
			
				break;

			case PAQUETE:
				
				break;
			case -1:
				log_error(kernel_logger, "El CPU se desconecto. Terminando servidor");
				control_key = 0;
				break;
			default:
				log_warning(kernel_logger,"Operacion desconocida de CPU Interrupt.");
				break;
		}
	}
    return NULL;
}