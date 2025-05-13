#include "../include/memoria_kernel.h"
#include "../include/kernel.h"


// Petición a Memoria para inicializar un proceso.
// Retorna 1 si hay espacio en memoria, -1 en caso contrario.
int init_proceso_memoria(char* nombre_archivo, char* size){
    int socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    // Envio peticion a Memoria
    t_buffer* buffer = crear_buffer();
    cargar_string_al_buffer(buffer, nombre_archivo);
    cargar_string_al_buffer(buffer, size);
    t_paquete* paquete = crear_paquete(K_M_INIT_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);

	// Recibo respuesta de Memoria
    int operacion = recibir_operacion(socket_memoria);

    switch (operacion) {
        case M_K_INIT_PROCESO_OK:
            return 1;

        case M_K_RESPUESTA_ERROR:
            return -1;

        default:
            return -1;
    }
    
    close(socket_memoria);
}




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
    }
}