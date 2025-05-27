#include "../include/cpu_kernel_dispatch.h"
#include "../include/kernel.h"
#include "../include/syscalls.h"


#include <commons/log.h>
#include <commons/string.h>

void* atender_cpu_kernel_dispatch(void* arg) {
    int socket_cpu_dispatch = *((int*)arg);
    free(arg);
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(socket_cpu_dispatch);
        switch (cod_op) {

             case HANDSHAKE: // Recibo nombre interfaz 
                t_buffer* b_handshake_recv = recibir_buffer(socket_cpu_dispatch);
                int hand_recibido = extraer_int_del_buffer(b_handshake_recv);
                char* cpu_id = extraer_string_del_buffer(b_handshake_recv);
                
                t_buffer* b_handshake_respuesta = crear_buffer();

                if (hand_recibido == HAND_CPU_KERNEL_DIS) {
                    log_info(kernel_logger, "HANDSHAKE OK - ID de la CPU conectada: %s", cpu_id);

                    t_cpu* cpu = dictionary_get(cpus, cpu_id);

                    if (cpu == NULL) {
                        // No existe aún, la creamos
                        cpu = malloc(sizeof(t_cpu));
                        cpu->pcb = NULL;
                        dictionary_put(cpus, cpu_id, cpu); // Agregamos al diccionario
                    }

                    // Ya sea nueva o existente, actualizamos el socket de dispatch
                    cpu->socket_dispatch = socket_cpu_dispatch;

                    cargar_int_al_buffer(b_handshake_respuesta, RESULT_OK);
                }
                
                else {
                    log_error(kernel_logger, "No se pudo completar el HANDSHAKE con la CPU: %s", cpu_id);
                    log_error(kernel_logger, "Valor de HANDSHAKE recibido: %d, se esperaba: %d", hand_recibido, HAND_CPU_KERNEL_DIS);
                    
                    cargar_int_al_buffer(b_handshake_respuesta, RESULT_ERROR);
                }
                
                //Envio la respuesta del HAND
                t_paquete* paquete = crear_paquete(HANDSHAKE, b_handshake_respuesta);
                enviar_paquete(paquete, socket_cpu_dispatch);

                //libero memoria
                //eliminar_buffer(b_handshake_recv);
                free(cpu_id);
                
                break;
            

            case CPU_K_REPLANIFICAR: {
                log_info(kernel_logger, "CPU solicitó replanificación (socket %d)", socket_cpu_dispatch);
                break;
            }

            case CPU_K_INIT_PROC: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                
                int pid = extraer_int_del_buffer(buffer);
                char* archivo = extraer_string_del_buffer(buffer);
                int tamanio = atoi(extraer_string_del_buffer(buffer));

                syscall_init_proc(pid, archivo, tamanio);

                despachar_a_cpu();
                
                eliminar_buffer(buffer);
                free(archivo);
                break;
            }

            case CPU_K_SOLICITAR_IO: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                
                int pid = extraer_int_del_buffer(buffer);
                char* dispositivo = extraer_string_del_buffer(buffer);
                int tiempo = atoi(extraer_string_del_buffer(buffer));
                int pc = extraer_int_del_buffer(buffer);
                
                syscall_io(dispositivo, tiempo, pid, pc);

                eliminar_buffer(buffer);
                free(dispositivo);
                break;
            }

            case CPU_K_DUMP_MEMORY: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);

                int pid = extraer_int_del_buffer(buffer);

                syscall_dump_memory(pid);

                
                break;
            }

            case CPU_K_EXIT: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                int pid = extraer_int_del_buffer(buffer);
                syscall_exit(pid);
                
                eliminar_buffer(buffer);
                
                break;
            }

            case CPU_K_SYSCALL_ERROR: { //TODO: este caso no tiene sentido porque desde cpu solo se envia si es uno de los casos anteriores
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                int pid = extraer_int_del_buffer(buffer);
                syscall_error(pid);
                
                eliminar_buffer(buffer);
                break;
            }

            case -1:
                log_warning(kernel_logger, "El CPU (socket %d) se desconectó.", socket_cpu_dispatch);
                control_key = false;
                break;

            default:
                log_warning(kernel_logger, "Operacion desconocida de CPU Dispatch (socket %d).", socket_cpu_dispatch);
                break;
        }
    }
    return NULL;
}