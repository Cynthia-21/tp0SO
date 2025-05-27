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


char* operacion_to_string(t_operacion op) {
    switch (op) {
        case NOOP: return strdup("NOOP");
        case READ: return strdup("READ");
        case WRITE: return strdup("WRITE");
        case INIT_PROC: return strdup("INIT_PROC");
        case IO: return strdup("I/O");
        case EXIT: return strdup("EXIT");
        default: return strdup("OPERACIÓN DESCONOCIDA");
    }
}


void imprimir_instrucciones() {
    for (int i = 0; i < list_size(lista_instrucciones_globales); i++) {
        t_instrucciones_proceso* proceso = list_get(lista_instrucciones_globales, i);
        printf("PID: %d\n", proceso->pid);

        for (int j = 0; j < list_size(proceso->lista_instrucciones); j++) {
            t_instruccion* instruccion = list_get(proceso->lista_instrucciones, j);

            // Imprimir operación
            char* nombre_operacion = operacion_to_string(instruccion->operacion); // función auxiliar opcional
            printf("  Operación: %s", nombre_operacion);

            // Imprimir parámetros
            if (instruccion->cantidad_parametros > 0) {
                printf(" | Parámetros: ");
                for (int k = 0; k < instruccion->cantidad_parametros; k++) {
                    printf("%s ", instruccion->parametros[k]);
                }
            }
            printf("\n");
            free(nombre_operacion); // Solo si usás strdup en operacion_to_string
        }
        printf("\n");
    }
}



// TODO: PENDIENTE logica real de verificacion de espacio en memoria
void handle_init_proceso(int socket) {
  
    t_buffer* buffer = recibir_buffer(socket);
    
    char* nombre_archivo = extraer_string_del_buffer(buffer);
    char* size = extraer_string_del_buffer(buffer);
    int pid = extraer_int_del_buffer(buffer);

    leer_instrucciones_desde_archivo(nombre_archivo, pid);

    // imprimir_instrucciones();  //borrar la funcion si no la vamos a usar mas

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

//TODO handles

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
