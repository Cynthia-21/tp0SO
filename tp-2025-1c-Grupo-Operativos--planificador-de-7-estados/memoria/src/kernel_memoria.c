#include "../include/kernel_memoria.h"
#include "../include/memoria.h"


int manejar_operacion_kernel(op_code_t operacion, int socket) {
    
    switch (operacion) {
        case K_M_INIT_PROCESO:
            handle_init_proceso(socket);
            break;

        case K_M_SUSPENDER_PROCESO:
            handle_suspender_proceso(socket);
            break;

        case K_M_REANUDAR_PROCESO:
            handle_reanudar_proceso(socket);
            break;

        case K_M_FINALIZAR_PROCESO:
            handle_finalizar_proceso(socket);
            break;

        case K_M_MEMORY_DUMP:
            handle_memory_dump(socket);
            break;

        default:
            return -1;
    }

    return 1;
}


// PENDIENTE logica real de verificacion de espacio en memoria
//Hecho el mock
void handle_init_proceso(int socket) {

    // Recibo petición de Kernel
    int desplazamiento = 0;
    t_buffer* buffer = recibir_buffer(socket);
    char* string_extraido = extraer_contenido_del_buffer2(buffer, &desplazamiento);
    char* size = extraer_contenido_del_buffer2(buffer, &desplazamiento);

    // Envio respuesta a Kernel
    t_buffer* buffer_rta = crear_buffer();
    char* variable = "OK";
    cargar_string_al_buffer(buffer_rta, variable);
    t_paquete* paquete = crear_paquete(M_K_INIT_PROCESO_OK, buffer_rta);
    enviar_paquete(paquete, socket);

    // Libero buffers y paquetes
    free(buffer->stream);
    free(buffer);
}



void handle_suspender_proceso(int socket){
    // ...
}


void handle_reanudar_proceso(int socket){
    // ...
}



void handle_finalizar_proceso(int socket){
    // ...
}


void handle_memory_dump(int socket){
    // ...
}





/*
// OLD
void atender_kernel_memoria(){
	bool control_key = 1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket);
	    switch (cod_op) {
	    case HANDSHAKE:
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
}}*/
