#include "../include/cpu.h"

/**
* @fn    conexiones
* @brief Establece las conexiones con memoria y kernel, y comienza a atender sus mensajes.
* @param cpu_id Identificador de la CPU que se enviará en el handshake con el kernel.
*/
void conexiones(char* cpu_id, t_log* cpu_logger) {
//No estoy seguro si memomira lleva cpu_id  
    conectar_memoria(cpu_logger);
    conectar_kernel(cpu_id, cpu_logger);

    atender_memoria(cpu_logger);
    atender_kernel(cpu_logger);
}
/**
* @fn    conectar_kernel
* @brief Establece las conexiones con los sockets de kernel (dispatch e interrupt) y realiza el handshake.
* @param cpu_id Identificador de la CPU que se enviará en el handshake.
*/
void conectar_kernel(char* cpu_id,t_log* cpu_logger) {
    socket_kernel_dispatch = crear_conexion(ip_kernel(), puerto_kernel_dispatch());
    if(socket_kernel_dispatch == -1) {
        log_error(cpu_logger, "ERROR al conectarse con kernel dispatch");
        exit(-1);
    }
    else { //HANDSHAKE
        log_info(cpu_logger, "Conectado a KERNEL DISPATCH");
        log_info(cpu_logger, "HANDSHAKE - CPU ID a enviar: %s", cpu_id);

        t_buffer* buffer = crear_buffer();
        cargar_string_al_buffer(buffer, cpu_id);

        t_paquete* paquete = crear_paquete(HANDSHAKE, buffer);
        enviar_paquete(paquete, socket_kernel_dispatch);

        eliminar_paquete(paquete);
    }
    
    socket_kernel_interrupt = crear_conexion(ip_kernel(), puerto_kernel_interrupt());
    if(socket_kernel_interrupt == -1) {
        log_error(cpu_logger, "ERROR al conectarse con kernel interrupt");
        exit(-1);
    }
    else { //HANDSHAKE
        log_info(cpu_logger, "Conectado a KERNEL INTERRUPT");
        log_info(cpu_logger, "HANDSHAKE - CPU ID a enviar: %s", cpu_id);

        t_buffer* buffer = crear_buffer();
        cargar_string_al_buffer(buffer, cpu_id);

        t_paquete* paquete = crear_paquete(PAQUETE, buffer);
        enviar_paquete(paquete, socket_kernel_interrupt);

        eliminar_paquete(paquete);
    }
}

/**
* @fn    conectar_memoria
* @brief Establece la conexión con el socket de memoria.
*/
void conectar_memoria(t_log* cpu_logger) {
    socket_memoria = crear_conexion(ip_memoria(), puerto_memoria());
    if(socket_memoria == -1) {
        log_error(cpu_logger, "ERROR al conectarse con memoria");
        exit(-1);
    }

    else log_info(cpu_logger, "Conectado a MEMORIA");
}

/**
* @fn    atender_kernel
* @brief Crea hilos para atender los mensajes de kernel dispatch e interrupt.
*/
void atender_kernel(t_log* cpu_logger) { //Atiendo los mensajes de KERNEL DISPATCH (como cliente)
    pthread_t hilo_cpu_interrupt;
    pthread_t hilo_cpu_dispatch;

    if(pthread_create(&hilo_cpu_dispatch, NULL, (void*) atender_kernel_cpu_dispatch,NULL) != 0){
        log_error(cpu_logger, "ERROR al crear el hilo con para atender el kernel dispatch");
    }

    if(pthread_create(&hilo_cpu_interrupt, NULL, (void*) atender_kernel_cpu_interrupt,NULL) != 0){
        log_error(cpu_logger, "ERROR al crear el hilo con para atender el kernel interrupt");
    }

    pthread_detach(hilo_cpu_dispatch);
    pthread_detach(hilo_cpu_interrupt);
}

/**
* @fn    atender_memoria
* @brief Crea un hilo para atender los mensajes de memoria.
*/
void atender_memoria(t_log* cpu_logger) { //Atiendo los mensajes de memoria (como cliente)
    pthread_t hilo_memoria;

    if(pthread_create(&hilo_memoria, NULL, (void*) atender_memoria_cpu,NULL) != 0){
        log_error(cpu_logger, "ERROR al crear el hilo con la memoria");
    }

    pthread_join(hilo_memoria, NULL);
}

/**
* @fn    esperar_hilos
* @brief Espera la finalización de los hilos creados (actualmente no implementado).
*/
void esperar_hilos(){
    // pthread_join(hilo_memoria, NULL);
}

/**
* @fn    cerrar_cpu
* @brief Libera las conexiones, destruye la configuración y los logs, y limpia recursos asociados a la CPU.
*/
void cerrar_cpu(t_log* cpu_logger) {
    //Conexiones
    liberar_conexion(socket_memoria);
    liberar_conexion(socket_kernel_dispatch);
    liberar_conexion(socket_kernel_interrupt);

    //Config
    config_destroy(cpu_config);
    
    //Logs
    log_destroy(cpu_logger);

    //Semaforos ... (proximamente)
}
/*Multiplicidad de CPU
char *ip;
	char *puerto;
	char charAux[50];
    char parametro[20];

	strcpy(charAux, modulo);
    t_log* loggerCPUKERNEL;
	loggerIOKernel= iniciar_logger(strcat(charAux,".log"));
*/