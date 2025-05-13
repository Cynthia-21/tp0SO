#include "../include/cpu.h"

int cant_interrupciones;
void comenzar_ciclo_instruccion(int pid, int pc, t_log* cpu_logger){
    
    t_instruccion* instruccion_a_ejecutar = fetch(pid, pc, cpu_logger);

    if (decode(instruccion_a_ejecutar)) { //necesitoo traduccion de memoria (es READ o WRITE)
        execute (pid, &pc, instruccion_a_ejecutar, cpu_logger);


        destruir_instruccion(instruccion_a_ejecutar);
    }
    else if (!es_syscall(instruccion_a_ejecutar)) { // es GOTO o NOOP
        execute (pid, &pc, instruccion_a_ejecutar, cpu_logger);


        destruir_instruccion(instruccion_a_ejecutar);
    }
    else {
        enviar_instruccion_a_kernel(pid, instruccion_a_ejecutar);


        destruir_instruccion(instruccion_a_ejecutar);
    }

    // TODO check interrupt

}


/**
* @fn    fetch
* @brief Solicita la instrucción correspondiente a un PID y un Program Counter (PC) a memoria.
* @param pid Identificador del proceso.
* @param pc Program Counter del proceso.
* @return Puntero a la instrucción obtenida.
*/
t_instruccion* fetch(int pid, int pc, t_log* cpu_logger) {
    char* mensaje = string_from_format("## PID: %d - FETCH - Program Counter: %d", pid, pc);
    log_info(cpu_logger, "%s", mensaje);
    free(mensaje);
    t_instruccion* instruccion = solicitar_instruccion_a_memoria(pc, pid);
    
    return instruccion;
}

bool decode(t_instruccion* instruccion) { //por si o por no. es instruccion que necesita traduccion de memoria? 
    t_operacion operacion = instruccion->operacion;
    return (operacion == READ || operacion == WRITE);
}

bool es_syscall(t_instruccion* instruccion) {
    t_operacion operacion = instruccion->operacion;
    return (operacion == IO || operacion == EXIT || operacion == INIT_PROC || operacion == MEMORY_DUMP);
}


void execute (int pid, int* pc, t_instruccion* instruccion, t_log* cpu_logger){

    switch (instruccion->operacion) {
        case NOOP:  //solo consume el tiempo del ciclo de instruccion
            log_info(cpu_logger, "## PID: %d - Ejecutando: NOOP", pid);
        break;

        case READ:{
            // Lectura/Escritura Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>”.

            char* direccion = instruccion->parametros[0];
            int tamanio = atoi(instruccion->parametros[1]); //atoi convierte un string a int
            printf("Contenido leído: ");
            for (int i = 0; i < tamanio; i++) {
                putchar(direccion[i]); //imprime caracter a caracter
            }

            log_info(cpu_logger, "## PID: %d - Ejecutando: READ - %s - %d", pid, direccion, tamanio);
            }
        break;

        case WRITE:{
            // Lectura/Escritura Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>”.

            char* direccion = instruccion->parametros[0];
            char* datos = instruccion->parametros[1];
            strcpy(direccion, datos);
            
            log_info(cpu_logger, "## PID: %d - Ejecutando: WRITE - %s - %s", pid, direccion, datos);

            }
        break;

        case GOTO:{
            int valor = atoi(instruccion->parametros[0]);
            *pc = valor; //se actualiza el pc por direccion de memoria
            
            log_info(cpu_logger, "## PID: %d - Ejecutando: GOTO - %d", pid, valor);
            }
        break;

        default:
        break;
    }
}

/**
* @fn    solicitar_instruccion_a_memoria
* @brief Envía un PID y un PC a memoria para solicitar la instrucción correspondiente.
* @param pc Program Counter del proceso.
* @param pid Identificador del proceso.
* @return Puntero a la instrucción obtenida.
*/
t_instruccion* solicitar_instruccion_a_memoria(int pc, int pid) {
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pc);
    cargar_int_al_buffer(buffer, pid);

    t_paquete* paquete = crear_paquete(PEDIR_INSTRUCCION, buffer);

    enviar_paquete(paquete, socket_memoria);

    // Recibo la instrucción de memoria
    // t_buffer* buffer2 = crear_buffer(); 
    int codigo;
    recv(socket_memoria, &codigo, sizeof(int), MSG_WAITALL);
    if(codigo != ENVIAR_INSTRUCCION) {
        log_error(cpu_logger, "Error al recibir la instrucción de memoria");
    }
    // buffer2->stream = recibir_buffer(socket_memoria); // creo que es mejor de la forma de abajo
    t_buffer* buffer2 = recibir_buffer(socket_memoria);
                                                                               // OJO el buffer solo tiene la instruccion el el PID y PC
    t_instruccion* instruccion = deserializar_instruccion_solicitada(buffer2); // buffer: PID | PC | WRITE 0 EJEMPLO_DE_ENUNCIADO
    
    eliminar_buffer(buffer2);
 
    return instruccion;
}




/**
* @fn    deserializar_instruccion_solicitada
* @brief Deserializa una instrucción recibida en un buffer.
* @param buffer Puntero al buffer que contiene la instrucción serializada.
* @return Puntero a la instrucción deserializada.
*/
t_instruccion* deserializar_instruccion_solicitada(t_buffer* buffer) {
    int desplazamiento = 0;

    t_instruccion* instruccion_deserializada = malloc(sizeof(t_instruccion));

    memcpy(&(instruccion_deserializada->operacion), buffer->stream + desplazamiento, sizeof(t_operacion));
    desplazamiento += sizeof(t_operacion);

    memcpy(&(instruccion_deserializada->cantidad_parametros), buffer->stream + desplazamiento, sizeof(int));
    desplazamiento += sizeof(int);

    instruccion_deserializada->parametros = malloc(instruccion_deserializada->cantidad_parametros * sizeof(char*));
    for (int i = 0; i < instruccion_deserializada->cantidad_parametros; i++) {
        int len;
        memcpy(&len, buffer->stream + desplazamiento, sizeof(int)); // El largo del siguiente string
        desplazamiento += sizeof(int);
        
        instruccion_deserializada->parametros[i] = malloc(len); // Pido memoria para ese espacio
        memcpy(instruccion_deserializada->parametros[i], buffer->stream + desplazamiento, len); // Copio el parámetro
        desplazamiento += len;
    }

    return instruccion_deserializada;
}



void enviar_instruccion_a_kernel(int pid, t_instruccion* instruccion) {
    t_buffer* buffer = crear_buffer();
    t_operacion operacion = instruccion->operacion;

    cargar_int_al_buffer(buffer, pid);
    cargar_int_al_buffer(buffer, operacion);
    cargar_int_al_buffer(buffer, instruccion->cantidad_parametros);

    for (int i = 0; i < instruccion->cantidad_parametros; i++) {
        cargar_string_al_buffer(buffer, instruccion->parametros[i]);
    }

    t_paquete* paquete = crear_paquete(SYSCALL_P, buffer);
    enviar_paquete(paquete, socket_kernel_dispatch);

    eliminar_buffer(buffer);
}

//falta para cuando tienen mas parametros, tipo 2
// void enviar_instruccion_a_kernel(t_instruccion * instruccion) {
//     t_buffer* buffer = crear_buffer();
//     t_operacion operacion = instruccion->operacion;
//     char* parametro = instruccion->parametros[0]; 
//     // Enviar la instrucción a kernel
//     cargar_int_al_buffer(buffer, operacion);
//     cargar_string_al_buffer(buffer, parametro);

//     t_paquete* paquete = crear_paquete(PAQUETE, buffer);
//     enviar_paquete(paquete, socket_kernel_dispatch);

//     eliminar_paquete(paquete);
//     eliminar_buffer(buffer);
// }