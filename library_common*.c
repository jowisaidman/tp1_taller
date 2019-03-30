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

int recv_message(int skt, char *buf, int size) {
	int received = 0;
	int s = 0;
	bool is_the_socket_valid = true;
	while (received < size && is_the_socket_valid) {
		s = recv(skt, &buf[received], size-received, MSG_NOSIGNAL);
		if (s == 0) {
			is_the_socket_valid = false;
		}
		if (s < 0) {
			is_the_socket_valid = false;
		}
		else {
			received += s;
		}
	}
	if (is_the_socket_valid) {
		return received;
	}
	else {
		return -1;
	}
}

int send_message(int skt, char *buf, int size) {
	int sent = 0;
	int s = 0;
	bool is_the_socket_valid = true;

	while (sent < size && is_the_socket_valid) {
		s = send(skt, &buf[sent], size-sent, MSG_NOSIGNAL);
      
		if (s == 0) {
			is_the_socket_valid = false;
		}
		else if (s < 0) {
			is_the_socket_valid = false;
		}
		else {
			sent += s;
		}
	}

	if (is_the_socket_valid) {
		return sent;
	}
	else {
		return -1;
	}
}

