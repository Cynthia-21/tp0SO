#include "../include/cpu_kernel_dispatch.h"
#include "../include/kernel.h"

#include <commons/log.h>
#include <commons/string.h>
#include <commons/collections/dictionary.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Función para manejar la comunicación con la CPU
 * @param arg: Puntero al socket_cpu_dispatch
 * @return NULL
 */
void* atender_cpu_kernel_dispatch(void* arg) {
    int socket_cpu_dispatch = *((int*)arg);
    free(arg);
    bool control_key = true;
    
    while (control_key) {
        int cod_op = recibir_operacion(socket_cpu_dispatch);
        
        switch (cod_op) {
            case HANDSHAKE: {
                t_buffer* b_handshake_recv = recibir_buffer(socket_cpu_dispatch);
                int hand_recibido = extraer_int_del_buffer(b_handshake_recv);
                char* cpu_id = extraer_string_del_buffer(b_handshake_recv);
                
                t_buffer* b_handshake_respuesta = crear_buffer();

                if(hand_recibido == HAND_CPU_KERNEL_DIS) {
                    log_info(kernel_logger, "ID de la CPU conectada: %s", cpu_id);
                    
                    // Lo guardo en el diccionario de dispositivos CPU dispatch conectados
                    dictionary_put(dispositivos_cpu_dispatch_conectados, cpu_id, (void*)(intptr_t)socket_cpu_dispatch);
                    
                    cargar_int_al_buffer(b_handshake_respuesta, RESULT_OK);
                } else {
                    log_error(kernel_logger, "No se pudo completar el HANDSHAKE con la CPU: %s", cpu_id);
                    log_error(kernel_logger, "Valor de HANDSHAKE recibido: %d, se esperaba: %d", 
                              hand_recibido, HAND_CPU_KERNEL_DIS);
                    
                    cargar_int_al_buffer(b_handshake_respuesta, RESULT_ERROR);
                }
                
                // Envio la respuesta del HAND
                t_paquete* paquete = crear_paquete(HANDSHAKE, b_handshake_respuesta);
                enviar_paquete(paquete, socket_cpu_dispatch);
                eliminar_paquete(paquete);

                // Liberar memoria
                eliminar_buffer(b_handshake_recv);
                free(cpu_id);
                break;
            }
            
            case SYSCALL_P: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                int pid = extraer_int_del_buffer(buffer);
                t_operacion operacion = extraer_int_del_buffer(buffer);
                int cantidad_parametros = extraer_int_del_buffer(buffer);

                char** parametros = malloc(sizeof(char*) * cantidad_parametros);
                if (parametros == NULL) {
                    log_error(kernel_logger, "Error de memoria al procesar syscall");
                    eliminar_buffer(buffer);
                    break;
                }
                
                for (int i = 0; i < cantidad_parametros; i++) {
                    parametros[i] = extraer_string_del_buffer(buffer);
                }
                
                log_info(kernel_logger, "## (%d) - Solicitó syscall: %d", pid, operacion);

                // Buscar el PCB correspondiente
                t_pcb* pcb = buscar_pcb_por_pid(pid);
                if (pcb == NULL) {
                    log_error(kernel_logger, "No se encontró PCB para PID %d", pid);
                    
                    // Liberar memoria
                    for (int i = 0; i < cantidad_parametros; i++) {
                        free(parametros[i]);
                    }
                    free(parametros);
                    eliminar_buffer(buffer);
                    break;
                }

                switch (operacion) {
                    case INIT_PROC_SYSCALL:
                        if (cantidad_parametros >= 2) {
                            int nuevo_pid = ++proximo_pid;
                            int tamanio = atoi(parametros[1]);
                            manejar_syscall_init_proc(nuevo_pid, parametros[0], tamanio);
                        } else {
                            log_error(kernel_logger, "Parámetros insuficientes para INIT_PROC");
                        }
                        break;
                        
                    case IO_SYSCALL:
                        if (cantidad_parametros >= 2) {
                            int tiempo = atoi(parametros[1]);
                            manejar_syscall_io(pid, parametros[0], tiempo);
                        } else {
                            log_error(kernel_logger, "Parámetros insuficientes para IO");
                        }
                        break;
                        
                    case EXIT_SYSCALL:
                        finalizar_proceso(pcb);
                        // Liberar la CPU
                        liberar_cpu(socket_cpu_dispatch);
                        break;
                        
                    // Otros syscalls
                    default:
                        log_warning(kernel_logger, "Syscall desconocida %d del PID %d", operacion, pid);
                        // Podría devolver la CPU a libres aquí según la syscall
                        liberar_cpu(socket_cpu_dispatch);
                        break;
                }
                
                // Liberar memoria
                for (int i = 0; i < cantidad_parametros; i++) {
                    free(parametros[i]);
                }
                free(parametros);
                eliminar_buffer(buffer);
                break;
            }
            
            case K_M_FINALIZAR_PROCESO: {
                t_buffer* buffer = recibir_buffer(socket_cpu_dispatch);
                int pid = extraer_int_del_buffer(buffer);
                int nuevo_pc = extraer_int_del_buffer(buffer);
                
                t_pcb* pcb = buscar_pcb_por_pid(pid);
                if (pcb != NULL) {
                    pcb->PC = nuevo_pc;
                    actualizar_pcb_estado(pcb, READY);
                    queue_push(cola_ready, pcb);
                    log_info(kernel_logger, "## (%d) Finalizó quantum y vuelve a READY", pid);
                } else {
                    log_warning(kernel_logger, "No se encontró PCB para PID %d al finalizar quantum", pid);
                }
                
                // Liberar la CPU
                liberar_cpu(socket_cpu_dispatch);
                eliminar_buffer(buffer);
                break;
            }
            
            case -1:
                log_error(kernel_logger, "El CPU (socket %d) se desconectó", socket_cpu_dispatch);
                manejar_desconexion_cpu(socket_cpu_dispatch);
                control_key = false;
                break;
                
            default:
                log_warning(kernel_logger, "Operación desconocida de CPU Dispatch (socket %d): %d", 
                            socket_cpu_dispatch, cod_op);
                break;
        }
    }
    return NULL;
}

/**
 * @brief Busca un PCB por su PID en las diferentes estructuras
 * @param pid: PID a buscar
 * @return Puntero al PCB encontrado o NULL si no existe
 */
t_pcb* buscar_pcb_por_pid(int pid) {
    // Buscar en las CPUs ocupadas
    t_pcb* pcb = NULL;
    
    void _buscar_en_cpus(char* key, void* value) {
        t_pcb* pcb_actual = (t_pcb*)value;
        if (pcb_actual != NULL && pcb_actual->PID == pid) {
            pcb = pcb_actual;
        }
    }
    
    dictionary_iterator(cpus_ocupadas, _buscar_en_cpus);
    if (pcb != NULL) return pcb;
    
    // Buscar en ready
    bool _es_pcb_buscado(void* elemento) {
        t_pcb* pcb_actual = (t_pcb*)elemento;
        return pcb_actual->PID == pid;
    }
    
    return list_find(cola_ready->elements, _es_pcb_buscado);
}

/**
 * @brief Libera una CPU y la marca como disponible
 * @param socket_cpu: Socket de la CPU a liberar
 */
void liberar_cpu(int socket_cpu) {
    char* cpu_id = NULL;
    
    void _encontrar_cpu(char* key, void* value) {
        int socket = (int)(intptr_t)value;
        if (socket == socket_cpu) {
            cpu_id = string_duplicate(key);
        }
    }
    
    dictionary_iterator(dispositivos_cpu_dispatch_conectados, _encontrar_cpu);
    
    if (cpu_id != NULL) {
        // Pasar de ocupada a libre
        t_pcb* pcb = dictionary_remove(cpus_ocupadas, cpu_id);
        dictionary_put(cpus_libres, cpu_id, (void*)(intptr_t)socket_cpu);
        
        log_info(kernel_logger, "CPU %s liberada", cpu_id);
        free(cpu_id);
    } else {
        log_warning(kernel_logger, "No se encontró la CPU con socket %d para liberar", socket_cpu);
    }
}

/**
 * @brief Maneja la desconexión de una CPU
 * @param socket_cpu: Socket de la CPU desconectada
 */
void manejar_desconexion_cpu(int socket_cpu) {
    char* cpu_id = NULL;
    
    void _encontrar_cpu(char* key, void* value) {
        int socket = (int)(intptr_t)value;
        if (socket == socket_cpu) {
            cpu_id = string_duplicate(key);
        }
    }
    
    dictionary_iterator(dispositivos_cpu_dispatch_conectados, _encontrar_cpu);
    
    if (cpu_id != NULL) {
        // Eliminar de la lista de CPUs conectadas
        dictionary_remove(dispositivos_cpu_dispatch_conectados, cpu_id);
        
        // Si estaba ocupada, recuperar el proceso
        t_pcb* pcb = dictionary_remove(cpus_ocupadas, cpu_id);
        if (pcb != NULL) {
            log_warning(kernel_logger, "CPU %s desconectada mientras ejecutaba PID %d", cpu_id, pcb->PID);
            actualizar_pcb_estado(pcb, READY);
            queue_push(cola_ready, pcb);
        }
        
        // Eliminar de la lista de CPUs libres si existía
        dictionary_remove(cpus_libres, cpu_id);
        
        log_info(kernel_logger, "CPU %s completamente desconectada", cpu_id);
        free(cpu_id);
    } else {
        log_warning(kernel_logger, "No se pudo identificar la CPU desconectada (socket %d)", socket_cpu);
    }
}
