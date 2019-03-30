#ifndef LIBRERIA_COMUN_H
#define LIBRERIA_COMUN_H
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

int recv_message(int skt, char *buf, int size);

int send_message(int skt, char *buf, int size);

#endif
