#define _POSIX_C_SOURCE 200112L
#define RESPONSE_MAX_LEN 1024

#include "common_TDA_socket_connect.h"

void set_hints_socket(socket_connect_t *skt_c) {
	memset(&skt_c->hints, 0, sizeof(struct addrinfo));
	skt_c->hints.ai_family = AF_INET;     
	skt_c->hints.ai_socktype = SOCK_STREAM; 
	skt_c->hints.ai_flags = 0;  	
}

int recv_message(socket_connect_t *skt_c, char *buf) {
	int received = 0,s = 0;
	int size = RESPONSE_MAX_LEN;
	bool is_the_socket_valid = true;
	while (received < size && is_the_socket_valid) {
		s = recv(skt_c->skt, &buf[received], size-received, MSG_NOSIGNAL);
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

int send_message(socket_connect_t *skt_c, char *buf) {
	int sent = 0,s = 0;
	int size = strlen(buf);
	bool is_the_socket_valid = true;
	while (sent < size && is_the_socket_valid) {
		s = send(skt_c->skt, &buf[sent], size-sent, MSG_NOSIGNAL); 
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
	int send_ok = send_message(skt_c,msg);
	shutdown(skt_c->skt, SHUT_WR);
	if (send_ok == -1) {
		shutdown(skt_c->skt, SHUT_RDWR);
		close(skt_c->skt);
		printf("There was an error in the socket\n");
		return false;
	}
	return true;		
}


void destruir_socket(socket_connect_t *skt_c) {
	//No hace nada
}


