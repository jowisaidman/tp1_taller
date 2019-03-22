#include <string.h>
#include <stdio.h>
#include "libreria_aux.h"

void get_request_stdin(char req[]){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the path of the file: ");
	if (fgets(req,30,stdin)==NULL) return;
	req[strcspn ( req, "\n" )] = '\0';
}

void get_request_param(char *argv[],char req[]) {
	FILE* fp = NULL;
	fp=fopen(argv[3],"r");
	if ((fp==NULL)) {
		printf("ERROR\n");
		return;
	}
	read_file(fp,req);
	fclose(fp);	
}

void read_file(FILE *fp,char line[]) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		line[pos] = c;
		pos++;
	}
}
