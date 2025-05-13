#include "../include/kernel.h"
#include "administracion_io.h"

// Crea un proceso de prueba
t_pcb* proceso_de_cpu(int cpu_id) {
    if (cola_ready == NULL)
        cola_ready = queue_create();

    t_pcb* pcb = malloc(sizeof(t_pcb));
    pcb->PID = cpu_id;
    pcb->estado_actual = EXEC;
    return pcb;
}

void simular_llamada_syscall_io(int cpu_id, char* nombre_io) {
    t_pcb* pcb_prueba = proceso_de_cpu(cpu_id);

    // Simulo una syscall IO válida de 10 segundos
    syscall_io(nombre_io, 10000, pcb_prueba);
}

//------------------------------------------------Entrada Salida-------------------------------------------------

/* Kernel deberá conocer todos los módulos de IO conectados,
   qué procesos están ejecutando IO en cada módulo y todos los procesos que están esperando una IO determinada. */

void syscall_io(char* nombre_io, int tiempo, t_pcb* pcb) {
    t_pcb_io* pcb_io = malloc(sizeof(t_pcb_io));

    if (pcb_io == NULL) {
        log_error(kernel_logger, "Fallo al reservar memoria para PCB IO");
        return;
    }

    log_info(kernel_logger, "## (%d) - Solicitó syscall: IO", pcb->PID);

    // Verifico que exista el io
    if (!dictionary_has_key(ios, nombre_io)) {
        log_error(kernel_logger, "## (%d) - Intento de IO en dispositivo inexistente: %s", pcb->PID, nombre_io);
        pcb->estado_actual = EXIT;
        log_info(kernel_logger, "## (%d) Pasa del estado EXECUTE al estado EXIT", pcb->PID);
        free(pcb);
        free(pcb_io);
        return;
    }

    // Bloqeo la pcb y la encolo en el io solicitado
    pcb->estado_actual = BLOCKED;
    pcb_io->pcb = pcb;
    pcb_io->tiempo_io = tiempo;
    t_io* io = dictionary_get(ios, nombre_io);
    queue_push(io->procesos, pcb_io);

    log_info(kernel_logger, "## (%d) - Bloqueado por IO: %s", pcb->PID, nombre_io);
    log_info(kernel_logger, "## (%d) Pasa del estado EXECUTE al estado BLOCKED", pcb->PID);

    ejecutar_siguiente(nombre_io);
}

void ejecutar_siguiente(char* nombre_io) {
    //Me traigo la cola de procesos en espera de la io solicitada
    t_io* io = dictionary_get(ios, nombre_io);

    //Verifico si esa IO tiene procesos en espera
    if (io->procesos == NULL || queue_is_empty(io->procesos)) return;

    //Busco un dispositivo libre
    int socket_libre = -1;

void buscar_libre(char* key, void* value) {
     t_dispositivo_io* disp = (t_dispositivo_io*)value;
     if (strcmp(disp->nombre_io, nombre_io) == 0 && disp->pcb_io == NULL) {
       socket_libre = atoi(key);
       return;
     }
}
    dictionary_iterator(dispositivos, buscar_libre);

    if (socket_libre == -1) return; //El proceso debera esperar a que se libere algun dispositivo

    //Saco el proceso de la lista de espera en IO y lo asigno a ese dispositivo libre
    t_pcb_io* pcb_io = queue_pop(io->procesos);

    // Uso el itoa de commons para evitar leaks
    char* socket_str = string_itoa(socket_libre);
    t_dispositivo_io* dispositivo_libre = dictionary_get(dispositivos, socket_str);
    free(socket_str);

    dispositivo_libre->pcb_io = pcb_io;

    //Ejecuto el proceso del respectivo dispositivo
    enviar_peticion_io(socket_libre, pcb_io->pcb->PID, pcb_io->tiempo_io);
    log_info(kernel_logger, "Se envió el proceso %d al dispositivo IO (%s)", pcb_io->pcb->PID, nombre_io);
}

void atender_fin_io(int socket) {
    // Usar string_itoa y liberar memoria
    char* socket_str = string_itoa(socket);
    t_dispositivo_io* dispositivo = dictionary_get(dispositivos, socket_str);
    free(socket_str);
    
    if (dispositivo == NULL || dispositivo->pcb_io == NULL)
        return;

    // Extraigo su proceso que estaba ejecutando
    t_pcb_io* pcb_io = dispositivo->pcb_io;
    dispositivo->pcb_io = NULL;

    // Lo mando a Ready
    queue_push(cola_ready, pcb_io->pcb);
    log_info(kernel_logger, "## (%d) finalizó IO y pasa a READY", pcb_io->pcb->PID);
    
    // Libero la estructura pcb_io ya que no se necesita más
    free(pcb_io);

    // Ejecuto el siguiente en la espera de su IO
    ejecutar_siguiente(dispositivo->nombre_io);
}


void atender_desconexion_io(int socket) {
    char* socket_str = string_itoa(socket);
    t_dispositivo_io* dispositivo = dictionary_get(dispositivos, socket_str);
    
    if (dispositivo == NULL) {
        log_error(kernel_logger, "Dispositivo IO no encontrado");
        free(socket_str);
        return;
 }
    
 t_io* io = dictionary_get(ios, dispositivo->nombre_io);
    
log_error(kernel_logger, "Un dispositivo IO (%s) se desconectó", dispositivo->nombre_io);

    if (dispositivo->pcb_io != NULL) {
       // Si dispositivo estaba ejecutando un proceso => lo mando a EXIT
        dispositivo->pcb_io->pcb->estado_actual = EXIT;
        log_info(kernel_logger, "## (%d) - Finaliza por desconexión de IO (%s)", 
        dispositivo->pcb_io->pcb->PID, dispositivo->nombre_io);
        
        // Liberar recurso
        free(dispositivo->pcb_io);
    }

    //Lo elimino de mi diccionario de dispositivos
dictionary_remove(dispositivos, string_itoa(socket));
    io->conectados--;

    if(io->conectados == 0){
        //Si es la ultimo dispositivo 
        //=> ELimino esa IO para que no se puedan encolar mas procesos en ella
         dictionary_remove_and_destroy(ios, dispositivo->nombre_io, (void*)free_io);
    }
     // Liberar el nombre del dispositivo que se usó en el strdup
    free(dispositivo->nombre_io);
}

//nose si ya esta hecho
// Función para liberar dispositivos IO
void free_dispositivo_io(t_dispositivo_io* dispositivo) {
    if (dispositivo->pcb_io != NULL) {
        free(dispositivo->pcb_io);
    }
    free(dispositivo);
}

// Función para liberar estructuras IO
void free_io(t_io* io) {
    if (io->procesos != NULL) {
        queue_destroy_and_destroy_elements(io->procesos, free);
    }
    free(io);
}

void recibir_io(char* nombre_io, int socket) {
    t_io* io;
    t_dispositivo_io* dispositivo;

    // Creo la nueva IO si es que no estaba
    if (!dictionary_has_key(ios, nombre_io)) {
        io = malloc(sizeof(t_io));
        if (io == NULL) {
            log_error(kernel_logger, "Error al asignar memoria para IO");
            return;
        }
        io->procesos = queue_create();
        io->conectados = 0;
        dictionary_put(ios, nombre_io, io);
    } else {
        io = dictionary_get(ios, nombre_io);
    }

    // Creo el dispositivo y lo añado a mi diccionario
    dispositivo = malloc(sizeof(t_dispositivo_io));
    if (dispositivo == NULL) {
        log_error(kernel_logger, "Error al asignar memoria para dispositivo IO");
        return;
    }
    
    dispositivo->nombre_io = strdup(nombre_io);
    dispositivo->pcb_io = NULL;

    char* socket_str = string_itoa(socket);
    dictionary_put(dispositivos, socket_str, dispositivo);
    free(socket_str);

    io->conectados++;

    log_info(kernel_logger, "Dispositivo I/O conectado: %s", nombre_io);
}

void enviar_peticion_io(int socket, int PID, int tiempo) {
    //Armo la solicitud
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, PID);
    cargar_int_al_buffer(buffer, tiempo);
    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    //La envio
    enviar_paquete(paquete, socket);
}

