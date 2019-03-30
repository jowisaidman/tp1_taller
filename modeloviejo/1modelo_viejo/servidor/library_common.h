#ifndef LIBRERIA_COMUN_H

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
