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
#include "libreria_aux.h"


int main(int argc, char *argv[]) {
	//Consigo el nombre del request
	char req[50];
	if (argc == 4) { //[1]
		get_request_param(argv,req);
	}
	if (argc < 4) {
		get_request_stdin(req);
	}
	//printf("El req es: %s\n",req);
	//printf("El largo del req es: %li\n",strlen(req));
	
	
	//Leo el request y lo valido (preparo todo para enviar al server)
	fp=fopen(req,"r");
	if ((fp==NULL)) {
		printf("ERROR\n");
		return 1;
	}
	
	//char line[1000];
	//printf("Se leyo la linea: %s\n",read_line(fp,line));
	
	
	
	
	
	
	//Creo sockets y conecto con el server
	bool are_we_connected = false;
	int s = 0;
   	struct addrinfo hints;
   	struct addrinfo *result, *ptr;
 	memset(&hints, 0, sizeof(struct addrinfo));//[por ahi conviene hacer esto en un .h]
   	hints.ai_family = AF_INET;       /* IPv4 (or AF_INET6 for IPv6)     */
   	hints.ai_socktype = SOCK_STREAM; /* TCP  (or SOCK_DGRAM for UDP)    */
   	hints.ai_flags = 0;              /* None (or AI_PASSIVE for server) */
   	const char *hostname = argv[1]; //puede que tenga que usar localhost (sino 127.0.0.1)
  	const char *servicename = argv[2];		//puede que aca tenga que ir un puerto (8080,9004,9005)
 	int skt = 0;
	s = getaddrinfo(hostname, servicename, &hints, &result);
   	if (s != 0) { 
      		printf("Error in getaddrinfo: %s\n", gai_strerror(s));
      		return 1;
   	}

   	for (ptr = result; ptr != NULL && are_we_connected == false; ptr = ptr->ai_next) {
   		skt = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
      	if (skt == -1) {
         	printf("Error: %s\n", strerror(errno));
      	} else {
         	s = connect(skt, ptr->ai_addr, ptr->ai_addrlen);
			if (s == -1) {
				printf("Error: %s\n", strerror(errno));
				close(skt);
			}
			are_we_connected = (s != -1);
		}
     }
     
     
	freeaddrinfo(result);

	if (are_we_connected == false) {
		return 1;
	}
	printf("FIN\n");
	return 0;

}

//[1]Si se le debe decir si es server o cliente aca va 4

//visual studio code




