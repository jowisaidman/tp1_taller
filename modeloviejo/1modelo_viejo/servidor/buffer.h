#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct buffer {
    char* data;  /*private*/
    size_t size; /*private*/
    size_t buf_size;
} buffer_t;

void buffer_create(buffer_t *b,size_t s);

char* get_data(buffer_t *b);

void buffer_destroy(buffer_t *b);

bool set_data(buffer_t *b,char *data,size_t len);

size_t get_buf_size(buffer_t *b);

#endif

