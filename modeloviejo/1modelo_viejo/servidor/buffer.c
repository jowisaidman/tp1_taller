#include "buffer.h"

void buffer_create(buffer_t *b,size_t s) {
	b->size = s;
	b->data = malloc(s* sizeof(char));
}

char* get_data(buffer_t *b){
	return 	b->data;
}

size_t get_buf_size(buffer_t *b){
	return 	b->buf_size;
}

bool set_data(buffer_t *b,char *data,size_t len) {
	if (len >= b->size) return false;
	memcpy(b->data, data, len);
	b->buf_size=len;
	return true;	
}

void buffer_destroy(buffer_t *b){
	if (b->data) free(b->data);
}

