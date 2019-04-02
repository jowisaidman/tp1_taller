#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct nodo_lista {
	char* cliente;
	size_t visitas;
	struct nodo_lista* prox;
} nodo_lista_t;

typedef struct lista {
	nodo_lista_t* primero;
	nodo_lista_t* ultimo;
	size_t largo;
} lista_t;

typedef struct lista_iter{
	nodo_lista_t* actual;
	lista_t* lista;
} lista_iter_t;


void lista_crear(lista_t *lista);

bool lista_agregar_cliente(lista_t *lista, char *cliente);

bool lista_esta_vacia(const lista_t *lista); 

void lista_destruir(lista_t* lista);

bool lista_sumar_visita(lista_t *lista, char *cliente);

void lista_iter_crear(lista_t *lista,lista_iter_t *iter);

bool lista_iter_avanzar(lista_iter_t *iter);

char *lista_iter_ver_actual_cliente(const lista_iter_t *iter);

size_t lista_iter_ver_actual_visitas(const lista_iter_t *iter);

bool lista_iter_al_final(const lista_iter_t *iter);

void lista_iter_destruir(lista_iter_t *iter);

#endif // LISTA_H

