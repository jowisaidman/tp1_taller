#define _POSIX_C_SOURCE 200112L
#define TAM_INICIAL 1024
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "common_TDA_socket_connect.h"
#include "server_TDA_socket_accept.h"
#include "server_TDA_request.h"
#include "server_TDA_lista.h"
#include "server_TDA_template.h"
#include "server_TDA_sensor.h"
	
int main(int argc, char *argv[]) {
	if (argc<4) {
		printf("Incorrect number of parameters\n");
		return 1;
	}

	bool is_the_accept_socket_valid = true;
	
	//creo socket
	socket_accept_t socket_server;
   	crear_socket_accept(&socket_server);
   	
   	//seteo direcciones posibles
   	if(!addrinfo_socket_accept(&socket_server,argv[1])) {
		destruir_socket_accept(&socket_server);
		return 1;
	}
	
	//seteo socket del servidor
	if(!skt_socket_accept(&socket_server)) {
		destruir_socket_accept(&socket_server);
		return 1;		
	} 

	//seteo opciones
	if(!skt_opciones_accept(&socket_server)) {
		destruir_socket_accept(&socket_server);
		return 1;		
	}
	
	//bind
	if (!bind_socket_accept(&socket_server)) {
		destruir_socket_accept(&socket_server);
		return 1;		
	}
	
	//listen
	if (!listen_socket_accept(&socket_server)) {
		destruir_socket_accept(&socket_server);
		return 1;		
	}

	bool continue_running = true; 
	int sensor_pos = 0;
	float temp = 0;
	lista_t lista_clientes;	
	lista_crear(&lista_clientes);

	//Creo template
	template_t template_rta;
	crear_template(&template_rta,argv[3]);
	armar_template(&template_rta,argv[3]);

	while (continue_running) {
		socket_connect_t socket_connect_s;
		is_the_accept_socket_valid = 
		accept_socket(&socket_server,&socket_connect_s);
		if(!is_the_accept_socket_valid) {
			continue_running = false;
		} else {
			char *buf = malloc(sizeof(char)*1024);
			memset(buf,'\0',TAM_INICIAL);	
			recibir_mensaje_socket(&socket_connect_s, buf,1024);
			request_t req; 
			request_crear(&req);
			parser(buf,&req);
			
			int valid_req = request_is_valid(&req);
			if (valid_req == 404) {
				request_destruir(&req);
				char *error404 = "HTTP/1.1 404 Not found\n\n";
				free(buf);
				enviar_mensaje_socket(&socket_connect_s,error404,strlen(error404));
				cerrar_canal_escritura(&socket_connect_s);
				continue;
			}
			if (valid_req == 400) {
				request_destruir(&req);
				char *error400 = "HTTP/1.1 400 Bad request\n\n";
				free(buf);
				enviar_mensaje_socket(&socket_connect_s,error400,strlen(error400));
				cerrar_canal_escritura(&socket_connect_s);
				continue;				
			}
			
			//Agrego visita al cliente
			if(!lista_sumar_visita(&lista_clientes,get_user_agent(&req))){
				lista_agregar_cliente(&lista_clientes,get_user_agent(&req));
			}
			
			//aca debo procesar el req y preparar el template
			sensor_t sensor;
			crear_sensor_server(&sensor);
			temp = get_sensor_temp(&sensor,argv[2],sensor_pos);
			sensor_pos+=2;
			if (sensor_reach_end(&sensor)) {
				continue_running = false;
			}
			
			//envio respuesta
			char* cabecera = "HTTP/1.1 200 OK\n\n";
			char temp_[64];
			enviar_mensaje_socket(&socket_connect_s,cabecera,strlen(cabecera));
			enviar_mensaje_socket(&socket_connect_s,get_template_parte1(&template_rta),
			get_largo_parte1(&template_rta));
			snprintf(temp_,sizeof(temp_), "%.2f", temp);
			enviar_mensaje_socket(&socket_connect_s,temp_,strlen(temp_));
			enviar_mensaje_socket(&socket_connect_s,get_template_parte2(&template_rta),
			get_largo_parte2(&template_rta));
			cerrar_canal_escritura(&socket_connect_s);
				
			//destruyo tdas y libero memoria
			request_destruir(&req);
			destruir_sensor_server(&sensor);
			free(buf);
			destruir_socket(&socket_connect_s);
		}
	}

	//Destruyo el template
	destruir_template(&template_rta);

	//cierro el socket y lo destruyo
	is_the_accept_socket_valid = destruir_socket_accept(&socket_server);
	
	//muestro visitantes
	lista_iter_t lista_iter_clientes;
	lista_iter_crear(&lista_clientes,&lista_iter_clientes);
	printf("# Estadisticas de visitantes\n\n"); 
	while (!lista_iter_al_final(&lista_iter_clientes)) {
		char *cliente = lista_iter_ver_actual_cliente(&lista_iter_clientes);
		size_t visitas = lista_iter_ver_actual_visitas(&lista_iter_clientes);
		printf("*%s: %zu\n",cliente,visitas);
		lista_iter_avanzar(&lista_iter_clientes);
	}
	lista_iter_destruir(&lista_iter_clientes);
	lista_destruir(&lista_clientes);

	//verifico que se cerro bien el socket
	if (is_the_accept_socket_valid) {
		return 1;
	} else { 
		return 0;
	}
}

