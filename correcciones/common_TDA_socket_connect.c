#define _POSIX_C_SOURCE 200112L
#define RESPONSE_MAX_LEN 1024

#include "common_TDA_socket_connect.h"
#include <stdlib.h>

void set_hints_socket(socket_connect_t *skt_c) {
	memset(&skt_c->hints, 0, sizeof(struct addrinfo));
	skt_c->hints.ai_family = AF_INET;     
	skt_c->hints.ai_socktype = SOCK_STREAM; 
	skt_c->hints.ai_flags = 0;  	
}

int recv_message(int skt, char *buf,int tam) {
	int received = 0,s = 0;
	bool is_the_socket_valid = true;
	while (received < tam && is_the_socket_valid) {
		s = recv(skt, &buf[received], tam-received, MSG_NOSIGNAL);
		if (s == 0) {
			is_the_socket_valid = false;
		}
		if (s < 0) {
			is_the_socket_valid = false;
		} else {
			received += s;
		}
	}
	if (is_the_socket_valid) {
		return received;
	} else {
		return -1;
	}
}

int send_message(int skt, char *buf, int tam) {
	int enviado = 0,s = 0;
	bool is_the_socket_valid = true;
	while (enviado < tam && is_the_socket_valid) {
		s = send(skt, &buf[enviado], tam-enviado, MSG_NOSIGNAL); 
		if (s == 0) {
			is_the_socket_valid = false;
		} else if (s < 0) {
			is_the_socket_valid = false;
		} else {
			enviado += s;
		}
	}
	if (is_the_socket_valid) {
		return enviado;
	} else {
		return -1;
	}
}

void crear_socket_connect_cliente(socket_connect_t *skt_c) { 
	skt_c->s = 0;
	skt_c->skt = 0;
}

void crear_socket_connect_servidor(socket_connect_t *skt_c,int skt) {
	skt_c->s = 0;  
	skt_c->skt = skt;
}

bool addrinfo_socket(socket_connect_t *skt_c,char *hostn, char* srvn){
	set_hints_socket(skt_c);
	skt_c->s = getaddrinfo(hostn, srvn, &skt_c->hints, &skt_c->result);
   	if (skt_c->s != 0) { 
      		printf("Error in getaddrinfo: %s\n", gai_strerror(skt_c->s));
      		return false;
   	}
   	return true;	
}

bool connect_socket(socket_connect_t *skt_c) {
	bool are_we_connected = false;
   	for (skt_c->ptr = skt_c->result; skt_c->ptr!=NULL && !are_we_connected; 
   	skt_c->ptr=skt_c->ptr->ai_next) {
   		skt_c->skt = socket(skt_c->ptr->ai_family, skt_c->ptr->ai_socktype,
   		 skt_c->ptr->ai_protocol);
      	if (skt_c->skt == -1) {
         	printf("Error: %s\n", strerror(errno));
      	} else {
			skt_c->s = connect(skt_c->skt, skt_c->ptr->ai_addr,
         	skt_c->ptr->ai_addrlen);
			if (skt_c->s == -1) {
				printf("Error: %s\n", strerror(errno));
				close(skt_c->skt);
			}
			are_we_connected = (skt_c->s != -1);
		}
    }
	freeaddrinfo(skt_c->result);
	if (are_we_connected == false) {
		return false;
	}
	return true;
}

bool enviar_mensaje_socket(socket_connect_t *skt_c,char msg[]) {
	int tam_total = strlen(msg);
	int enviado = 0;
	while (tam_total>enviado) {
		int enviar = 100;
		if (tam_total-enviado<100) enviar = tam_total-enviado;
		char msg_env[enviar];
		strncpy(msg_env,msg+enviado,enviar);
		int send_ok = send_message(skt_c->skt,msg_env,enviar);
		if (send_ok == -1) {
			shutdown(skt_c->skt, SHUT_RDWR);
			close(skt_c->skt);
			printf("There was an error in the socket\n");
			return false;
		}
		enviado+=enviar;
	}	
	shutdown(skt_c->skt, SHUT_WR);
	return true;		
}

bool recibir_mensaje_socket(socket_connect_t *skt_c,char msg[],int tam) {
	int r = 0;
	int recibido = 0;
	while( r != -1) {
		//char buf_aux[50];
		//memset(buf_aux,'\0',50);	
		r = recv_message(skt_c->skt, msg+recibido,50);
		//for (int i = 0; i<strlen(buf_aux); i++) {
		//msg[recibido+i]=buf_aux[i];
		//memcpy(msg+recibido,buf_aux,strlen(buf_aux));
		//printf("buffer:aux: %s\n",buf_aux);
		//printf("largo: %li\n",strlen(buf_aux));
		//}
		if (recibido>=tam-50) {
			msg = (char*)realloc(msg,tam*2);
			tam = tam*2;
		}
		recibido += r;
	}
/*
	while (r != -1) {
		char *buf_aux =calloc(50,sizeof(char));
		r = recv_message(skt_c->skt, buf_aux,50);
		memcpy(msg+recibido,buf_aux,strlen(buf_aux)-1);
		//snprintf(msg+recibido,strlen(buf_aux),"%s%s",msg,buf_aux);
		printf("EL VALOR DE REV_MSG ES : %i\n",r);
		printf("ACA RECIBIO EL BUFFFEEEEERRRRRRR: %s\n",msg);
		recibido += (int)strlen(buf_aux);
		free(buf_aux);
	}
*/	
	return true;
}


void destruir_socket(socket_connect_t *skt_c) {
	//No hace nada
}


