#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct template {
	char* contenido;
	char* contenido_parte1;
	char* contenido_parte2;
	int largo_parte1;
	int largo_parte2;
} template_t;

//Recibe por parametro una estructura de tipo template_t y un float.
//Crea un template que tiene como dato el float pasado por parametro
void crear_template(template_t *template,const char* file_name);

//Recibe por parametro un puntero a un archivo y un puntero de tipo
//template_t. Lee el archivo y lo copia como dato en el template
void read_file(FILE *fp,template_t *template);

//Recibe por parametro un puntero a un template_t y parsea su contenido
//reemplazando la temperatura correspondiente.
void parser_template(template_t *template);

//Recibe por parametro un puntero a un template y una string y arma el
//template que se va a responder al cliente
void armar_template(template_t *template,char* file_name);

//Recibe un puntero a un template_t y lo destruye
void destruir_template(template_t *template);

char* get_template_parte1(template_t *template);

char* get_template_parte2(template_t *template);

int get_largo_parte1(template_t *template);

int get_largo_parte2(template_t *template);

#endif

