#include "kernel_planificadores.h"
#include <commons/collections/queue.h>
#include <commons/collections/dictionary.h>
#include <commons/collections/list.h>
#include <commons/temporal.h>

// Mutex
pthread_mutex_t mutex_planificacion;
pthread_mutex_t mutex_proceso_en_ejecucion;

// Estructuras
t_pcb* proceso_en_ejecucion = NULL;

static time_t obtener_timestamp_actual() {
    char* timestamp = temporal_get_string_time("%s");
    time_t t = (time_t)atoi(timestamp);
    free(timestamp);
    return t;
}

void init_primer_proceso(char* nombre_archivo, char* size) {
    printf("Presione ENTER para crear y planificar el primer proceso...\n");
    getchar();

    int pid = 1;
    int tamanio = atoi(size);
    t_pcb* pcb = crear_pcb(pid, nombre_archivo, tamanio);
    agregar_a_new(pcb);

    int response = init_proceso_memoria(pid, nombre_archivo, size);
    if (response < 0) {
        log_error(kernel_logger, "No se puede inicializar primer proceso");
        abort();
    }
}

int init_proceso_memoria(int pid, char* nombre_archivo, char* size) {
    socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);
    if (socket_memoria == -1) {
        log_error(kernel_logger, "Fallo al conectar con Memoria");
        return -1;
    }

    t_buffer* buffer = crear_buffer();
    cargar_string_al_buffer(buffer, nombre_archivo);
    cargar_string_al_buffer(buffer, size);
    cargar_int_al_buffer(buffer, pid);

    t_paquete* paquete = crear_paquete(K_M_INIT_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);

    pthread_t hilo_memoria;
    pthread_create(&hilo_memoria, NULL, (void*) atender_memoria_kernel, NULL);

    return 1;
}

t_pcb* crear_pcb(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = malloc(sizeof(t_pcb));
    if (!nuevo_pcb) {
        log_error(kernel_logger, "No se pudo asignar memoria para el PCB del proceso %d", pid);
        return NULL;
    }

    nuevo_pcb->PID = pid;
    nuevo_pcb->PC = 0;
    nuevo_pcb->archivo = strdup(archivo);
    nuevo_pcb->estado_actual = NEW;
    nuevo_pcb->estimacion_actual = 0;

    for (int i = 0; i < NUM_ESTADOS; i++) {
        nuevo_pcb->ME[i] = 0;
        nuevo_pcb->MT[i] = 0;
    }

    nuevo_pcb->ME[NEW] = 1;
    nuevo_pcb->MT[NEW] = obtener_timestamp_actual();

    log_info(kernel_logger, "## (%d) Se crea el proceso - Estado: NEW", pid);
    return nuevo_pcb;
}

void agregar_a_new(t_pcb* pcb) {
    pthread_mutex_lock(&mutex_cola_new);
    queue_push(cola_new, pcb);
    pthread_mutex_unlock(&mutex_cola_new);

    pcb->ME[NEW]++;
    pcb->MT[NEW] = obtener_timestamp_actual();
    log_info(kernel_logger, "## (%d) Proceso agregado a cola NEW", pcb->PID);
}

t_pcb* siguiente_fifo() {
    pthread_mutex_lock(&mutex_cola_ready);
    t_pcb* pcb = queue_pop(cola_ready);
    pthread_mutex_unlock(&mutex_cola_ready);
    return pcb;
}

t_pcb* siguiente_sjf() {
    pthread_mutex_lock(&mutex_cola_ready);
    t_list* lista_ready = list_create();

    while (!queue_is_empty(cola_ready)) {
        list_add(lista_ready, queue_pop(cola_ready));
    }

    if (list_is_empty(lista_ready)) {
        pthread_mutex_unlock(&mutex_cola_ready);
        list_destroy(lista_ready);
        return NULL;
    }

    t_pcb* mejor = list_get(lista_ready, 0);
    for (int i = 1; i < list_size(lista_ready); i++) {
        t_pcb* candidato = list_get(lista_ready, i);
        if (candidato->estimacion_actual < mejor->estimacion_actual) {
            mejor = candidato;
        }
    }

    list_remove_element(lista_ready, mejor);
    for (int i = 0; i < list_size(lista_ready); i++) {
        t_pcb* resto = list_get(lista_ready, i);
        queue_push(cola_ready, resto);
    }

    list_destroy(lista_ready);
    pthread_mutex_unlock(&mutex_cola_ready);

    return mejor;
}

t_pcb* obtener_siguiente_ready() {
    if (strcmp(ALGORITMO_CORTO_PLAZO, "FIFO") == 0)
        return siguiente_fifo();
    else if (strcmp(ALGORITMO_CORTO_PLAZO, "SJF") == 0)
        return siguiente_sjf();
    else {
        log_error(kernel_logger, "Algoritmo de planificación no reconocido: %s", ALGORITMO_CORTO_PLAZO);
        return NULL;
    }
}

void mover_a_ready() {

    pthread_mutex_lock(&mutex_cola_new);
    t_pcb* pcb = queue_pop(cola_new);
    pthread_mutex_unlock(&mutex_cola_new);

    if (!pcb) {
        log_error(kernel_logger, "Error: cola NEW vacía al intentar mover a READY");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_lock(&mutex_cola_ready);
    queue_push(cola_ready, pcb);
    pthread_mutex_unlock(&mutex_cola_ready);

    pcb->estado_actual = READY;
    pcb->ME[READY]++;
    pcb->MT[READY] = obtener_timestamp_actual();

    log_info(kernel_logger, "PID %d - Estado anterior: NEW - Estado actual: READY", pcb->PID);
}


void despachar_a_cpu() {
    pthread_mutex_lock(&mutex_cola_ready);
    if (queue_is_empty(cola_ready)) {
        pthread_mutex_unlock(&mutex_cola_ready);
        return;
    }

    t_pcb* pcb = queue_pop(cola_ready);
    pthread_mutex_unlock(&mutex_cola_ready);

    int socket_libre = -1;
    t_cpu* cpu_libre = NULL;

    void buscar_cpu_libre(char* key, void* value) {
        t_cpu* cpu = (t_cpu*) value;
        if (cpu->pcb == NULL && socket_libre == -1) {
            socket_libre = cpu->socket_dispatch;
            cpu_libre = cpu;
        }
    }

    pthread_mutex_lock(&mutex_cpus);
    dictionary_iterator(cpus, buscar_cpu_libre);
    pthread_mutex_unlock(&mutex_cpus);

    if (socket_libre == -1 || cpu_libre == NULL) {
        log_info(kernel_logger, "No hay CPU libre, reinsertando PID %d en cola READY", pcb->PID);
        pthread_mutex_lock(&mutex_cola_ready);
        queue_push(cola_ready, pcb);
        pthread_mutex_unlock(&mutex_cola_ready);
        return;
    }

    cpu_libre->pcb = pcb;

    pcb->estado_actual = EXEC;
    pcb->ME[EXEC]++;
    pcb->MT[EXEC] = obtener_timestamp_actual();

    proceso_en_ejecucion = pcb;

    enviar_peticion_cpu(socket_libre, pcb->PID, pcb->PC);

    log_info(kernel_logger, "PID %d - Estado anterior: READY - Estado actual: EXEC", pcb->PID);
    log_info(kernel_logger, "Se envió el proceso %d a la CPU con socket %d", pcb->PID, socket_libre);
}

void enviar_peticion_cpu(int socket, int PID, int PC) {
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, PID);
    cargar_int_al_buffer(buffer, PC);

    t_paquete* paquete = crear_paquete(K_CPU_EXEC_PROCESO, buffer);
    enviar_paquete(paquete, socket);
}

void actualizar_estimacion(t_pcb* pcb, int tiempo_real) {
    double anterior = pcb->estimacion_actual;
    pcb->estimacion_actual = ALFA * tiempo_real + (1 - ALFA) * anterior;
    log_info(kernel_logger, "PID %d - Estimación actualizada: %.2f → %.2f", pcb->PID, anterior, pcb->estimacion_actual);
}

void manejar_syscall_init_proc(int pid, char* archivo, int tamanio) {
    t_pcb* nuevo_pcb = crear_pcb(pid, archivo, tamanio);
    agregar_a_new(nuevo_pcb);
}

void finalizar_proceso(t_pcb* pcb) {
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pcb->PID);

    t_paquete* paquete = crear_paquete(K_M_FINALIZAR_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);

    int cod_op = recibir_operacion(socket_memoria);
    if (cod_op == M_K_RESPUESTA_OK) {
        pcb->estado_actual = EXIT_P;
        log_info(kernel_logger, "## (%d) - Finaliza el proceso", pcb->PID);
        free(pcb->archivo);
        free(pcb);
    } else {
        log_error(kernel_logger, "## (%d) Fallo al finalizar proceso", pcb->PID);
    }
}

void cargar_double_al_buffer(t_buffer* un_buffer, double valor) {
    agregar_a_buffer(un_buffer, &valor, sizeof(double));
}

void cargar_time_t_al_buffer(t_buffer* un_buffer, time_t tiempo) {
    agregar_a_buffer(un_buffer, &tiempo, sizeof(time_t));
}
