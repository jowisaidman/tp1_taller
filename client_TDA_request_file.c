#define MAX_BUF_LEN 512 

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "client_TDA_request_file.h"

void crear_request(req_cliente_t *req) { 
	req->msg = calloc(MAX_BUF_LEN,sizeof(char));
	req->nombre = calloc(MAX_BUF_LEN,sizeof(char));
}

void set_nombre(req_cliente_t *req,char nombre_r[]) {
	nombre_r[strcspn(nombre_r,"\n")]='\0';
	memcpy(req->nombre,nombre_r,strlen(nombre_r));
}

char* get_msg(req_cliente_t *req) {
	return req->msg;
}

void get_request_stdin(req_cliente_t *req){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the the request (<c> to continue): \n");
	char buf1[MAX_BUF_LEN];
	char buf2[MAX_BUF_LEN];
	while (true) {
		if (fgets(buf1,MAX_BUF_LEN,stdin)==NULL) return;
		if (strcmp(buf1,"c\n") == 0) break;
		snprintf(buf2,sizeof(char)*MAX_BUF_LEN,"%s%s",req->msg,buf1);
		memcpy(req->msg,buf2,strlen(buf2));
	}
}

bool read_file(req_cliente_t *req) {//podria verificar
	FILE *fp;
	fp=fopen(req->nombre,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return false;
	}
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		req->msg[pos] = c;
		pos++;
	}
	fclose(fp);
	return true;
}

void destruir_request(req_cliente_t *req) { 
	free(req->msg);
	free(req->nombre);
}

