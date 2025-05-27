#include "../include/operarConCPU.h"
/*
void operar_con_cpu(int *socket_cpu)
{ 
    
    t_buffer *buffer;
    t_paquete *paquete;
    int desplazamiento;
    int dir_fisica;
    int pid;

    while (1)
    {   
        
        int cod_op = recibir_operacion(*socket_cpu);
        buffer = crear_buffer();
        buffer->stream = recibir_buffer(&(buffer->size),*socket_cpu);
        desplazamiento = 0;
        switch (cod_op)
        {
        case SOLICITUD_INSTRUCCION:
           
            enviar_instruccion(socket_cpu, buffer); 
            break;

        case SOLICITUD_MARCO:

            responder_solicitud_de_marco(socket_cpu,buffer);
            break;
*/