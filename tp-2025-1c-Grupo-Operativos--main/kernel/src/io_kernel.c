#include "../include/io_kernel.h"
#include "../include/kernel.h"
#include <commons/log.h>
#include <unistd.h>

void* atender_io_kernel(void* arg){
    int socket_io = *((int*)arg);
    free(arg);
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(socket_io);

        switch (cod_op) {
        case HANDSHAKE:

			// Recibo nombre interfaz mediante handshake       
            t_buffer* buffer = recibir_buffer(socket_io);
            char* nombre_dispositivo = extraer_string_del_buffer(buffer);

            log_info(kernel_logger, "Dispositivo I/O conectado: %s", nombre_dispositivo);

            // Lo guardo en el diccionario de interfaces io
            dictionary_put(dispositivos_io_conectados, nombre_dispositivo, (void*)(intptr_t)socket_io);

			if(strcmp(nombre_dispositivo, "disco") == 0){
				// Simular primer peticion a disco
				simular_peticion_io("disco");
			}

			eliminar_buffer(buffer);
            free(nombre_dispositivo);

            break;

        case PAQUETE:

			t_buffer* respuesta_buffer = recibir_buffer(socket_io);
			int pid = extraer_int_del_buffer(respuesta_buffer);
			char* nombre_interfaz = extraer_string_del_buffer(respuesta_buffer);

			if(dictionary_has_key(dispositivos_io_conectados, nombre_interfaz)){
				
				atender_fin_io(pid, nombre_interfaz);

				eliminar_buffer(respuesta_buffer); 
			}

            break;
        case -1:
            log_error(kernel_logger, "El IO se desconecto. Terminando servidor");
            control_key = 0;
            break;
        default:
            log_warning(kernel_logger, "Operacion desconocida de IO.");
            break;
        }
    }
    return NULL;
}
