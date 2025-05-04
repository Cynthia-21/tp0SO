#include "../include/cpu_memoria.h"
#include "../include/memoria.h"

void enviar_peticion_cpu(int socket_cpu_cliente, int pid, int tiempo) {

    t_buffer* buffer = crear_buffer();

    cargar_int_al_buffer(buffer, pid);
    cargar_int_al_buffer(buffer, tiempo);

    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    enviar_paquete(paquete, socket_cpu_cliente);

    eliminar_paquete(paquete);

}

void simular_peticion_cpu(char* dispositivo) {
    if (dictionary_has_key(dispositivos_cpu_conectados, dispositivo)) {

        int socket_cpu_cliente = (intptr_t)dictionary_get(dispositivos_cpu_conectados, dispositivo);

        enviar_peticion_cpu(socket_cpu_cliente, 100, 2000);

    } else {
        log_error(memoria_logger, "Intento de I/O en dispositivo inexistente: %s", dispositivo);
    }
}

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

				if(strcmp(nombre_dispositivo, "disco") == 0){
					// Simular primer peticion a disco
					simular_peticion_cpu("disco");
				}

				eliminar_buffer(buffer);
				free(nombre_dispositivo);

				break;

			case PAQUETE:

				t_buffer* respuesta_buffer = recibir_buffer(socket_memoria);
				int pid = extraer_int_del_buffer(respuesta_buffer);
				char* nombre_interfaz = extraer_string_del_buffer(respuesta_buffer);

				if(dictionary_has_key(dispositivos_cpu_conectados, nombre_interfaz)){
					
					log_info(memoria_logger, "## (%d) finalizó IO en %s y pasa a READY", pid, nombre_interfaz);

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
