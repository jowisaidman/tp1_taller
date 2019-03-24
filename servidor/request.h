#ifndef VECTOR_DINAMICO_H
#define VECTOR_DINAMICO_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 ******************************************************************/

typedef struct request {
    void* action;
    void* resourse;
    void* http_p;
    void* header;
    void* host;
} request_t;

// Crea un request
// Post: request es un request vacío
request_t* request_crear();

// Destruye el request
// Pre: el request fue creado
// Post: se eliminaron todos los elementos del request
void request_destruir(request_t* req);

bool set_action(request_t* req,char action[]);

bool set_http_p(request_t* req,char action[]);

bool set_resourse(request_t* req,char action[]);

bool set_user_agent(request_t* req,char action[]);

bool set_host(request_t* req,char host[]);

void* get_action(request_t* req);

void* get_user_agent(request_t* req);

void* get_http_p(request_t* req);

void* get_resourse(request_t* req);

void* get_host(request_t* req);

#endif
