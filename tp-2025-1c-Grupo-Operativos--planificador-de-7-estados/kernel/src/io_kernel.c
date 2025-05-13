#include "../include/io_kernel.h"
#include "../include/kernel.h"
#include "../include/administracion_io.h"
#include <commons/log.h>
#include <unistd.h>

bool pruebas1 = false;
bool pruebas2 = false;

void* atender_io_kernel(void* arg){
    int socket = *((int*)arg);
    free(arg);
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(socket);

        switch (cod_op) {
            case HANDSHAKE: { 
                // Recibo nombre interfaz mediante handshake       
                t_buffer* buffer = recibir_buffer(socket);
                char* nombre_io = extraer_string_del_buffer(buffer);

                // Lo guardo en el diccionario de interfaces io
                recibir_io(nombre_io, socket);

                // Verifico si tiene procesos en cola para ejecutarlos
                ejecutar_siguiente(nombre_io);

                if (!pruebas1) {
                    simular_llamada_syscall_io(0, "DISCO");
                    simular_llamada_syscall_io(1, "DISCO");
                    pruebas1 = true;
                }

                eliminar_buffer(buffer);
                free(nombre_io);

                break;
            }
            case FIN_IO: {
                t_buffer* buffer = recibir_buffer(socket);
                atender_fin_io(socket); //Solo atiendo la finalizacion
                free(buffer);
                break;
            }
            case -1: {
                atender_desconexion_io(socket);
                control_key = 0;

                //Si ya no quedan DISCOS no me deberia dejar hacer la syscall
                if (!pruebas2) {
                    simular_llamada_syscall_io(3, "DISCO");
                    pruebas2 = true;
                }

                break;
            }
            default: {
                log_warning(kernel_logger, "Operacion desconocida de IO: %d", cod_op);
                break;
            }
        }
    }
    return NULL;
}
