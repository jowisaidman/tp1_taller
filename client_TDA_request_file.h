#ifndef _CLIENT_REQUEST_FILE_H_
#define _CLIENT_REQUEST_FILE_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct req_cliente {
    char* nombre;
    char* msg;
} req_cliente_t;

//Recibe como parametro un puntero de tipo req_cliente_t y pide
//memoria para el mensaje que contendra el request y el nombre.
void crear_request(req_cliente_t *req);

//Recibe como parametro un puntero de tipo req_cliente_t 
//y modifica el mensaje del mismo con un un request del 
//usuario dado por entrada estandar.
void get_request_stdin(req_cliente_t *req);

//Recibe como parametro un puntero de tipo req_cliente_t y setea el
//mensaje del req_cliente_t leyendolo de un archivo.
bool read_file(req_cliente_t *req);

void set_nombre(req_cliente_t *req,char nombre_r[]);

char* get_msg(req_cliente_t *req);

//Recibe como parametro un puntero de tipo req_cliente_t y 
//libera la memoria.
void destruir_request(req_cliente_t *req);

#endif

