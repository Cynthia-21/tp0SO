#include "../include/cpu_kernel_dispatch.h"
#include "../include/kernel.h"

#include <commons/log.h>
#include <commons/string.h>

extern t_log* kernel_logger;
extern t_list* cpus_libres;
extern t_list* cpus_ocupadas;
extern int proximo_pid;

void* atender_cpu_kernel_dispatch(void* arg) {
    int socket_cpu_dispatch = *((int*)arg);
    free(arg);
    bool control_key = 1;
    while (control_key) {
        int cod_op = recibir_operacion(socket_cpu_dispatch);

        switch (cod_op) {
            case MENSAJE: {
                char* mensaje = recibir_mensaje(socket_cpu_dispatch);
                //Recibir paquete y como lo mandan los pibes 
                //ejemplo: 
                //t_buffer* respuesta_buffer = recibir_buffer(socket_io);
			    //int pid = extraer_int_del_buffer(respuesta_buffer);
			    //char* nombre_interfaz = extraer_string_del_buffer(respuesta_buffer); 
                
                //las funciones recibir_entero, string, etc no existen y no las hagan. Usen lo que tenemos
            
                log_info(kernel_logger, "Mensaje de CPU: %s", mensaje);
                free(mensaje);
                break;
            }
            case SYSCALL: {
                /* int pid = recibir_entero(socket_cliente);
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
                } */
                break;
            }
            case CPU_FINISHED: {
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
