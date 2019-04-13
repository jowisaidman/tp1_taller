#define LARGO_INICIAL 512 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "client_TDA_request_file.h"

void crear_request(req_cliente_t *req) { 
	req->msg = calloc(LARGO_INICIAL,sizeof(char));
	req->nombre = calloc(LARGO_INICIAL,sizeof(char));
}

void set_nombre(req_cliente_t *req,char nombre_r[]) {
	memcpy(req->nombre,nombre_r,strlen(nombre_r));
}

char* get_msg(req_cliente_t *req) {
	return req->msg;
}

bool leer_archivo(req_cliente_t *req) {
	FILE *entrada;
	if (strcmp(req->nombre,"stdin") == 0) {	
		entrada = stdin;
	} else {
		entrada = fopen(req->nombre, "r");
		if (entrada == NULL) return false;
	}
	char c;
	int pos=0;
	while( (c=fgetc(entrada))!=EOF ) {
		req->msg[pos] = c;
		pos++;
	}
	if (entrada != stdin) fclose(entrada);
	printf("El req->msg es: %s\n",req->msg);	
	return true;
}

void destruir_request(req_cliente_t *req) { 
	free(req->msg);
	free(req->nombre);
}

