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
#include "library_common*.c"
#include "request_server*.c"
#include "lista_server*.c"
#include "template_server*.c"
#include "sensor_server*.c"
	
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
		} else {
			memset(buf, 0, MAX_BUF_LEN);
			recv_message(peerskt, buf, MAX_BUF_LEN-1);
			request_t* req =request_crear();		
			parser(buf,req);
			
			//Agrego visita al cliente
			if(!lista_sumar_visita(&lista_clientes,get_user_agent(req))){
				lista_agregar_cliente(&lista_clientes,get_user_agent(req));
			}
			
			int valid_req = request_is_valid(req);
			if (valid_req == 404) {
				request_destruir(req);
				char *error404 = "HTTP/1.1 404 Not found\n\n";
				send_message(peerskt,error404,strlen(error404));
				shutdown(peerskt, SHUT_RDWR);
				continue;
			}
			if (valid_req == 400) {
				request_destruir(req);
				char *error400 = "HTTP/1.1 400 Bad request\n\n";
				send_message(peerskt,error400,strlen(error400));
				shutdown(peerskt, SHUT_RDWR);
				continue;				
			}
			
			//aca debo procesar el req y preparar el template
			bool reach_end = false;
			temp = get_sensor_temp(argv[2],b_pos,&reach_end);
			if (reach_end) continue_running = false;
			
			char* buf = calloc(1024,sizeof(char));
			get_template(argv[3],temp,buf);
			char respuesta[1024];
			snprintf(respuesta,sizeof(char)*1024,"HTTP/1.1 200 OK\n\n%s", buf);		
			//aca envio respuesta al cliente
			send_message(peerskt,respuesta,strlen(respuesta));
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
		printf("* %s: %zu\n",cliente,visitas);
		lista_iter_avanzar(&lista_iter_clientes);
	}
	lista_iter_destruir(&lista_iter_clientes);
	lista_destruir(&lista_clientes);

	if (is_the_accept_socket_valid) {
		return 1;
	} else { 
		return 0;
	}
}

