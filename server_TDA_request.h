#ifndef REQUEST_H
#define REQUEST_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct request {
    void* action;
    void* resourse;
    void* http_p;
    void* header;
} request_t;

// Crea un request
// Post: request es un request vacío
request_t* request_crear();

// Destruye el request
// Pre: el request fue creado
// Post: se eliminaron todos los elementos del request
void request_destruir(request_t* req);

//Toma como parametro una cadena y una estructura de dato
//de tipo request_t y setea los datos correspondientes. 
void parser(char msg[],request_t* req);

//Toma como parametro un puntero a una estructura de tipo request_t 
//y verifica si sus datos son validos para un request http.
int request_is_valid(request_t* req);

void* get_user_agent(request_t* req);

#endif



