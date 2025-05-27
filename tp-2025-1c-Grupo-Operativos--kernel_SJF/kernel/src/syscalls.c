#include "../include/kernel.h"
#include "syscalls.h"



void syscall_exit(int pid){
    log_info(kernel_logger, "## PID: %d - Solicitó syscall: EXIT", pid);

    t_cpu* cpu_encontrada = NULL;

    void buscar_cpu_por_pid(char* key, void* value) {
    t_cpu* cpu = (t_cpu*)value;
        if (cpu->pcb != NULL && cpu->pcb->PID == pid) {
            cpu_encontrada = cpu;
        }
    }

    dictionary_iterator(cpus, buscar_cpu_por_pid);

    if (cpu_encontrada == NULL) {
        log_error(kernel_logger, "No se encontró ninguna CPU ejecutando el PID %d", pid);
        return;
    }

    cpu_encontrada->pcb->estado_actual = EXIT;
    cpu_encontrada->pcb = NULL;

    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pid);
    t_paquete* paquete = crear_paquete(K_M_FINALIZAR_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);
}


void syscall_init_proc(int pid, char* archivo, int tamanio){
    
    log_info(kernel_logger, "## PID: %d - Solicitó syscall: INIT_PROC", pid);
    
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pid);
    t_paquete* paquete = crear_paquete(K_M_INIT_PROCESO, buffer);
    enviar_paquete(paquete, socket_memoria);
}


void syscall_dump_memory(int pid){

    log_info(kernel_logger, "## PID: %d - Solicitó syscall: DUMP_MEMORY", pid);
    
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pid);
    t_paquete* paquete = crear_paquete(K_M_MEMORY_DUMP, buffer);
    enviar_paquete(paquete, socket_memoria);
}

void syscall_error(int pid){
    
    log_error(kernel_logger, "Proceso %d generó error de syscall. Se enviará a EXIT.", pid);

    t_cpu* cpu_encontrada = NULL;

    void buscar_cpu_por_pid(char* key, void* value) {
    t_cpu* cpu = (t_cpu*)value;
        if (cpu->pcb != NULL && cpu->pcb->PID == pid) {
            cpu_encontrada = cpu;
        }
    }

    dictionary_iterator(cpus, buscar_cpu_por_pid);

    if (cpu_encontrada == NULL) {
        log_error(kernel_logger, "No se encontró ninguna CPU ejecutando el PID %d", pid);
        return;
    }

    t_pcb* pcb = cpu_encontrada->pcb;
    
    cpu_encontrada->pcb = NULL;
    cpu_encontrada->pcb->estado_actual = EXIT;

}


//------------------------------------------------Entrada Salida-------------------------------------------------

/* Kernel deberá conocer todos los módulos de IO conectados,
   qué procesos están ejecutando IO en cada módulo y todos los procesos que están esperando una IO determinada. */

// Semáforos para sincronización
pthread_mutex_t mutex_dispositivos = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_ios = PTHREAD_MUTEX_INITIALIZER;

void syscall_io(char* nombre_io, int tiempo, int pid, int pc) {

    t_pcb_io* pcb_io = malloc(sizeof(t_pcb_io));
    t_pcb* pcb = NULL;
    t_cpu* cpu_encontrada = NULL;

    void buscar_cpu_por_pid(char* key, void* value) {
    t_cpu* cpu = (t_cpu*)value;
        if (cpu->pcb != NULL && cpu->pcb->PID == pid) {
            cpu_encontrada = cpu;
        }
    }

    dictionary_iterator(cpus, buscar_cpu_por_pid);

    if (pcb_io == NULL) {
        log_error(kernel_logger, "Fallo al reservar memoria para PCB IO");
        return;
    }

    log_info(kernel_logger, "## PID: %d - Solicitó syscall: I/O", pid);

    pcb = cpu_encontrada->pcb;
    cpu_encontrada->pcb = NULL;

    // Verifico que exista el io
    if (!dictionary_has_key(ios, nombre_io)) {
        log_error(kernel_logger, "## (%d) - Intento de IO en dispositivo inexistente: %s", pid, nombre_io);
        pcb->estado_actual = EXIT;
        log_info(kernel_logger, "## (%d) Pasa del estado EXECUTE al estado EXIT", pid);
        free(pcb);
        free(pcb_io);
        return;
    }

    // Bloqeo la pcb y la encolo en el io solicitado
    pcb->estado_actual = BLOCKED;
    pcb->PC = pc;
    pcb_io->pcb = pcb;
    pcb_io->tiempo_io = tiempo;
    t_io* io = dictionary_get(ios, nombre_io);
    queue_push(io->procesos, pcb_io);

    log_info(kernel_logger, "## (%d) - Bloqueado por IO: %s", pcb->PID, nombre_io);
    log_info(kernel_logger, "## (%d) Pasa del estado EXECUTE al estado BLOCKED", pcb->PID);

    ejecutar_siguiente(nombre_io);
}

void ejecutar_siguiente(char* nombre_io) {
    pthread_mutex_lock(&mutex_ios);
    //Me traigo la cola de procesos en espera de la io solicitada
    t_io* io = dictionary_get(ios, nombre_io);

    //Verifico si esa IO tiene procesos en espera
    if (io->procesos == NULL || queue_is_empty(io->procesos)) {
        pthread_mutex_unlock(&mutex_ios);
        return;
    }
    pthread_mutex_unlock(&mutex_ios);

    //Busco un dispositivo libre
    int socket_libre = -1;

    pthread_mutex_lock(&mutex_dispositivos);

    void buscar_libre(char* key, void* value) {
    t_dispositivo_io* disp = (t_dispositivo_io*)value;
        if (strcmp(disp->nombre_io, nombre_io) == 0 && disp->pcb_io == NULL) {
            socket_libre = atoi(key);
        }
    }

    dictionary_iterator(dispositivos, buscar_libre);
    if (socket_libre == -1) {
        pthread_mutex_unlock(&mutex_dispositivos);
        return; //El proceso deberá esperar a que se libere algún dispositivo
    }

    //Saco el proceso de la lista de espera en IO y lo asigno a ese dispositivo libre
    pthread_mutex_lock(&mutex_ios);
    t_pcb_io* pcb_io = queue_pop(io->procesos);
    pthread_mutex_unlock(&mutex_ios);

    t_dispositivo_io* dispositivo_libre = dictionary_get(dispositivos, string_itoa(socket_libre));
    dispositivo_libre->pcb_io = pcb_io;
    pthread_mutex_unlock(&mutex_dispositivos);

    //Ejecuto el proceso del respectivo dispositivo
    enviar_peticion_io(socket_libre, pcb_io->pcb->PID, pcb_io->tiempo_io);
    log_info(kernel_logger, "Se envió el proceso %d al dispositivo IO (%s)", pcb_io->pcb->PID, nombre_io);
}

void atender_fin_io(int socket) {
    pthread_mutex_lock(&mutex_dispositivos);
    //Me traigo el dispositivo que terminó
    t_dispositivo_io* dispositivo = dictionary_get(dispositivos, string_itoa(socket));
    if (dispositivo == NULL || dispositivo->pcb_io == NULL) {
        pthread_mutex_unlock(&mutex_dispositivos);
        return;
    }

    //Extraigo su proceso que estaba ejecutando
    t_pcb_io* pcb_io = dispositivo->pcb_io;
    dispositivo->pcb_io = NULL;
    pthread_mutex_unlock(&mutex_dispositivos);

    //Lo mando a Ready
    pthread_mutex_lock(&mutex_cola_ready);
    queue_push(cola_ready, pcb_io->pcb);
    pthread_mutex_unlock(&mutex_cola_ready);

    log_info(kernel_logger, "## (%d) finalizó IO y pasa a READY", pcb_io->pcb->PID);

    //Ejecuto el siguiente en la espera de su IO
    ejecutar_siguiente(dispositivo->nombre_io);
}

void atender_desconexion_io(int socket) {
    pthread_mutex_lock(&mutex_dispositivos);
    t_dispositivo_io* dispositivo = dictionary_get(dispositivos, string_itoa(socket));
    if (dispositivo == NULL) {
        pthread_mutex_unlock(&mutex_dispositivos);
        log_error(kernel_logger, "Dispositivo IO no encontrado");
        return;
    }

    char* nombre_io = strdup(dispositivo->nombre_io); // Necesario fuera del mutex
    t_pcb_io* pcb_io = dispositivo->pcb_io;

    log_warning(kernel_logger, "Un dispositivo IO (%s) se desconectó", dispositivo->nombre_io);

    if (pcb_io != NULL) {
        //Si dispositivo estaba ejecutando un proceso => lo mando a EXIT
        pcb_io->pcb->estado_actual = EXIT;
        log_info(kernel_logger, "## (%d) - Finaliza por desconexión de IO (%s)", pcb_io->pcb->PID, dispositivo->nombre_io);
    }

    dictionary_remove(dispositivos, string_itoa(socket));
    pthread_mutex_unlock(&mutex_dispositivos);

    pthread_mutex_lock(&mutex_ios);
    t_io* io = dictionary_get(ios, nombre_io);
    io->conectados--;

    if (io->conectados == 0) {
        //Si es el último dispositivo
        //=> Elimino esa IO para que no se puedan encolar más procesos en ella
        dictionary_remove(ios, nombre_io);
    }
    pthread_mutex_unlock(&mutex_ios);

    free(nombre_io);
}

void recibir_io(char* nombre_io, int socket) {
    pthread_mutex_lock(&mutex_ios);
    t_io* io;
    t_dispositivo_io* dispositivo;

    //Creo la nueva IO si es que no estaba
    if (!dictionary_has_key(ios, nombre_io)) {
        io = malloc(sizeof(t_io));
        io->procesos = queue_create();
        io->conectados = 0;
        dictionary_put(ios, nombre_io, io);
    }
    pthread_mutex_unlock(&mutex_ios);

    //Creo el dispositivo y lo añado a mi diccionario
    dispositivo = malloc(sizeof(t_dispositivo_io));
    dispositivo->nombre_io = strdup(nombre_io);
    dispositivo->pcb_io = NULL;

    pthread_mutex_lock(&mutex_dispositivos);
    dictionary_put(dispositivos, string_itoa(socket), dispositivo);
    pthread_mutex_unlock(&mutex_dispositivos);

    pthread_mutex_lock(&mutex_ios);
    io = dictionary_get(ios, nombre_io);
    io->conectados++;
    pthread_mutex_unlock(&mutex_ios);

    log_info(kernel_logger, "Dispositivo I/O conectado: %s", nombre_io);
}

void enviar_peticion_io(int socket, int PID, int tiempo) {
    //Armo la solicitud
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, PID);
    cargar_int_al_buffer(buffer, tiempo);
    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    //La envío
    enviar_paquete(paquete, socket);
}


