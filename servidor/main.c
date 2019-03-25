#define _POSIX_C_SOURCE 200112L
#define MAX_BUF_LEN 64 
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "request.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "library_common.h"

void parser(char msg[],request_t* req) {
	if (msg == NULL) return;
	char *delim = " \n\r:";
	char *token = strtok(msg, delim);
	int pos = 0;
    while(token != NULL){
        if (pos == 0) set_action(req,token);
        if (pos == 1) set_resourse(req,token);
        if (pos == 2) set_http_p(req,token);
        if (strcmp((const char*)token,"User-Agent")==0) {
			token = strtok(NULL, delim);
			set_user_agent(req,token);
		}
        token = strtok(NULL, delim);
        pos++;
    }
}

bool request_is_valid(request_t* req) {
	const char* action = (const char*)get_action(req);
	if (strcmp(action,"GET") != 0) return false;
	const char* resource = (const char*)get_resourse(req);
	if (strcmp(resource,"/sensor") != 0) return false;
	const char* http_p = (const char*)get_http_p(req);
	if (strcmp(http_p,"HTTP/1.1") != 0) return false;
	return true;
}

int read_temp(char *file_name,long int pos) { //ta leyendo mal esto
	FILE *fp;
	int num_read;
	if ((fp = fopen(file_name,"rb")) == NULL){
		printf("Error! opening file");
		return 0;
	}
	fseek(fp,pos,SEEK_SET); 
	if(!fread(&num_read, 1, 2, fp)) return 0;
	num_read = (num_read & 0x0000ffff);
	printf("leido: %i\n",num_read);
	return num_read;
}

float get_sensor_temp(char *file_name,long int pos) {
	int number_read = read_temp(file_name,pos);
	float temp = ((number_read-2000)/100);
	return temp;
}
 
 
	
int main(int argc, char *argv[]) {
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
	
	while (continue_running) {
		peerskt = accept(skt, NULL, NULL); 
		if (peerskt == -1) {
			printf("Error: %s\n", strerror(errno));
			continue_running = false;
			is_the_accept_socket_valid = false;
		}
		else {
			memset(buf, 0, MAX_BUF_LEN);
			recv_message(peerskt, buf, MAX_BUF_LEN-1);
			request_t* req =request_crear();		
			parser(buf,req);
			if (!request_is_valid(req)) {
				printf("The request is invalid\n");
				continue_running = false;
			}
			//aca debo procesar el req y hacer lo necesario
			temp = get_sensor_temp(argv[2],b_pos);
			if (!temp) return 1;
			//aca envio respuesta al cliente
			//char ans[100];
			//sprintf(ans, "La temperatura leida fue: %f",temp);
			char* ans = "ta ok\n";
			send_message(peerskt,ans,strlen(ans)-1);
			shutdown(peerskt, SHUT_RDWR);
			request_destruir(req);
			b_pos+=4;
			printf("Answer OK\n");
			continue_running = false;
		}
	}
	close(skt);

	if (is_the_accept_socket_valid) {
		return 1;
	} 
	else { 
		return 0;
	}
}

/*			printf("llego bien action: %s\n",(char*)get_action(req));
			printf("llego bien resource: %s\n",(char*)get_resourse(req));
			printf("llego bien http_p: %s\n",(char*)get_http_p(req));
			printf("llego bien user agent: %s\n",(char*)get_user_agent(req));*/
