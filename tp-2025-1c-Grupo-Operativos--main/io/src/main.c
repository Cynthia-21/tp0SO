#include "../include/io.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Uso: %s [nombre_interfaz]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* nombre_dispositivo = argv[1];


    io_logger = log_create("io.log", "LOGGER I/O", 1, LOG_LEVEL_INFO);
    if(io_logger == NULL) {
        perror("Fallo en I/O logger: no se pudo crear.");
        exit(EXIT_FAILURE);
    }

    io_config = config_create("io.config");
    if(io_config == NULL){
        perror("Fallo en I/O config: no se pudo crear.");
        exit(EXIT_FAILURE);
    }

    // Leer IP y PUERTO del kernel
    IP_KERNEL = config_get_string_value(io_config, "IP_KERNEL");
    PUERTO_KERNEL = config_get_string_value(io_config, "PUERTO_KERNEL");
    LOG_LEVEL = config_get_string_value(io_config, "LOG_LEVEL");
    

    // Conectarse al Kernel
    socket_kernel = crear_conexion(IP_KERNEL, PUERTO_KERNEL);
    log_info(io_logger, "Conectado a KERNEL");

    // Enviar handshake inicial con el nombre identificador
    log_info(io_logger, "Nombre de dispositivo a enviar: %s", nombre_dispositivo);

    t_buffer* buffer = crear_buffer();
    cargar_string_al_buffer(buffer, nombre_dispositivo); // nombre_dispositivo es un string

    t_paquete* paquete = crear_paquete(HANDSHAKE, buffer);
    enviar_paquete(paquete, socket_kernel);

    eliminar_paquete(paquete); // libera buffer internamente

    // Esperar a que llegue un proceso de kernel
    atender_kernel_io(nombre_dispositivo);

    // Liberar recursos
    liberar_conexion(socket_kernel);
    config_destroy(io_config);
    log_destroy(io_logger);

    return 0;
}
