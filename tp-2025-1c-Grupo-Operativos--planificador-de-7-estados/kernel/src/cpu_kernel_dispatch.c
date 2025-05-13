#include "../include/cpu_kernel_dispatch.h"
#include "../include/kernel.h"

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

            if(hand_recibido == HAND_CPU_KERNEL_DIS) {
                log_info(kernel_logger, "ID de la CPU conectada: %s", cpu_id);
                
                // Lo guardo en el diccionario de interfaces io
                dictionary_put(dispositivos_cpu_dispatch_conectados, cpu_id, (void*)(intptr_t)socket_cpu_dispatch);
                
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
            case MENSAJE:
                log_info(kernel_logger, "Me llegaron los siguientes valores desde %d:\n", socket_cpu_dispatch);
                // Aquí puedes procesar el paquete recibido
                break;
            case SYSCALL_P: {
                /* 
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                int pid = extraer_int_del_buffer(buffer);
                t_operacion operacion = extraer_int_del_buffer(buffer);
                int cantidad_parametros = extraer_int_del_buffer(buffer);

                char** parametros = malloc(sizeof(char*) * cantidad_parametros); // para el caso de INIT_PROC o IO que tienn dos parametros
                for (int i = 0; i < cantidad_parametros; i++) {
                    parametros[i] = extraer_string_del_buffer(buffer);
                }
            
                int pid = recibir_entero(socket_cliente);
                int syscall_id = recibir_entero(socket_cliente);
                log_info(kernel_logger, "## (%d) - Solicitó syscall: %d", pid, syscall_id);

                switch (syscall_id) {
                    case INIT_PROC_SYSCALL: {
                        char* archivo = recibir_string(socket_cliente);
                        int tamanio = recibir_entero(socket_cliente);
                        proximo_pid++;
                        manejar_syscall_init_proc(proximo_pid - 1, archivo, tamanio);
                        free(archivo);
                        break;
                    }
                    case IO_SYSCALL: {
                        char* dispositivo = recibir_string(socket_cliente);
                        int tiempo = recibir_entero(socket_cliente);
                        manejar_syscall_io(pid, dispositivo, tiempo);
                        free(dispositivo);
                        break;
                    }
                    case EXIT_SYSCALL: {
                        t_pcb* proceso_a_finalizar = NULL;
                        for (int i = 0; i < list_size(cpus_ocupadas); i++) {
                            t_pcb* pcb = list_get(cpus_ocupadas, i);
                            if (pcb->pid == pid) {
                                proceso_a_finalizar = list_remove(cpus_ocupadas, i);
                                break;
                            }
                        }
                        if (proceso_a_finalizar != NULL) {
                            finalizar_proceso(proceso_a_finalizar);
                        } else {
                            log_warning(kernel_logger, "No se encontró el proceso %d para finalizar.", pid);
                        }
                        list_add(cpus_libres, (void*)(intptr_t)socket_cliente); // CPU se libera
                        break;
                    }
                    // Otros syscalls
                    default:
                        log_warning(kernel_logger, "Syscall desconocida %d del PID %d.", syscall_id, pid);
                        // Podría devolver la CPU a libres aquí según la syscall
                        list_add(cpus_libres, (void*)(intptr_t)socket_cliente);
                        break;
                } 
            */
                break;
            }
            case K_M_FINALIZAR_PROCESO: {
                /* int pid = recibir_entero(socket_cliente);
                int nuevo_pc = recibir_entero(socket_cliente);
                t_pcb* proceso_finalizado = NULL;
                for (int i = 0; i < list_size(cpus_ocupadas); i++) {
                    t_pcb* pcb = list_get(cpus_ocupadas, i);
                    if (pcb->pid == pid) {
                        proceso_finalizado = list_remove(cpus_ocupadas, i);
                        break;
                    }
                }
                if (proceso_finalizado != NULL) {
                    proceso_finalizado->pc = nuevo_pc;
                    proceso_finalizado->estado_actual = READY;
                    list_add(cola_ready, proceso_finalizado);
                    log_info(kernel_logger, "## (%d) Finalizó quantum y vuelve a READY", pid);
                }
                list_add(cpus_libres, (void*)(intptr_t)socket_cliente); */
                break;
            }
            case -1:
                log_error(kernel_logger, "El CPU (socket %d) se desconecto.", socket_cpu_dispatch);
                control_key = false;
                break;
            default:
                log_warning(kernel_logger, "Operacion desconocida de CPU Dispatch (socket %d).", cod_op);
                break;
        }
    }
    return NULL;
} 

/*
void * enviar_mensaje_cpu_dispatch() {
    int socket_cpu_dispatch = *((int*)arg);
    free(arg);
    char* mensaje = "Hola desde el kernel";
    enviar_mensaje(mensaje, socket_cpu_dispatch);
    return NULL;
}*/

