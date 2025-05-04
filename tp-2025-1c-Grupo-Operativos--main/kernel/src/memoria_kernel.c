#include "../include/memoria_kernel.h"
#include "../include/kernel.h"

void atender_memoria_kernel(){
	bool control_key = 1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket_memoria);
	    switch (cod_op) {
	    case MENSAJE:
		
		    break;
	    case PAQUETE:
		
		    log_info(kernel_logger, "Me llegaron los siguientes valores:\n");
		
		    break;
	    case -1:
		    log_error(kernel_logger, "La memoria se desconecto. Terminando servidor");
		    control_key = 0;
			break;
	    default:
		    log_warning(kernel_logger,"Operacion desconocida de memoria");
		    break;
	}
}}