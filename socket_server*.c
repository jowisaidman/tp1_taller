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

typedef struct s_server {
    int* s;
    int* skt;
    int* opt;
    int* peerskt;
    struct addrinfo hints;
    struct addrinfo *ptr;
} socket_server_t;

void crear_socket_server(socket_server_t *skt_s) { 
	skt_s->s = (int*)calloc(1, sizeof(int));
	*skt_s->s = 0;
	skt_s->skt = (int*)calloc(1, sizeof(int));
	*skt_s->skt = 0;
	skt_s->opt = (int*)calloc(1, sizeof(int));
	*skt_s->opt = 1;
	skt_s->peerskt = (int*)calloc(1, sizeof(int));
	*skt_s->peerskt = 0;	
}

int recv_message(int skt, char *buf, int size) {
	int received = 0;
	int s = 0;
	bool is_the_socket_valid = true;
	while (received < size && is_the_socket_valid) {
		s = recv(skt, &buf[received], size-received, MSG_NOSIGNAL);
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

int send_message(int skt, char *buf, int size) {
	int sent = 0;
	int s = 0;
	bool is_the_socket_valid = true;
	while (sent < size && is_the_socket_valid) {
		s = send(skt, &buf[sent], size-sent, MSG_NOSIGNAL); 
		if (s == 0) {
			is_the_socket_valid = false;
		} else if (s < 0) {
			is_the_socket_valid = false;
		} else {
			sent += s;
		}
	}
	if (is_the_socket_valid) {
		return sent;
	} else {
		return -1;
	}
}

void set_hints_socket_server(socket_server_t *skt_s) {
	memset(&skt_s->hints, 0, sizeof(struct addrinfo));
	skt_s->hints.ai_family = AF_INET;     
	skt_s->hints.ai_socktype = SOCK_STREAM; 
	skt_s->hints.ai_flags = 0;  	
}

bool addrinfo_socket_server(socket_server_t *skt_s,char *srvn){
	set_hints_socket_server(skt_s);
	*skt_s->s = getaddrinfo(NULL, srvn, &skt_s->hints, &skt_s->ptr);
   	if (*skt_s->s != 0) { 
      		printf("Error in getaddrinfo: %s\n", gai_strerror(*skt_s->s));
      		return false;
   	}
   	return true;	
}

bool skt_socket_server(socket_server_t *skt_s) {
	*skt_s->skt = socket(skt_s->ptr->ai_family, skt_s->ptr->ai_socktype,
	 skt_s->ptr->ai_protocol);

	if (*skt_s->skt == -1) {
		printf("Error: %s\n", strerror(errno));
		freeaddrinfo(skt_s->ptr);
		return false;
	}
	return true;	
}

bool skt_opciones_server(socket_server_t *skt_s) {
	*skt_s->s = setsockopt(*skt_s->skt, SOL_SOCKET, SO_REUSEADDR,
	 skt_s->opt, sizeof(*skt_s->opt));
	if (*skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(*skt_s->skt);
		freeaddrinfo(skt_s->ptr);
		return false;
	}
	return true;	
}

bool bind_socket_server(socket_server_t *skt_s) {
	*skt_s->s = bind(*skt_s->skt, skt_s->ptr->ai_addr, skt_s->ptr->ai_addrlen);
	freeaddrinfo(skt_s->ptr);
	if (*skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(*skt_s->skt);
		return false;
	}
	return true;
}

bool listen_socket_server(socket_server_t *skt_s) {
	*skt_s->s = listen(*skt_s->skt, 20);
	if (*skt_s->s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(*skt_s->skt);
		return false;
	}
	return true;
}

bool accept_socket_server(socket_server_t *skt_s) {
	*skt_s->peerskt = accept(*skt_s->skt, NULL, NULL); 
	if (*skt_s->peerskt == -1) {
		printf("Error: %s\n", strerror(errno));
		return false;
	}
	return true; 
}

void recv_msg_socket_server(socket_server_t *skt_s,char buf[],int tam){
	recv_message(*skt_s->peerskt, buf, tam);
}

void envr_msg_socket_server(socket_server_t *skt_s,char msg[]) {
	send_message(*skt_s->peerskt,msg,strlen(msg));
	shutdown(*skt_s->peerskt, SHUT_RDWR);	
}

int cerrar_socket_server(socket_server_t *skt_s){
	return close(*skt_s->skt);
} 

void destruir_socket_server(socket_server_t *skt_s) {
	free(skt_s->s);
	free(skt_s->skt);
	free(skt_s->opt);
	free(skt_s->peerskt);
}


