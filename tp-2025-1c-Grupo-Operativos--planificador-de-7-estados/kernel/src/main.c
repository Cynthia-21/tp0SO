#include "../include/kernel.h"

int main(int argc, char* argv[]) {

    // ---------------------------------------- INITIATION -----------------------------------------
    
    // Config
    kernel_config = config_create("kernel.config");
    if(kernel_config == NULL){
        perror("Fallo en KERNEL config: no se pudo crear.");
        exit(EXIT_FAILURE);
    }
    
    IP_MEMORIA = config_get_string_value(kernel_config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(kernel_config, "PUERTO_MEMORIA");
    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_INTERRUPT");
    PUERTO_ESCUCHA_IO = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_IO");
    ALGORITMO_CORTO_PLAZO = config_get_string_value(kernel_config, "ALGORITMO_CORTO_PLAZO");
    ALGORITMO_INGRESO_A_READY = config_get_string_value(kernel_config, "ALGORITMO_INGRESO_A_READY");
    ALFA = config_get_int_value(kernel_config, "ALFA");
    TIEMPO_SUSPENSION = config_get_int_value(kernel_config, "TIEMPO_SUSPENSION");
    LOG_LEVEL = config_get_string_value(kernel_config, "LOG_LEVEL");

    // Inicializar estructuras de datos
    cola_new = queue_create();
    cola_ready = queue_create();
    cola_susp_ready = queue_create();
    cola_susp_blocked = queue_create();
    cpus_libres = dictionary_create();
    cpus_ocupadas = dictionary_create();

    
    /* CONEXIONES */
    // Logger    
    kernel_logger = log_create("kernel.log", "LOGGER KERNEL", 1, log_level_from_string(LOG_LEVEL));
    if(kernel_logger == NULL) {
        perror("Fallo en KERNEL logger: no se pudo crear.");
        exit(EXIT_FAILURE);
    }

    // Validar que se haya iniciado Kernel con los parámetros iniciales
    char* nombre_archivo;
    char* size;

    if (argc < 3) {
        fprintf(stderr, "Uso: %s [nombre_archivo] - %s [size]\n", argv[0], argv[1]);
        exit(EXIT_FAILURE);
    } else{
        nombre_archivo = argv[1];
        size = argv[2];
    }

    // ---------------------------------------- CONNECTIONS -----------------------------------------
    
    // Iniciar servidor de KERNEL CPU
    socket_kernel_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, kernel_logger, "KERNEL DISPATCH INICIADO COMO SERVIDOR.");
    socket_kernel_interrupt = iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, kernel_logger, "KERNEL INTERRUPT INICIADO COMO SERVIDOR.");
    
    // Iniciar servidor de KERNEL I/O
    socket_kernel_io = iniciar_servidor(PUERTO_ESCUCHA_IO, kernel_logger, "KERNEL INICIADO esperando a I/O.");
    

    /* INICIAR HILOS */
    pthread_t hilo_dispatch, hilo_interrupt, hilo_io;

    pthread_create(&hilo_dispatch, NULL, escuchar_cpu_dispatch, NULL);

    pthread_create(&hilo_interrupt, NULL, escuchar_cpu_interrupt, NULL);

    pthread_create(&hilo_io, NULL, escuchar_io_kernel, NULL);
    
	// ------------------------- PLANIFICAR PRIMER PROCESO  -------------------------

    init_primer_proceso(nombre_archivo, size);

	// ------------------------- FINALIZAR  -------------------------


    /* Esperar a que terminen los hilos de CPU e IO */
    pthread_detach(hilo_dispatch);
    pthread_detach(hilo_interrupt);
    pthread_join(hilo_io, NULL);

    
    liberar_conexion(socket_memoria);
    liberar_conexion(socket_kernel_io);
    liberar_conexion(socket_kernel_dispatch);
    liberar_conexion(socket_kernel_interrupt);

    config_destroy(kernel_config);
    log_destroy(kernel_logger);

    return EXIT_SUCCESS;
}
