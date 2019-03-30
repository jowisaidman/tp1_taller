#ifndef REQUESTFILE_H
#define REQUESTFILE_H
#include <stdio.h>

void get_request_stdin(char req[]);

void read_file(FILE *fp,char line[]);

void get_request_param(char *filename,char req[]);

#endif

