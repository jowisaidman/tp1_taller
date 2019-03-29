#define _POSIX_C_SOURCE 200112L
#define MAX_BUF_LEN 64 
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "library_common.h"
#include "request.h"
#include "lista.h"

void parser(char msg[],request_t* req) {
	if (msg == NULL) return;
	char *delim = " \n\r:";
	char *token = strtok(msg, delim);
	int pos = 0;
    while(token != NULL){
        if (pos == 0) set_action(req,token);
        if (pos == 1) set_resourse(req,token);
        if (pos == 2) set_http_p(req,token);
        if (strcmp((const char*)token,"User-Agent")==0) {
			token = strtok(NULL, delim);
			set_user_agent(req,token);
		}
        token = strtok(NULL, delim);
        pos++;
    }
}

bool request_is_valid(request_t* req) {
	const char* action = (const char*)get_action(req);
	if (strcmp(action,"GET") != 0) return false;
	const char* resource = (const char*)get_resourse(req);
	if (strcmp(resource,"/sensor") != 0) return false;
	const char* http_p = (const char*)get_http_p(req);
	if (strcmp(http_p,"HTTP/1.1") != 0) return false;
	return true;
}

int read_temp(char *file_name,long int pos,bool *reach_end) {
	FILE *fp;
	uint16_t s1;
	if ((fp = fopen(file_name,"rb")) == NULL){
		printf("Error! opening file");
		return 0;
	}
	fseek(fp,pos,SEEK_SET); 
	if (!fread(&s1, 2, 1, fp)) {
		*reach_end=true;
		fseek(fp,pos-2,SEEK_SET);
		if(!fread(&s1, 2, 1, fp)) return 0;
	}
	s1 = htons(s1);
	fclose(fp);
	return s1;
}


float get_sensor_temp(char *file_name,long int pos,bool *reach_end) {
	int number_read = read_temp(file_name,pos,reach_end);
	float temp = ((number_read-2000)/100);
	return temp;
}

void parser_template(char buf[],float temp) {
	if (buf == NULL) return;
	char *str1 = calloc(strlen(buf),sizeof(char));
	char *str2 = calloc(strlen(buf),sizeof(char));
    char *ptr;
    int pos;
    int max = strlen(buf);
    ptr=strstr(buf,"{{datos}}");
    pos = ptr - buf;
    for (int i=0; i<pos; i++) {
		str1[i]=buf[i];
	}
	pos+=9;
	for (int i=0; i<max-pos; i++) {
		str2[i]=buf[i+pos];
	}
	if(!sprintf(buf,"%s%.2f%s",str1,temp,str2)) printf("Error");
	free(str1);
	free(str2);
}

void read_file(FILE *fp,char buf[]) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		buf[pos] = c;
		pos++;
		if (pos>=1024) break;
	}
}

void get_template(char *file_name,float temp,char buf[]) {
	FILE* fp = NULL;
	fp=fopen(file_name,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	read_file(fp,buf);
	parser_template(buf,temp);
	fclose(fp);
}
 
	
int main(int argc, char *argv[]) {
	if (argc<4) {
		printf("Incorrect number of parameters\n");
		return 1;
	}
	int s = 0;
	int opt = 1;
	bool is_the_accept_socket_valid = true;
	bool continue_running = true;
	struct addrinfo hints;
	struct addrinfo *ptr;
	int skt, peerskt = 0;
	char buf[MAX_BUF_LEN];
	long int b_pos = 0;
	float temp = 0;//no deberia empezar en 0
	lista_t lista_clientes;
	lista_iter_t lista_iter_clientes; 

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       
	hints.ai_socktype = SOCK_STREAM; 
	hints.ai_flags = AI_PASSIVE;    

	s = getaddrinfo(NULL, argv[1], &hints, &ptr);

	if (s != 0) { 
		printf("Error in getaddrinfo: %s\n", gai_strerror(s));
		return 1;
	}

	skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (skt == -1) {
		printf("Error: %s\n", strerror(errno));
		freeaddrinfo(ptr);
		return 1;
	}
 
	s = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		freeaddrinfo(ptr);
		return 1;
	}
	
	s = bind(skt, ptr->ai_addr, ptr->ai_addrlen);
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		freeaddrinfo(ptr);
		return 1;
	}
	
	freeaddrinfo(ptr);

	s = listen(skt, 20);
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		return 1;
	}
	
	lista_crear(&lista_clientes);
	while (continue_running) {
		peerskt = accept(skt, NULL, NULL); 
		if (peerskt == -1) {
			printf("Error: %s\n", strerror(errno));
			continue_running = false;
			is_the_accept_socket_valid = false;
		}
		else {
			memset(buf, 0, MAX_BUF_LEN);
			recv_message(peerskt, buf, MAX_BUF_LEN-1);
			request_t* req =request_crear();		
			parser(buf,req);
			if (!request_is_valid(req)) {
				printf("The request is invalid\n");
				continue_running = false;
			}
			//Agrego visita al cliente
			if(!lista_sumar_visita(&lista_clientes,get_user_agent(req))){
				lista_agregar_cliente(&lista_clientes,get_user_agent(req));
			}
			
			//aca debo procesar el req y preparar el template
			bool reach_end = false;
			temp = get_sensor_temp(argv[2],b_pos,&reach_end);
			if (reach_end) continue_running = false;
			
			char* buf = calloc(1024,sizeof(char));
			get_template(argv[3],temp,buf);
					
			//aca envio respuesta al cliente
			send_message(peerskt,buf,strlen(buf));
			shutdown(peerskt, SHUT_RDWR);
			request_destruir(req);
			b_pos+=2;
			free(buf);
		}
	}
	close(skt);
	lista_iter_crear(&lista_clientes,&lista_iter_clientes);
	printf("# Estadisticas de visitantes\n\n"); 
	while (!lista_iter_al_final(&lista_iter_clientes)) {
		char *cliente = lista_iter_ver_actual_cliente(&lista_iter_clientes);
		size_t visitas = lista_iter_ver_actual_visitas(&lista_iter_clientes);
		printf("%s: %li\n",cliente,visitas);
		lista_iter_avanzar(&lista_iter_clientes);
	}
	lista_iter_destruir(&lista_iter_clientes);
	lista_destruir(&lista_clientes);

	if (is_the_accept_socket_valid) {
		return 1;
	} 
	else { 
		return 0;
	}
}

