#include "lista.h"
#include <stdlib.h>
#include <string.h>

nodo_lista_t* nodo_crear(nodo_lista_t* nuevo_nodo,char* cliente){
	if(nuevo_nodo==NULL)return NULL;
	nuevo_nodo->cliente = calloc(strlen(cliente)+1,sizeof(char));
	memcpy(nuevo_nodo->cliente,cliente,strlen(cliente));
	nuevo_nodo->visitas = 1;
	nuevo_nodo->prox = NULL;
	return nuevo_nodo;
}

void lista_crear(lista_t *lista){
	if(lista==NULL) return;
	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->largo = 0;
}	

bool lista_agregar_cliente(lista_t *lista, char *cliente){
	nodo_lista_t* nuevo_nodo = malloc(sizeof(nodo_lista_t));
	nodo_crear(nuevo_nodo,cliente);
	if(nuevo_nodo==NULL)return false;
	if(lista_esta_vacia(lista)){
		lista->primero = nuevo_nodo;
	} else {
		lista->ultimo->prox = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	}
	lista->largo++;
	return true;			
}

bool lista_esta_vacia(const lista_t *lista){
	return (lista->largo==0 && lista->primero==NULL);
}

void lista_borrar_primero(lista_t *lista){
	if (lista_esta_vacia(lista)) return;
	nodo_lista_t* nodo_destruir = lista->primero;
	lista->primero = lista->primero->prox;
	free(nodo_destruir->cliente);
	free(nodo_destruir);
	lista->largo--;
}

void lista_destruir(lista_t *lista){
	while (!lista_esta_vacia(lista)){
		lista_borrar_primero(lista);	
	}	
}

bool lista_sumar_visita(lista_t *lista, char *cliente) {
	nodo_lista_t* nodo_actual = lista->primero;
	for (int i=0; i<lista->largo;i++) {
		char *cliente_actual = nodo_actual->cliente;
		if (strcmp(cliente_actual,cliente) == 0) {
			nodo_actual->visitas++;
			return true;	
		}
		nodo_actual = nodo_actual->prox;		
	}
	return false;
}

/*Primitivas del iterador de la lista*/

void lista_iter_crear(lista_t *lista,lista_iter_t *iter){
	if (iter==NULL) return;
	iter->actual = lista->primero;
	iter->lista = lista;
}

bool lista_iter_avanzar(lista_iter_t *iter){
	if(iter->actual==NULL){
		return false;
	}
	iter->actual = iter->actual->prox;
	return true;
}

char *lista_iter_ver_actual_cliente(const lista_iter_t *iter){
	if(iter->actual==NULL)return NULL;
	return iter->actual->cliente;
}

size_t lista_iter_ver_actual_visitas(const lista_iter_t *iter){
	if(iter->actual==NULL)return 0;
	return iter->actual->visitas;
}

bool lista_iter_al_final(const lista_iter_t *iter){
	return (iter->actual==NULL);
}

void lista_iter_destruir(lista_iter_t *iter){
	//no hace nada porque no pedi memoria
}


