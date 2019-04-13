#define _POSIX_C_SOURCE 200112L
#define REQUEST_MAX_LEN 128
#define RESPONSE_MAX_LEN 1024

#include <stdlib.h>
#include "client_TDA_request_file.h"
#include "common_TDA_socket_connect.h"

int main(int argc, char *argv[]) {
	if (argc< 3 || argc > 4) {
		printf("Error: invalid numer of parameters");
		return 1;
	}
	//Consigo el nombre del request
   	req_cliente_t req;
   	crear_request(&req);
	if (argc == 4) {
		set_nombre(&req,argv[3]);
	}   
	if (argc == 3) {
		set_nombre(&req,"stdin");
	}

	if(!leer_archivo(&req)) {
		destruir_request(&req);
		return 1;
	}
	
	//creo socket
   	socket_connect_t socket_cliente;
   	crear_socket_connect_cliente(&socket_cliente);              

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
	enviar_mensaje_socket(&socket_cliente,get_msg(&req));
	destruir_request(&req);
	
	//recibo respuesta
	char rta[RESPONSE_MAX_LEN];
	memset(rta,'\0',RESPONSE_MAX_LEN);	
	recv_message(&socket_cliente, rta);
	printf("%s",rta); 
	destruir_socket(&socket_cliente);
	return 0;
}

