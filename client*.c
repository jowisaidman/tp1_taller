#define _POSIX_C_SOURCE 200112L
#define REQUEST_MAX_LEN 128
#define RESPONSE_MAX_LEN 1024

#include <stdlib.h>
#include "request_file_client*.c"
#include "socket_cliente*.c"

int main(int argc, char *argv[]) {
	if (argc< 3 || argc > 4) {
		printf("Error: invalid numer of parameters");
		return 1;
	}
	//Consigo el nombre del request
   	req_cliente_t req;
   	crear_request(&req);  
	if (argc == 4) {
		//snprintf(req,sizeof(char)*32,"%s",argv[3]);
		set_nombre(&req,argv[3]);
		if(!read_file(&req)) {
			destruir_request(&req);
			return 1;
		}
	}
	if (argc == 3) {
		get_request_stdin(&req);
	}
	
	//creo socket
   	socket_cliente_t socket_cliente;
   	crear_socket(&socket_cliente);              

	//tomo direcciones posibles
   	if(!addrinfo_socket(&socket_cliente,argv[1],argv[2])) {
		destruir_request(&req);
		destruir_socket(&socket_cliente);
		return 1;
	}

	//conecto cliente
	if(!connect_socket(&socket_cliente)) {
		destruir_request(&req);
		destruir_socket(&socket_cliente);
		return 1;
	}
	
	//envio mensaje
	enviar_mensage_socket(&socket_cliente,get_msg(&req));
	destruir_request(&req);
	
	//espero respuesta
	recibir_mensage_socket(&socket_cliente);
	destruir_socket(&socket_cliente);
	return 0;
}

