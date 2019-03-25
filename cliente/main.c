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
#include "library_common.c"

void get_request_stdin(char req[]){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the path of the request: ");
	if (fgets(req,30,stdin)==NULL) return;
	req[strcspn ( req, "\n" )] = '\0';
}

void read_file(FILE *fp,char line[]) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		line[pos] = c;
		pos++;
	}
}

void get_request_param(char *argv[],char req[]) {
	FILE* fp = NULL;
	fp=fopen(argv[3],"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	read_file(fp,req);
	fclose(fp);	
}

int main(int argc, char *argv[]) {
	if (argc< 3 || argc > 4) {
		printf("Error: invalid numer of parameters");
		return 1;
	}
	
	//Consigo el nombre del request
	char req[50];
	if (argc == 4) {
		get_request_param(argv,req);
	}
	if (argc == 3) {
		get_request_stdin(req);
	}
	
	//Leo el request(preparo todo para enviar al server)
	FILE *fp;
	printf("%s\n",req);
	fp=fopen(req,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return 1;
	}
	char message[200];
	read_file(fp,message);
	fclose(fp);
	
	//Creo sockets y conecto con el server
	bool are_we_connected = false;
	bool is_there_a_socket_error = false;
	bool is_the_remote_socket_closed = false;
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
		
	
	//envio mensaje
	send_message(skt,message,strlen(message));
	shutdown(skt, SHUT_WR);
	if (is_the_remote_socket_closed || is_there_a_socket_error) {
		shutdown(skt, SHUT_RDWR);
		close(skt);
		printf("There was an error in the socket\n");
		return 1;
	}
	//espero respuesta
	char rta[100];
	recv_message(skt,rta, RESPONSE_MAX_LEN-1);
	printf("%s\n",rta);
	return 0;
}

//visual studio code




