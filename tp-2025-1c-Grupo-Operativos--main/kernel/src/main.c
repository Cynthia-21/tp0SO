#include "../include/kernel.h"

//IMPORTANTE: Cynthia soy tizi, volvi a la version original del archivo, el que hay que reparar es el main.c
//del commit "agregue hilos y planificadores" que subiste. 
//Hice esto para que las conexiones puedan funcionar en la rama main, 
//en la feature branch podes poner el main.c de "agregue hilos y planificadores".

int main(int argc, char* argv[]) {
    kernel_logger = log_create("kernel.log", "LOGGER KERNEL", 1, LOG_LEVEL_INFO);
    if(kernel_logger == NULL) {
        perror("Fallo en KERNEL logger: no se pudo crear.");
        exit(EXIT_FAILURE);
    }
    
    kernel_config = config_create("kernel.config");
    if(kernel_config == NULL){
        perror("Fallo en KERNEL config: no se pudo crear.");
        exit(EXIT_FAILURE);
    }
    
    // Para las conexiones
    IP_MEMORIA = config_get_string_value(kernel_config, "IP_MEMORIA");
    PUERTO_MEMORIA = config_get_string_value(kernel_config, "PUERTO_MEMORIA");

    PUERTO_ESCUCHA_DISPATCH = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_DISPATCH");
    PUERTO_ESCUCHA_INTERRUPT = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_INTERRUPT");
    PUERTO_ESCUCHA_IO = config_get_string_value(kernel_config, "PUERTO_ESCUCHA_IO");
    
    // Para otras cositas 
    ALGORITMO_CORTO_PLAZO = config_get_string_value(kernel_config, "ALGORITMO_CORTO_PLAZO");
    ALGORITMO_INGRESO_A_READY = config_get_string_value(kernel_config, "ALGORITMO_INGRESO_A_READY");
    ALFA = config_get_int_value(kernel_config, "ALFA");
    TIEMPO_SUSPENSION = config_get_int_value(kernel_config, "TIEMPO_SUSPENSION");
    LOG_LEVEL = config_get_string_value(kernel_config, "LOG_LEVEL");

    /* CONEXIONES */
    // Iniciar servidor de KERNEL CPU
    socket_kernel_dispatch = iniciar_servidor(PUERTO_ESCUCHA_DISPATCH, kernel_logger, "KERNEL DISPATCH INICIADO COMO SERVIDOR.");
    socket_kernel_interrupt = iniciar_servidor(PUERTO_ESCUCHA_INTERRUPT, kernel_logger, "KERNEL INTERRUPT INICIADO COMO SERVIDOR.");
    socket_kernel = iniciar_servidor(PUERTO_ESCUCHA_IO, kernel_logger, "KERNEL INICIADO esperando a I/O.");
    
    // Conectarse a Memoria
    socket_memoria = crear_conexion(IP_MEMORIA, PUERTO_MEMORIA);

    /* INICIAR HILOS */
    pthread_t hilo_dispatch, hilo_interrupt, hilo_io, hilo_memoria;

    pthread_create(&hilo_dispatch, NULL, escuchar_cpu_dispatch, NULL);
    pthread_create(&hilo_interrupt, NULL, escuchar_cpu_interrupt, NULL);
    pthread_create(&hilo_io, NULL, escuchar_io_kernel, NULL);
    pthread_create(&hilo_memoria, NULL, (void*) atender_memoria_kernel, NULL);

    /* Esperar que termine el hilo de memoria (o podés esperar a todos si querés) */
    pthread_join(hilo_memoria, NULL);

    /* FINALIZAR */
    liberar_conexion(socket_memoria);
    liberar_conexion(socket_kernel);
    liberar_conexion(socket_kernel_dispatch);
    liberar_conexion(socket_kernel_interrupt);

    config_destroy(kernel_config);
    log_destroy(kernel_logger);

    return EXIT_SUCCESS;
}
