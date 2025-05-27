#include "../include/memoria.h"

t_log* memoria_logger = NULL;
t_config* memoria_config = NULL;
int socket_server = -1;
t_dictionary* dispositivos_cpu_conectados;

//Lista de instrucciones
t_list * lista_instrucciones_globales;


void* handle_connection(void* arg) {
    int client_socket = *((int*)arg);
    free(arg);  // Liberar la memoria para el socket

    int operacion = recibir_operacion(client_socket);

    // Si el que se conectó es Kernel --> Conexión efímera
    if (operacion >= K_M_HANDSHAKE && operacion <= K_M_MEMORY_DUMP) { 

        manejar_operacion_kernel(operacion, client_socket);
        close(client_socket);

    // Si el que se conectó es una CPU --> Conexión persistente
    } else if (operacion >= CPU_M_HANDSHAKE && operacion <= CPU_M_ELIMINAR_CACHE_POR_PROCESO) {

        // PENDIENTE Agregar CPU a la lista de CPUs conectados
        
        manejar_operacion_cpu(operacion, client_socket);
        
        while(true) {
            // log_debug(memoria_logger, "Mandó un mensaje una CPU");

            int bytes = recv(client_socket, &operacion, sizeof(op_code_t), 0);
            if (bytes <= 0) {
                log_info(memoria_logger, "CPU desconectada.");
                break;
            }

            manejar_operacion_cpu(operacion, client_socket);
        }
        // close(client_socket);

    } else {
        log_error(memoria_logger, "Código de operación desconocido: %d", operacion);
        log_error(memoria_logger, "CPU desconectada");
        // close(client_socket);
    }

    return NULL;
}


//____________________________para memoria__________________________________//
void serializar_instruccion(t_instruccion *instruccion, t_buffer *buffer)
{    
/////////Quizas podriamos pensar en poner un pc /ṕid...
    cargar_int_al_buffer(buffer, instruccion->operacion); 
    cargar_int_al_buffer(buffer, instruccion->cantidad_parametros); 
    
    char** parametros = malloc(sizeof(char*) * instruccion->cantidad_parametros); // para el caso de INIT_PROC o IO que tienn dos parametros
    for (int i = 0; i < instruccion->cantidad_parametros; i++) {
        cargar_string_al_buffer(buffer, instruccion->parametros[i]);
    }

    free(parametros); 
}


void leer_instrucciones_desde_archivo(char *nombre_archivo, int pid_asociado) {
    char* path = string_new();
    string_append_with_format(&path, "%s%s", path_instrucciones(), nombre_archivo);
    FILE* nuevo_archivo = fopen(path, "r");

    if (nuevo_archivo == NULL) {
        log_error(memoria_logger, "Error al abrir el archivo");
        free(path);
        return;
    }

    t_instrucciones_proceso* instrucciones_pid = malloc(sizeof(t_instrucciones_proceso));
    instrucciones_pid->lista_instrucciones = list_create();
    instrucciones_pid->pid = pid_asociado;

    char *linea = malloc(100);

    while (fgets(linea, 100, nuevo_archivo) != NULL) {
        size_t len = strlen(linea);

        // Saltear líneas vacías o con solo espacios
        if (len == 0 || strspn(linea, " \t\r\n") == len) {
            continue;
        }

        if (linea[len - 1] == '\n') {
            linea[len - 1] = '\0';
        }

        log_debug(memoria_logger, "Línea original: %s", linea);

        char* contenido = strtok(linea, " ");
        if (contenido == NULL) {
            log_error(memoria_logger, "Línea vacía o sin tokens. Se ignora.");
            continue;
        }

        t_instruccion* instruccion = malloc(sizeof(t_instruccion));
        instruccion->operacion = obtener_operacion_cpu(contenido);
        instruccion->cantidad_parametros = get_cant_parametros(instruccion->operacion);

        if (instruccion->cantidad_parametros < 0) {
            log_error(memoria_logger, "Error: operación no reconocida. Se descarta la instrucción.");
            free(instruccion);
            continue;
        }

        instruccion->parametros = malloc(sizeof(char*) * instruccion->cantidad_parametros);

        for (int i = 0; i < instruccion->cantidad_parametros; i++) {
            contenido = strtok(NULL, " ");
            if (contenido == NULL) {
                log_error(memoria_logger, "Error: faltan parámetros para la instrucción. Se descarta.");
                for (int j = 0; j < i; j++) {
                    free(instruccion->parametros[j]);
                }
                free(instruccion->parametros);
                free(instruccion);
                goto continuar;
            }
            instruccion->parametros[i] = strdup(contenido);
        }

        list_add(instrucciones_pid->lista_instrucciones, instruccion);

    continuar:
        continue;
    }

    fclose(nuevo_archivo);
    list_add(lista_instrucciones_globales, instrucciones_pid);
    free(linea);
    free(path);
}

t_operacion obtener_operacion_cpu(const char *operacion) {
    if (strcmp(operacion, "NOOP") == 0) {
        return NOOP;
    }
    if (strcmp(operacion, "READ") == 0) {
        return READ;
    }
    if (strcmp(operacion, "WRITE") == 0) {
        return WRITE;
    }
    if (strcmp(operacion, "GOTO") == 0) {
        return GOTO;
    }
    if (strcmp(operacion, "IO") == 0) {
        return IO;
    }
    if (strcmp(operacion, "INIT_PROC") == 0) {
        return INIT_PROC;
    }
    if (strcmp(operacion, "DUMP_MEMORY") == 0) {
        return DUMP_MEMORY;
    }
    if (strcmp(operacion, "EXIT") == 0) {
        return EXIT;
    }
    return ACAROMPE;
}

int get_cant_parametros(t_operacion identificador)
{
    int cant_parametros = 0;
    switch (identificador)
    {
        case NOOP: case DUMP_MEMORY: case EXIT:
            return 0;
        break;

         case GOTO: 
            return cant_parametros = 1;
        break;
 
        case READ: case WRITE: case IO: case INIT_PROC:
            return cant_parametros = 2;
            break;
        default:
            log_error(memoria_logger, "Error: Operacion no reconocida.");
            return -1;
            break;
    }
}





t_list* obtener_instrucciones_proceso(int pid_enviado)
{
    for(int i = 0; i < list_size(lista_instrucciones_globales); i++)
    {
        t_instrucciones_proceso* instrucciones_proceso = list_get(lista_instrucciones_globales, i);
        if(instrucciones_proceso->pid == pid_enviado)
        {
            return instrucciones_proceso->lista_instrucciones;
        }
    }
    return NULL;
}


// el buffer tiene que tener el PID y el PC
// cambiar nombre por enviar_pid_pc
/*
void enviar_instruccion(int *socket_cpu, t_buffer *buffer) // de MEMORIA a CPU
{   
    
    int pc = extraer_int_del_buffer(buffer);
    int pid = extraer_int_del_buffer(buffer);
    
    t_list* lista_instrucciones = obtener_instrucciones_proceso(pid);
    
    if(lista_instrucciones == NULL)
    {
        log_info(memoria_logger,"Error al obtener la lista de instrucciones o se acabaron las instrucciones");
        return;
    }
    
    if(pc < 0 || pc >= list_size(lista_instrucciones)){
        log_error(memoria_logger, "Error: program_counter fuera de rango.");
        return;
    }
    
    t_instruccion* instruccion_enviar = list_get(lista_instrucciones, pc);
    
    
    // Obtiene la instrucción en la posición pc
    
    t_buffer* buffer_instruccion = crear_buffer();
    serializar_instruccion(instruccion_enviar, buffer_instruccion); //le podriamos pasar el pid y pc para que lo serialice tambiemn
    
    t_paquete *paquete = crear_paquete(M_CPU_RESPUESTA_INSTRUCCION, buffer_instruccion); 
    enviar_paquete(paquete, *socket_cpu);
    free(instruccion_enviar);

}
*/
