#define LARGO_INICIAL 512 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "client_TDA_request_file.h"

void crear_request(req_cliente_t *req) { 
	req->msg = calloc(LARGO_INICIAL,sizeof(char));
	req->nombre = calloc(LARGO_INICIAL,sizeof(char));
	req->largo = LARGO_INICIAL;
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
		fseek(entrada,0,SEEK_END);
		req->msg = (char*)realloc(req->msg,ftell(entrada)+2);
		fseek(entrada,0,SEEK_SET); 
	}
	char c;
	int pos=0;
	while( (c=fgetc(entrada))!=EOF ) {
		if (pos+1 == req->largo) {
			req->msg = (char*)realloc(req->msg,req->largo*2);
			req->largo = req->largo*2;
		}
		req->msg[pos] = c;
		pos++;
	}
	req->msg[pos+1] = '\0'; //puede llegar a romper si justo pos estaba en el ultimo lugar, deberia corregirlo junto con la funcoin agrandar_msg
	if (entrada != stdin) fclose(entrada);
	return true;
}

void destruir_request(req_cliente_t *req) { 
	free(req->msg);
	free(req->nombre);
}

