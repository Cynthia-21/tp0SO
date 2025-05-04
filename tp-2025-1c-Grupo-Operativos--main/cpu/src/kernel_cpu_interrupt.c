#include "../include/kernel_cpu_interrupt.h"
#include "../include/cpu.h"

void atender_kernel_cpu_interrupt(){
    while (control_key) {
	    int cod_op = recibir_operacion(socket_kernel_interrupt);
	    switch (cod_op) {
	    case MENSAJE:
		
		    break;
	    case PAQUETE:
		
		    log_info(cpu_logger, "Me llegaron los siguientes valores:\n");
		
		    break;
	    case -1:
		    log_error(cpu_logger, "El kernel se desconecto. Terminando servidor");
		    control_key = 0;
			break;
	    default:
		    log_warning(cpu_logger,"Operacion desconocida de kernel Interrupt.");
		    break;
	}
}}