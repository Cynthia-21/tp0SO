#include "../include/kernel_io.h"
#include "../include/io.h"

void atender_kernel_io(){
	bool control_key = 1;
    while (control_key) {
	    int cod_op = recibir_operacion(socket_kernel);
	    switch (cod_op) {
	    case MENSAJE:
           
		    break;
	    case PAQUETE:
			//peticion recibida del kernel
		    t_buffer* buffer = recibir_buffer(socket_kernel);

            int pid = extraer_int_del_buffer(buffer);
            int tiempo_io = extraer_int_del_buffer(buffer);

            log_info(io_logger, "## PID: %d - Inicio de IO - Tiempo: %d", pid, tiempo_io);

			// iniciar un usleep por el tiempo indicado en la request
            usleep(tiempo_io * 1000);

            log_info(io_logger, "## PID: %d - Fin de IO", pid);

			eliminar_buffer(buffer);

            // Crear un buffer para la respuesta que contenga el estado de la solicitud
			t_buffer* respuesta = crear_buffer();
			cargar_int_al_buffer(respuesta, 1);
			t_paquete* paquete = crear_paquete(FIN_IO, respuesta);
			
			// Enviar el paquete de vuelta al kernel
			enviar_paquete(paquete, socket_kernel);
			
		    break;
	    case -1:
		    log_error(io_logger, "El Kernel se desconecto. Terminando servidor");
		    control_key = 0;
            break;
	    default:
		    log_warning(io_logger,"Operacion desconocida de kernel.");
		    break;
	}
}}