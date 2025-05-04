#include "../include/kernel_memoria.h"
#include "../include/memoria.h"

void atender_kernel_memoria(){
	bool control_key = 1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket_kernel);
	    switch (cod_op) {
	    case MENSAJE:
		
		    break;
	    case PAQUETE:
		
		    log_info(memoria_logger, "Me llegaron los siguientes valores:\n");
		
		    break;
	    case -1:
		    log_error(memoria_logger, "El kernel se desconecto. Terminando servidor");
		    control_key = 0;

			break;
	    default:
		    log_warning(memoria_logger,"Operacion desconocida de kernel.");
		    break;
	}
}}