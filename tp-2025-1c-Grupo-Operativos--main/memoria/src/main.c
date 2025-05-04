#include "../include/memoria.h"

void* escuchar_cpu_memoria() {
    dispositivos_cpu_conectados = dictionary_create();

    while (1) {
        int socket_cpu_nuevo = esperar_cliente(socket_memoria, memoria_logger, "Nuevo CPU conectado a MEMORIA");

        if (socket_cpu_nuevo == -1) {
            log_error(memoria_logger, "Fallo al aceptar una conexión CPU.");
            continue;
        }

        int* socket_ptr = malloc(sizeof(int));
        *socket_ptr = socket_cpu_nuevo;

        pthread_t hilo_cpu;
        pthread_create(&hilo_cpu, NULL, atender_cpu_memoria, socket_ptr);
        pthread_detach(hilo_cpu);

    }
    return NULL;
}

int main(int argc, char* argv[]) {

    /* INICIALIZAR MEMORIA */
    //TODO: psarlo a una funcion

    memoria_logger = log_create("memoria.log", "LOGGER MEMORIA", 1, LOG_LEVEL_INFO);
    if(memoria_logger == NULL) {
        perror("Fallo en MEMORIA logger: no se pudo crear. ");
        exit(EXIT_FAILURE);
    }
    
    memoria_config = config_create("memoria.config");
    if(memoria_config == NULL){
        perror("Fallo en MEMORIA config: no se pudo crear. ");
        exit(EXIT_FAILURE);
    }
      
    // Para las conexiones // 
    PUERTO_ESCUCHA = config_get_string_value(memoria_config, "PUERTO_ESCUCHA");

    // Para otras cositas 
    TAM_MEMORIA = config_get_int_value(memoria_config, "TAM_MEMORIA"); 
    TAM_PAGINA = config_get_int_value(memoria_config, "TAM_PAGINA"); 
    ENTRADAS_POR_TABLA = config_get_int_value(memoria_config, "ENTRADAS_POR_TABLA"); 
    CANTIDAD_NIVELES = config_get_int_value(memoria_config, "CANTIDAD_NIVELES"); 
    RETARDO_MEMORIA = config_get_int_value(memoria_config, "RETARDO_MEMORIA"); 
    PATH_SWAPFILE = config_get_string_value(memoria_config, "PATH_SWAPFILE");
    RETARDO_SWAP = config_get_int_value(memoria_config, "RETARDO_SWAP"); 
    LOG_LEVEL = config_get_string_value(memoria_config, "LOG_LEVEL");
    DUMP_PATH = config_get_string_value(memoria_config, "DUMP_PATH");
    PATH_INSTRUCCIONES = config_get_string_value(memoria_config, "PATH_INSTRUCCIONES");

    // Iniciar servidor de memoria
    socket_memoria = iniciar_servidor(PUERTO_ESCUCHA, memoria_logger, "MEMORIA INICIADA COMO SERVIDOR");

    // Esperar conexion de kernel
    socket_kernel = esperar_cliente(socket_memoria, memoria_logger, "Kernel conectado a MEMORIA");

    //Atender los mensajes de CPU
    pthread_t hilo_cpu;
    pthread_create(&hilo_cpu, NULL, (void*) escuchar_cpu_memoria,NULL);

    //Atender los mensajes de kernel
    pthread_t hilo_kernel;
    pthread_create(&hilo_kernel, NULL, (void*) atender_kernel_memoria,NULL);
    pthread_join(hilo_kernel, NULL);
    
    //Liberar memoria
    liberar_conexion(socket_memoria);
    liberar_conexion(socket_kernel);
    liberar_conexion(socket_cpu);
    config_destroy(memoria_config);
    log_destroy(memoria_logger);

    return EXIT_SUCCESS;
}

