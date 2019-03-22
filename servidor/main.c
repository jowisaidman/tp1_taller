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


int recv_message(int skt, char *buf, int size) {
	int received = 0;
	int s = 0;
	bool is_the_socket_valid = true;

	while (received < size && is_the_socket_valid) {
		s = recv(skt, &buf[received], size-received, MSG_NOSIGNAL);
      
		if (s == 0) { // nos cerraron el socket :(
			is_the_socket_valid = false;
		}
		else if (s < 0) { // hubo un error >(
			is_the_socket_valid = false;
		}
		else {
			received += s;
		}
	}

	if (is_the_socket_valid) {
		return received;
	}
	else {
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
		}
		else if (s < 0) {
			is_the_socket_valid = false;
		}
		else {
			sent += s;
		}
	}

	if (is_the_socket_valid) {
		return sent;
	}
	else {
		return -1;
	}
}
 
	
int main(int argc, char *argv[]) {
	int s = 0;
	bool continue_running = true;
	bool is_the_accept_socket_valid = true;
   
	struct addrinfo hints;
	struct addrinfo *ptr;

	int skt, peerskt = 0;

	char buf[MAX_BUF_LEN];
	//char *tmp;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
	hints.ai_flags = AI_PASSIVE;     /* AI_PASSIVE for server           */

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

	// Activamos la opcion de Reusar la Direccion en caso de que esta
	// no este disponible por un TIME_WAIT
	int opt = 1; 
	s = setsockopt(skt, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		freeaddrinfo(ptr);
		return 1;
	}
	
	// Decimos en que direccion local queremos escuchar, en especial el puerto
	// De otra manera el sistema operativo elegiria un puerto random
	// y el cliente no sabria como conectarse
	//BIND
	s = bind(skt, ptr->ai_addr, ptr->ai_addrlen);
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		freeaddrinfo(ptr);
		return 1;
	}
	
	freeaddrinfo(ptr);

	// Cuanto clientes podemos mantener en espera antes de poder acceptarlos?
	s = listen(skt, 20);
	if (s == -1) {
		printf("Error: %s\n", strerror(errno));
		close(skt);
		return 1;
	}
   
	while (continue_running) {
		peerskt = accept(skt, NULL, NULL);   // aceptamos un cliente
		if (peerskt == -1) {
			printf("Error: %s\n", strerror(errno));
			continue_running = false;
			is_the_accept_socket_valid = false;
		}
		else {
			printf("New client\n");
			memset(buf, 0, MAX_BUF_LEN);
			recv_message(peerskt, buf, MAX_BUF_LEN-1); 
			printf("El mensaje fue %s\n",buf);
		}
		//continue_running = false;	
	}
	shutdown(skt, SHUT_RDWR);
	close(skt);

	if (is_the_accept_socket_valid) {
		return 1;
	} 
	else { 
		return 0;
	}
}

