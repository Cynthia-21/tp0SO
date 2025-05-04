#include "../include/cpu.h"

int cant_interrupciones;
/* void ciclo_de_instrucciones(void* algo){
    t_instruccion *instruccion_a_ejecutar;
    cant_interrupciones = 0;
    // pthread_t hilo_interrupciones;
    // pthread_create(&hilo_interrupciones, NULL, (void*)check_interrupt, ctx);//hilo que recibe interrupciones del KERNEL

    while(pc != NULL && cant_interrupciones == 0){
        instruccion_a_ejecutar = fetch(pid, pc); //de donde saco el pid y pc aca?
        //LIBERAR LA MEMORIA DE instruccion_a_ejecutar!!!!

        if(!decode(instruccion_a_ejecutar)) { //si no
            pc++;
        }

        t_operacion operacion = execute(instruccion_a_ejecutar, pc);

        switch(operacion) {
            case NOOP:

                break;

            case READ:

                break;
            
            case WRITE:

                break;

            case GOTO:

                break;
        }



    }

    // if(hayInterrupcion){
    //     sem_post(&recibir_instruccion);
    //     pthread_join(hilo_interrupciones, NULL);
    //  }
    //  else{
    //      pthread_cancel(hilo_interrupciones);
    //  }
            
    
    // free(instruccion_a_ejecutar);

}
*/



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

bool decode(t_instruccion* instruccion) {

    t_operacion operacion = instruccion->operacion;
    return (operacion == NOOP || operacion == READ || operacion == WRITE || operacion == GOTO);
}

// typedef struct{    
// 	t_operacion operacion; //enum: NOOP, READ, WRITE, GOTO
//     int cantidad_parametros;
//     char **parametros;
// } t_instruccion;

int execute (int pid, int* pc, t_instruccion* instruccion, t_log* cpu_logger){

    switch (instruccion->operacion) {
        case NOOP:  //solo consume el tiempo del ciclo de instruccion
            log_info(cpu_logger, "## PID: %d - Ejecutando: NOOP", pid);
            return NOOP;
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
            return READ;
        break;

        case WRITE:{
            // Lectura/Escritura Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>”.

            char* direccion = instruccion->parametros[0];
            char* datos = instruccion->parametros[1];
            strcpy(direccion, datos);
            
            log_info(cpu_logger, "## PID: %d - Ejecutando: WRITE - %s - %s", pid, direccion, datos);

            }
            return WRITE;
        break;

        case GOTO:{
            int valor = atoi(instruccion->parametros[0]);
            *pc = valor; //se actualiza el pc por direccion de memoria
            
            log_info(cpu_logger, "## PID: %d - Ejecutando: GOTO - %d", pid, valor);
            }
            return GOTO;
        break;

        default:
            return EXIT_FAILURE;
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
    cargar_int_al_buffer(buffer, pid);
    cargar_int_al_buffer(buffer, pc);

    t_paquete* paquete = crear_paquete(PAQUETE, buffer);

    enviar_paquete(paquete, socket_memoria);

    eliminar_paquete(paquete);
    eliminar_buffer(buffer);

    // Recibo la instrucción de memoria
    t_buffer* buffer2 = crear_buffer(); // TODO: ver si ponemos alguna parte en otra función así no tenemos buffer1 y buffer2
    buffer2->stream = recibir_buffer(socket_memoria);

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

