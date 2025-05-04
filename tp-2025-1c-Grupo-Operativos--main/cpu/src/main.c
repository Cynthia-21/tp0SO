#include "../include/cpu.h"

/**
* @fn    main
* @brief Punto de entrada del programa. Inicializa logs, configuración y conexiones, y luego cierra los recursos.
* @param argc Cantidad de argumentos pasados al programa.
* @param argv Array de argumentos pasados al programa.
* @return Código de salida del programa.
*/
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Falta el identificador de la CPU.\nUso: %s [identificador]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* cpu_id = argv[1];

    t_log* logger = inicializar_logger(cpu_id);
    inicializar_config();
    
    conexiones(cpu_id, logger);

    cerrar_cpu();
    
    return EXIT_SUCCESS;
}
