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

// Crea una lista vacia
void lista_crear(lista_t *lista);

//Recibe como parametro una lista y un string y agrega el string a la
//lista
bool lista_agregar_cliente(lista_t *lista, char *cliente);

//Recibe como parametro una lista y devuelve un booleano indicando
//si esta o  no vacia
bool lista_esta_vacia(const lista_t *lista); 

//Destruye la lista que recibe por parametro
void lista_destruir(lista_t* lista);

//Recibe por parametro una lista y un string y devuelve un booleano
//indicando si pudo sumar una aparicion de dicho string a la lista
bool lista_sumar_visita(lista_t *lista, char *cliente);

//Crea un iterador de la lista que recibe por parametro
void lista_iter_crear(lista_t *lista,lista_iter_t *iter);

//Avanza el iterador de la lista que recibe por parametro
bool lista_iter_avanzar(lista_iter_t *iter);

//Recibe por parametro un iterador y devuelve la cadena en la cual 
//se encuentra actualmente
char *lista_iter_ver_actual_cliente(const lista_iter_t *iter);

//Devuelve un size_t indicando el valor sobre el cual se encuentra
//actualmente el iterador
size_t lista_iter_ver_actual_visitas(const lista_iter_t *iter);

//Indica si el iterador que recibe como parametro esta al final
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador de la lista
void lista_iter_destruir(lista_iter_t *iter);

#endif // LISTA_H

