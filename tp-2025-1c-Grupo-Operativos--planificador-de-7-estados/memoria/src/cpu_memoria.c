#include "../include/cpu_memoria.h"
#include "../include/memoria.h"


int manejar_operacion_cpu(op_code_t operacion, int socket) {

    switch (operacion) {
        case CPU_M_HANDSHAKE:
            handle_handshake_cpu(socket);
            break;

        case CPU_M_SOLICITAR_INSTRUCCION:
            handle_solicitar_instruccion(socket);
            break;

        case CPU_M_ACCESO_TABLA_PAGINAS:
            handle_acceso_tabla_paginas(socket); 
            break;
            
        case CPU_M_LEER_MEMORIA:
            handle_leer_memoria(socket); 
            break;

        case CPU_M_ESCRIBIR_MEMORIA:
            handle_escribir_memoria(socket); 
            break;

        case CPU_M_LEER_PAGINA_COMPLETA:
            handle_leer_pagina_completa(socket); 
            break;

        case CPU_M_ESCRIBIR_PAGINA_COMPLETA:
            handle_escribir_pagina_completa(socket); 
            break;

        case CPU_M_ESCRIBIR_PAGINA_MODIFICADA:
            handle_escribir_pagina_completa(socket); 
            break;

        case CPU_M_ELIMINAR_TLB_POR_PROCESO:
            handle_escribir_pagina_completa(socket); 
            break;

        case CPU_M_ELIMINAR_CACHE_POR_PROCESO:
            handle_escribir_pagina_completa(socket); 
            break;

        default:
            log_error(memoria_logger, "Operación CPU desconocida: %d", operacion); break;
    }

    return 1;
}


void handle_handshake_cpu(int socket) {
    // Recibo petición de Kernel
    int desplazamiento = 0;                        
    t_buffer* buffer = recibir_buffer(socket);
    char* msg_handshake = extraer_contenido_del_buffer2(buffer, &desplazamiento);
    
    log_debug(memoria_logger, "Mensaje de Handshake de CPU: %s", msg_handshake);

    // Libero buffers y paquetes
    free(buffer->stream);
    free(buffer);
}


void handle_solicitar_instruccion(int socket){
    // ...
}

void handle_acceso_tabla_paginas(int socket){
    // ...
}

void handle_leer_memoria(int socket){
    printf("CPU solicita leer memoria!\n");
    fflush(stdout);
}

void handle_escribir_memoria(int socket){
    // ...
}

void handle_leer_pagina_completa(int socket){
    // ...
}

void handle_escribir_pagina_completa(int socket){
    // ...
}




/* 
void* escuchar_cpu_memoria() {
    dispositivos_cpu_conectados = dictionary_create();

    while (1) {
        int socket_cpu_nuevo = esperar_cliente(socket_memoria, memoria_logger, "Nuevo CPU conectado a MEMORIA");

        if (socket_cpu_nuevo == -1) {
            log_error(memoria_logger, "Fallo al aceptar una conexión CPU.");
            continue;
        }

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = socket_cpu_nuevo;

        pthread_t hilo_cpu;
        pthread_create(&hilo_cpu, NULL, atender_cpu_memoria, socket_ptr);
        pthread_detach(hilo_cpu);

    }
    return NULL;
}
 */


void enviar_peticion_cpu(int socket_cpu_cliente, int pid, int tiempo) {

    t_buffer* buffer = crear_buffer();

    cargar_int_al_buffer(buffer, pid);
    cargar_int_al_buffer(buffer, tiempo);

    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    enviar_paquete(paquete, socket_cpu_cliente);

}

void simular_peticion_cpu(char* dispositivo) {
    if (dictionary_has_key(dispositivos_cpu_conectados, dispositivo)) {

        int socket_cpu_cliente = (intptr_t)dictionary_get(dispositivos_cpu_conectados, dispositivo);

        enviar_peticion_cpu(socket_cpu_cliente, 100, 2000);

    } else {
        log_error(memoria_logger, "Intento de I/O en dispositivo inexistente: %s", dispositivo);
    }
}



/* 

void* atender_cpu_memoria(void* arg){
	bool control_key = 1;
	while (control_key) {
			int socket_cpu = *((int*)arg);

			int cod_op = recibir_operacion(socket_cpu);

			switch (cod_op) {

			case HANDSHAKE:
				// Recibo nombre interfaz mediante handshake       
				t_buffer* buffer = recibir_buffer(socket_cpu);
				char* nombre_dispositivo = extraer_string_del_buffer(buffer);

				log_info(memoria_logger, "Dispositivo CPU conectado: %s", nombre_dispositivo);

				// Lo guardo en el diccionario de interfaces CPU
				dictionary_put(dispositivos_cpu_conectados, nombre_dispositivo, (void*)(intptr_t)socket_cpu);

				eliminar_buffer(buffer);
				free(nombre_dispositivo);

				break;
			case PEDIR_INSTRUCCION:
				buffer = recibir_buffer(socket_cpu);
				//buffer contiene pc y pid
				enviar_instruccion(&socket_cpu, buffer); // Deserializa y envia la instruccion

			case PAQUETE: // TODO: creo que no se usa

				t_buffer* respuesta_buffer = recibir_buffer(socket_memoria);
				int pid = extraer_int_del_buffer(respuesta_buffer);
				char* nombre_interfaz = extraer_string_del_buffer(respuesta_buffer);

				if(dictionary_has_key(dispositivos_cpu_conectados, nombre_interfaz)){
					
					log_info(memoria_logger, "## (%d) finalizó CPU en %s y pasa a READY", pid, nombre_interfaz);

					eliminar_buffer(respuesta_buffer); 
				}
			
				break;
			case -1:
				log_error(memoria_logger, "El CPU se desconecto. Terminando servidor");
				control_key = 0;
				break;
			default:
				log_warning(memoria_logger,"Operacion desconocida de CPU.");
				break;
		}
	}

    return NULL;	
}
 */