#include "../include/cpu.h"

// Para las conexiones
char* puerto_memoria() {
    return config_get_string_value(cpu_config, "PUERTO_MEMORIA");
}
char* ip_memoria() {
    return config_get_string_value(cpu_config, "IP_MEMORIA");
}
char* puerto_kernel_dispatch() {
    return config_get_string_value(cpu_config, "PUERTO_KERNEL_DISPATCH");
}
char* puerto_kernel_interrupt() {
    return config_get_string_value(cpu_config, "PUERTO_KERNEL_INTERRUPT");
}   
char* ip_kernel() {
    return config_get_string_value(cpu_config, "IP_KERNEL");
}

// Para otras cositas 
char* entradas_tlb() {
    return config_get_string_value(cpu_config, "ENTRADAS_TLB");
}
char* reemplazo_tlb() {
    return config_get_string_value(cpu_config, "REEMPLAZO_TLB");
}
char* entradas_cache() {
    return config_get_string_value(cpu_config, "ENTRADAS_CACHE");
}
char* reemplazo_cache() {
    return config_get_string_value(cpu_config, "REEMPLAZO_CACHE");
}
char* retardo_cache() {
    return config_get_string_value(cpu_config, "RETARDO_CACHE");
}
char* log_level() {
    return config_get_string_value(cpu_config, "LOG_LEVEL");
}

/*
void inicializar_logs() {

    cpu_logger = log_create("cpu.log", "LOGGER CPU", 1, LOG_LEVEL_INFO);
    if(cpu_logger == NULL) {
        perror("Fallo en CPU logger: no se pudo crear. ");
        exit(EXIT_FAILURE);
    }
}*/

t_log* inicializar_logger(char *nombre)
{
	t_log* nuevo_logger;

    // Hace el nombre de la cpu con el .log
    size_t longitud = strlen(nombre) + strlen(".log") + 1;
    char* nombre_con_extension = malloc(longitud);
    strcpy(nombre_con_extension, nombre);       // Copiás el nombre base
    strcat(nombre_con_extension, ".log");       // Le agregás la extensión

	nuevo_logger = log_create(nombre_con_extension, "LOGGER CPU", 1, LOG_LEVEL_INFO);
	if(nuevo_logger == NULL)
	{
		printf("Fallo en CPU logger: no se pudo crear.");
		exit(EXIT_FAILURE);
	}

	return nuevo_logger;
}

void inicializar_config() {
    cpu_config = config_create("cpu.config");
    if(cpu_config == NULL){
        perror("Fallo en CPU config: no se pudo crear. ");
        exit(EXIT_FAILURE);
    }
}

