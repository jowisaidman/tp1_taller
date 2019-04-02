#ifndef SOCKET_ACCEPT
#define SOCKET_ACCEPT

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "server_TDA_socket_connect.h"

typedef struct socket_accept {
    int s;
    int skt;
    int opt;
    struct addrinfo hints;
    struct addrinfo *ptr;
} socket_accept_t;

void crear_socket_accept(socket_accept_t *skt_s);

bool addrinfo_socket_accept(socket_accept_t *skt_s,char *srvn);

bool skt_socket_accept(socket_accept_t *skt_s);

bool skt_opciones_accept(socket_accept_t *skt_s);

bool bind_socket_accept(socket_accept_t *skt_s);

bool listen_socket_accept(socket_accept_t *skt_s);

bool accept_socket(socket_accept_t *skt_s,socket_connect_t *skt_c);

int cerrar_socket_accept(socket_accept_t *skt_s);

void destruir_socket_accept(socket_accept_t *skt_s);

#endif

