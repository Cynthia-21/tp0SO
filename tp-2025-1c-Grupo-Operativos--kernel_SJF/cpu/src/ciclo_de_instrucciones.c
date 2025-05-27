#include "../include/cpu.h"

int cant_interrupciones;

/**
* @fn    fetch
* @brief Solicita la instrucción correspondiente a un PID y un Program Counter (PC) a memoria.
* @param pid Identificador del proceso.
* @param pc Program Counter del proceso.
* @return Puntero a la instrucción obtenida.
*/
void fetch(t_log* cpu_logger) {

    log_info(cpu_logger, "## PID: %d - FETCH - Program Counter: %d", pid, pc);
    
    /*  Le mando el PID y PC a memoria para que me devuelva la instruccion*/
    t_buffer* buffer = crear_buffer();
    cargar_int_al_buffer(buffer, pc);
    cargar_int_al_buffer(buffer, pid);
    
    t_paquete* paquete = crear_paquete(CPU_M_SOLICITAR_INSTRUCCION, buffer);
    enviar_paquete(paquete, socket_memoria);

    int cod_op = recibir_operacion(socket_memoria);

    if (cod_op == M_CPU_RESPUESTA_INSTRUCCION) {
		
        t_buffer* buffer_respuesta = recibir_buffer(socket_memoria); //Instruccion recibida
        
        /*   ETAPA DECODE   */
        t_instruccion* instruccion = decode(buffer_respuesta);  
        t_operacion operacion = instruccion -> operacion;
        
        eliminar_buffer(buffer_respuesta);
        
        if (operacion == READ || operacion == WRITE) { //necesito traduccion de memoria (es READ o WRITE)
            execute (instruccion, cpu_logger);
            check_interrupt(instruccion, cpu_logger);
            destruir_instruccion(instruccion);
            fetch(cpu_logger);
        }

        else if (!es_syscall(instruccion)) { // es GOTO o NOOP
            execute (instruccion, cpu_logger);
            check_interrupt(instruccion, cpu_logger);
            destruir_instruccion(instruccion);
            fetch(cpu_logger);
        }

        else { // es una SYSCALL
            int fin_del_archivo = enviar_instruccion_a_kernel(instruccion, cpu_logger);
            check_interrupt(instruccion, cpu_logger);
            destruir_instruccion(instruccion);
            if(!fin_del_archivo) { //si la instruccion envaida no es EXIT
                fetch(cpu_logger);
            }
            else {
                log_debug(cpu_logger, "Instruccion recibida EXIT, no quedan mas instrucciones en el archivo\n");
            }
        }

    }
}


bool es_syscall(t_instruccion* instruccion) {
    t_operacion operacion = instruccion->operacion;
    return (operacion == IO || operacion == EXIT || operacion == INIT_PROC || operacion == DUMP_MEMORY);
}


void execute (t_instruccion* instruccion, t_log* cpu_logger){

    switch (instruccion->operacion) {
        case NOOP:  //solo consume el tiempo del ciclo de instruccion
            log_info(cpu_logger, "## PID: %d - Ejecutando: NOOP", pid);
        break;

        case READ:
        {
            char* direccion = instruccion->parametros[0];
            int tamanio = atoi(instruccion->parametros[1]); //atoi convierte un string a int

            // Lectura/Escritura Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>”.
            log_info(cpu_logger, "## PID: %d - Ejecutando: READ - %s - %d", pid, direccion, tamanio);
            // UPDATE 15-05
            // Envio a Memoria lo que necesito leer
            
            t_buffer* buffer_rta = crear_buffer();
            cargar_string_al_buffer(buffer_rta, direccion); // Agrego dirección a leer al buffer
            cargar_string_al_buffer(buffer_rta, instruccion->parametros[1]); // Agrego tamaño a leer al buffer
            t_paquete* paquete = crear_paquete(CPU_M_LEER_MEMORIA, buffer_rta);
            enviar_paquete(paquete, socket_memoria);
 
            // Recibo respuesta de Memoria
            if(recibir_operacion(socket_memoria) == M_CPU_VALOR_LEIDO){

                t_buffer* buffer = recibir_buffer(socket_memoria);
                char* valor_leido = extraer_string_del_buffer(buffer);    
                eliminar_buffer(buffer);

            } else {
                log_debug(cpu_logger, "Memoria me contestó otra cosa");
            }
    
        }
        break;

        case WRITE:
            {
            // Lectura/Escritura Memoria: “PID: <PID> - Acción: <LEER / ESCRIBIR> - Dirección Física: <DIRECCION_FISICA> - Valor: <VALOR LEIDO / ESCRITO>”.

            char* direccion = instruccion->parametros[0];
            char* datos = instruccion->parametros[1];

            log_info(cpu_logger, "## PID: %d - Ejecutando: WRITE - %s - %s", pid, direccion, datos);

            // UPDATE 15-05
            // Envio a Memoria lo que necesito escribir
            t_buffer* buffer_rta = crear_buffer();
            cargar_string_al_buffer(buffer_rta, direccion); // Agrego dirección a escribir al buffer
            cargar_string_al_buffer(buffer_rta, datos); // Agrego datos a escribir al buffer
            t_paquete* paquete = crear_paquete(CPU_M_ESCRIBIR_MEMORIA, buffer_rta);
            enviar_paquete(paquete, socket_memoria);
            
            // Recibo respuesta de Memoria
            if(recibir_operacion(socket_memoria) == M_CPU_CONFIRMACION_ESCRITURA){

                t_buffer* buffer = recibir_buffer(socket_memoria);
                char* valor_leido = extraer_string_del_buffer(buffer);    
                eliminar_buffer(buffer);

            } else {
                log_debug(cpu_logger, "Memoria me contestó otra cosa");
            }
            
        }

        break;

        case GOTO:{
            int valor = atoi(instruccion->parametros[0]);
            pc = valor; //se actualiza el pc por direccion de memoria
            
            log_info(cpu_logger, "## PID: %d - Ejecutando: GOTO - %d", pid, valor);
            }
        break;

        default:
        break;
    }

}

void check_interrupt(t_instruccion* instruccion, t_log* cpu_logger){ //TODO: evaluar si el incremento de pc se puede hacer por fuera, asi me evito pasarle la instruccion por parametro
    if (hay_alguna_interrupcion()){
        log_info(cpu_logger, "## LLega interrupcion al puerto interrupt");
        //mandar pid y pc actualizado
        t_buffer* buffer_interrupt = crear_buffer();
        cargar_int_al_buffer(buffer_interrupt, pc);
        cargar_int_al_buffer(buffer_interrupt, pid);

        t_paquete* paquete = crear_paquete(K_CPU_INTERRUPT_PROCESO, buffer_interrupt);

        enviar_paquete(paquete, socket_kernel_interrupt);
    }

    if (instruccion->operacion != GOTO){
        pc++;
    }

}


bool hay_alguna_interrupcion(){
    return interrupt;
}





/**
* @fn    deserializar_instruccion_solicitada
* @brief Deserializa una instrucción recibida en un buffer.
* @param buffer Puntero al buffer que contiene la instrucción serializada.
* @return Puntero a la instrucción deserializada.
*//*
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
*/
t_instruccion* decode(t_buffer* buffer) {
    t_instruccion* instruccion_deserializada = malloc(sizeof(t_instruccion));

    instruccion_deserializada->operacion = extraer_int_del_buffer(buffer);
    instruccion_deserializada->cantidad_parametros = extraer_int_del_buffer(buffer);

    instruccion_deserializada->parametros = malloc(instruccion_deserializada->cantidad_parametros * sizeof(char*));
    for (int i = 0; i < instruccion_deserializada->cantidad_parametros; i++) {
        instruccion_deserializada->parametros[i] = extraer_string_del_buffer(buffer);
    }

    return instruccion_deserializada;
}

int enviar_instruccion_a_kernel(t_instruccion* instruccion, t_log* cpu_logger) {

    t_buffer* buffer = crear_buffer();
    
    cargar_int_al_buffer(buffer, pid);

    switch (instruccion -> operacion){
        case INIT_PROC:
            // cargar_int_al_buffer(buffer, instruccion -> operacion);
            cargar_string_al_buffer(buffer, instruccion -> parametros[0]); //archivo de instrucciones
            cargar_string_al_buffer(buffer, instruccion -> parametros[1]); //tamaño

            t_paquete* paquete_init_proc = crear_paquete(CPU_K_INIT_PROC, buffer);
            enviar_paquete(paquete_init_proc, socket_kernel_dispatch);
            return 0;
            
        break;
            
        case DUMP_MEMORY:
            // cargar_int_al_buffer(buffer, instruccion -> operacion);
            
            t_paquete* paquete_dump_memory = crear_paquete(CPU_K_DUMP_MEMORY, buffer);
            enviar_paquete(paquete_dump_memory, socket_kernel_dispatch);
            return 0;
        break;
            
        case IO:
            // cargar_int_al_buffer(buffer, instruccion -> operacion);
            cargar_string_al_buffer(buffer, instruccion -> parametros[0]); //Dispositivo
            cargar_string_al_buffer(buffer, instruccion -> parametros[1]); //Tiempo
            cargar_int_al_buffer(buffer, pc+1); //Para salvar contexto
           
            t_paquete* paquete_io = crear_paquete(CPU_K_SOLICITAR_IO, buffer);
            enviar_paquete(paquete_io, socket_kernel_dispatch);
            
            // Dejar de ejecutar ese proceso y empezar a ejecutar otro
            atender_kernel_cpu_dispatch(cpu_logger);
            return 0;
        break;
            
        case EXIT:
            // cargar_int_al_buffer(buffer, instruccion -> operacion);    
            t_paquete* paquete_exit = crear_paquete(CPU_K_EXIT, buffer);
            enviar_paquete(paquete_exit, socket_kernel_dispatch);
            return 1;
        break;

        default:
            eliminar_buffer(buffer);
            return (-1);
    }
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