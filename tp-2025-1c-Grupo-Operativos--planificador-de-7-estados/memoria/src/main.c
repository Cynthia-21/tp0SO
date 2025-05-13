#include "../include/memoria.h"

int main(int argc, char* argv[]) {

    // ---------------------------------------- INITIATION  -----------------------------------------

    inicializar_config();
    inicializar_logger();
    t_list *cpu_conectados = list_create();


    // ---------------------------------------- OPEN SERVER  ----------------------------------------

    socket_server = iniciar_servidor(puerto_escucha(), memoria_logger, "MEMORIA INICIADA COMO SERVIDOR");

    // ------------------------------------ ACCEPT CONNECTIONS  -------------------------------------

    int client_socket;
    
    while(true){
        // Aceptar conexión
        client_socket = accept(socket_server, NULL, NULL);

        // Validar conexión
        if(client_socket < 0){
            log_error(memoria_logger, "Error aceptando conexión con cliente nuevo.");
        } else{
            log_debug(memoria_logger, "Nuevo cliente aceptado.");
        }

        // Crear un hilo para gestionar la conexión
        pthread_t thread;
        int* p_socket = malloc(sizeof(int)); // Asignar memoria para el socket

        *p_socket = client_socket;

        if (pthread_create(&thread, NULL, handle_connection, p_socket) != 0) {
            log_error(memoria_logger, "No se pudo crear hilo para nueva conexión.");
            close(client_socket);
            free(p_socket);
        } else {
            pthread_detach(thread); // Liberar recursos automáticamente cuando el hilo termine
        }

    }

    // ---------------------------------------- TERMINATION ----------------------------------------

    liberar_conexion(socket_server);
    config_destroy(memoria_config);
    log_destroy(memoria_logger);

    return EXIT_SUCCESS;
}


//-------------------Cosas Para Agregar-------------------//

//recibir

// el buffer tiene que tener el PID y el PC
void enviar_instruccion(int *socket_cpu, t_buffer *buffer) // de MEMORIA a CPU
{   
    
    int desplazamiento = 0;

    // Obtiene el PC y PID del buffer
    int pc;
    memcpy(&pc, buffer->stream, sizeof(int)); //Deserilizo el PC
    desplazamiento += sizeof(int);   // int pc = extraer_int_del_buffer(buffer);
    
    int pid; 
    memcpy(&pid, buffer->stream + desplazamiento, sizeof(int));  // int pid = extraer_int_del_buffer(buffer);

    //TODO se puede cambiar por... deserializar_pc_pid(buffer, &pc, &pid);

    //creo una lista de instrucciones asociada al PID
    t_list* instruccion_a_enviar = obtener_instrucciones_proceso(pid);

    if(instruccion_a_enviar == NULL)
    {
        log_error(memoria_logger,"Error al obtener la lista de instrucciones");
        return;
    }
    
    if(pc < 0 || pc >= (list_size(instruccion_a_enviar))){
        
        log_error(memoria_logger, "Error: program_counter fuera de rango.");
        return;
    } 
    t_buffer* buffer_instruccion = crear_buffer();

    // Obtiene la instrucción en la posición pc
    t_instruccion* instruccion_enviar = list_get(instruccion_a_enviar, pc);

    serializar_instruccion(instruccion_enviar, buffer_instruccion); //le podriamos pasar el pid y pc para que lo serialice tambiemn
    
    t_paquete *paquete = crear_paquete(ENVIAR_INSTRUCCION, buffer_instruccion ); //ojo aca que estoy reutilizando el buffer, quizas necesito uno nuevo. el viejo tenia pc y pid

    // ELIMINAR LA LISTA ACÁ
    enviar_paquete(paquete, *socket_cpu);

}


t_list* obtener_instrucciones_proceso(int pid_enviado)
{ 
    for(int i=0; i<(list_size(lista_instrucciones_globales)); i++)
    {
        t_instrucciones_proceso* instrucciones_asociado_pid = list_get(lista_instrucciones_globales, i);
        if(instrucciones_asociado_pid -> pid == pid_enviado)
        {
            return instrucciones_asociado_pid -> lista_instrucciones;
        }
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

//                                         INIT_PROC34         17
void leerInstruccionesDesdeArchivo(char *nombre_archivo,int pid_asociado){

    char* path = string_new();
    string_append_with_format(&path, "%s%s", path_instrucciones(), nombre_archivo); //crea la ruta 
    FILE* nuevo_archivo = fopen(path, "r"); ///home/utnso/tp-2025-1c-Grupo-Operativos-/scripts/INIT_PROC34

    if(nuevo_archivo == NULL)
    {
        log_error(memoria_logger,"error al abrir el archivo");
        return;
    }
    
    int contador_instrucciones = 0;
    fseek(nuevo_archivo, 0, SEEK_SET);
    //char ** lista_instrucciones_creada;
    t_instrucciones_proceso *instrucciones_pid = malloc(sizeof(t_instrucciones_proceso));
    instrucciones_pid -> lista_instrucciones = list_create();
    char * linea = malloc(100); // Espacio suficiente para las instrucciones
    char* contenido;
    

    while(fgets(linea, 100, nuevo_archivo) != NULL)
    {   
        size_t len = strlen(linea);
        t_instruccion* instruccion = malloc(sizeof(t_instruccion));        
        if(len > 0 && linea[len -1] == '\n')
        {
            linea[len - 1] = '\0';
        } 
        contenido = strtok(linea, " ");
        instruccion->operacion = obtener_operacion_cpu(contenido);
        instruccion->cantidad_parametros = get_cant_parametros(instruccion->operacion);
        instruccion->parametros = malloc(sizeof(char*)*(instruccion->cantidad_parametros));

        for(int i=0; i<(instruccion->cantidad_parametros);i++){
            contenido = strtok(NULL, " ");
            instruccion->parametros[i] = malloc(strlen(contenido)+1);
            instruccion->parametros[i] = strdup(contenido);
        }
        
        list_add(instrucciones_pid -> lista_instrucciones, instruccion);
        contador_instrucciones++;
    }

    fclose(nuevo_archivo);

    instrucciones_pid -> pid = pid_asociado;

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
        return MEMORY_DUMP;
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
        case NOOP: case MEMORY_DUMP: case EXIT:
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
