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
    
    atender_kernel(cpu_logger);
    atender_memoria(cpu_logger);
}

/**
* @fn    conectar_kernel
* @brief Establece las conexiones con los sockets de kernel (dispatch e interrupt) y realiza el handshake.
* @param cpu_id Identificador de la CPU que se enviará en el handshake.
*/
void conectar_kernel(char* cpu_id, t_log* cpu_logger) {
    //DISPATCH
    printf("Conectando a kernel dispatch...\n");
    printf("IP KERNEL: %s\n", ip_kernel());
    printf("PUERTO KERNEL DISPATCH: %s\n", puerto_kernel_dispatch());
    socket_kernel_dispatch = crear_conexion(ip_kernel(), puerto_kernel_dispatch());
    printf("socket kernel dispatch: %d\n", socket_kernel_dispatch);
    if(socket_kernel_dispatch == -1) {
        log_error(cpu_logger, "ERROR al conectarse con kernel dispatch");
        exit(-1);
    }
    else { //HANDSHAKE
        
        log_info(cpu_logger, "Conectado a KERNEL DISPATCH");

        t_buffer* b_handshake_envio = crear_buffer();
        cargar_int_al_buffer(b_handshake_envio, HAND_CPU_KERNEL_DIS);
        cargar_string_al_buffer(b_handshake_envio, cpu_id);
        t_paquete* paquete_disp = crear_paquete(HANDSHAKE, b_handshake_envio);
        enviar_paquete(paquete_disp, socket_kernel_dispatch);
        log_info(cpu_logger, "Enviando HANDSHAKE a KERNEL DISPATCH");
        // eliminar_buffer(b_handshake_envio); //Segmentation fault (core dumped)
        
        //espero la respuesta del handshake en atender_kernel_cpu_dispatch()        
    }


    //INTERRUPT
    socket_kernel_interrupt = crear_conexion(ip_kernel(), puerto_kernel_interrupt());
    if(socket_kernel_interrupt == -1) {
        log_error(cpu_logger, "ERROR al conectarse con kernel interrupt");
        exit(-1);
    }
    else { //HANDSHAKE
        log_info(cpu_logger, "Conectado a KERNEL INTERRUPT");
        t_buffer* b_handshake_envio_int = crear_buffer();
        cargar_int_al_buffer(b_handshake_envio_int, HAND_CPU_KERNEL_INT);
        cargar_string_al_buffer(b_handshake_envio_int, cpu_id);

        t_paquete* paquete = crear_paquete(HANDSHAKE, b_handshake_envio_int);
        enviar_paquete(paquete, socket_kernel_interrupt);
    }
}

/**
* @fn    conectar_memoria
* @brief Establece la conexión con el socket de memoria.
*/
void conectar_memoria(t_log* cpu_logger) {
    printf("Conectando a memoria...\n");
    printf("IP MEMORIA: %s\n", ip_memoria());
    printf("PUERTO MEMORIA: %s\n", puerto_memoria());
    socket_memoria = crear_conexion(ip_memoria(), puerto_memoria());
    printf("socket memoria: %d\n", socket_memoria);
    if(socket_memoria == -1) {
        log_error(cpu_logger, "ERROR al conectarse con memoria");
        exit(-1);
    }
    else log_info(cpu_logger, "Conectado a MEMORIA");
    printf("socket memoria: %d\n", socket_memoria);
}

/**
* @fn    atender_kernel
* @brief Crea hilos para atender los mensajes de kernel dispatch e interrupt.
*/
void atender_kernel(t_log* cpu_logger) { //Atiendo los mensajes de KERNEL DISPATCH (como cliente)
    pthread_t hilo_cpu_interrupt;
    pthread_t hilo_cpu_dispatch;

    if(pthread_create(&hilo_cpu_dispatch, NULL, (void*) atender_kernel_cpu_dispatch, cpu_logger) != 0){
        log_error(cpu_logger, "ERROR al crear el hilo con para atender el kernel dispatch");
    }
    
    if(pthread_create(&hilo_cpu_interrupt, NULL, (void*) atender_kernel_cpu_interrupt, cpu_logger) != 0){
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
