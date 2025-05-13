#include "../include/memoria_cpu.h"
#include "../include/cpu.h"

void atender_memoria_cpu(t_log* cpu_logger){
	bool control_key = 1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket_memoria);
	    switch (cod_op) {
	    case MENSAJE:
		
		    break;
	    
	    case -1:
		    log_error(cpu_logger, "La memoria se desconecto. Terminando servidor");
		    control_key = 0;

			break;
	    default:
		    log_warning(cpu_logger,"Operacion desconocida de memoria.");
		    break;
		}
	}
}
