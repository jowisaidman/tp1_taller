#ifndef SOCKET_CONNECT
#define SOCKET_CONNECT

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

typedef struct socket_connect {
    int s;
    int skt;
    struct addrinfo hints;
    struct addrinfo *result;
    struct addrinfo *ptr;
} socket_connect_t;

//Recibe por parametro un puntero a socket_connect_t, un string y un int.
//Espera un mensaje del socket al cual esta conectado

//Crea un socket de comunicacion para el cliente
void crear_socket_connect_cliente(socket_connect_t *skt_c);

//Crea un socket de comunicacion para el cliente
void crear_socket_connect_servidor(socket_connect_t *skt_c,int skt);

bool addrinfo_socket(socket_connect_t *skt_c,char *hostn, char* srvn);

bool connect_socket(socket_connect_t *skt_c);

//Recibe por parametro un puntero a socket_connect_t, un string y un int.
//Envia un mensaje al socket el cual esta conectado.
bool enviar_mensaje_socket(socket_connect_t *skt_c,char msg[],int tam);

bool recibir_mensaje_socket(socket_connect_t *skt_c,char msg[],int tam);

void destruir_socket(socket_connect_t *skt_c);

int get_largo(req_cliente_t *req);

#endif

