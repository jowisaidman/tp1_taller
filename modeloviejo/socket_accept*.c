#define _POSIX_C_SOURCE 200112L
#define REQUEST_MAX_LEN 128
#define RESPONSE_MAX_LEN 1024

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "socket_connect*.c"

typedef struct socket_connect {
    int s;
    int skt;
    int opt;
    struct addrinfo hints;
    struct addrinfo *ptr;
} socket_connect_t;

void crear_socket_connect(socket_connect_t *skt_s) { 
	skt_s->s = 0;
	skt_s->skt = 0;
	skt_s->opt = 1;
}

void set_hints_socket_server(socket_connect_t *skt_s) {
	memset(&skt_s->hints, 0, sizeof(struct addrinfo));
	skt_s->hints.ai_family = AF_INET;     
	skt_s->hints.ai_socktype = SOCK_STREAM; 
	skt_s->hints.ai_flags = 0;  	
}

bool addrinfo_socket_server(socket_connect_t *skt_s,char *srvn){
	set_hints_socket_server(skt_s);
	skt_s->s = getaddrinfo(NULL, srvn, &skt_s->hints, &skt_s->ptr);
   	if (skt_s->s != 0) { 
      		printf("Error in getaddrinfo: %s\n", gai_strerror(skt_s->s));
      		return false;
   	}
   	return true;	
}

bool skt_socket_server(socket_connect_t *skt_s) {
	skt_s->skt = socket(skt_s->ptr->ai_family, skt_s->ptr->ai_socktype,
	skt_s->ptr->ai_protocol);

	if (skt_s->skt == -1) {
		printf("Error: %s\n", strerror(errno));
		freeaddrinfo(skt_s->ptr);
		return false;
	}
	return true;	
}

bool skt_opciones_server(socket_connect_t *skt_s) {
	skt_s->s = setsockopt(skt_s->skt, SOL_SOCKET, SO_REUSEADDR,
	 skt_s->opt, sizeof(skt_s->opt));
	if (skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt_s->skt);
		freeaddrinfo(skt_s->ptr);
		return false;
	}
	return true;	
}

bool bind_socket_server(socket_connect_t *skt_s) {
	skt_s->s = bind(skt_s->skt, skt_s->ptr->ai_addr, skt_s->ptr->ai_addrlen);
	freeaddrinfo(skt_s->ptr);
	if (skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt_s->skt);
		return false;
	}
	return true;
}

bool listen_socket_server(socket_connect_t *skt_s) {
	skt_s->s = listen(skt_s->skt, 20);
	if (skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt_s->skt);
		return false;
	}
	return true;
}

bool accept_socket_server(socket_connect_t *skt_s,socket_connect_t *skt_c) {
	int skt = accept(skt_s->skt, NULL, NULL); 
	if (skt == -1) {
		printf("Error: %s\n", strerror(errno));
		return false;
	}
	crear_socket_connect_servidor(skt_c,skt);
	return true; 
}

int cerrar_socket_server(socket_connect_t *skt_s){
	return close(skt_s->skt);
} 

void destruir_socket_server(socket_server_t *skt_s) {
	
}


