#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void get_request_stdin(char req[]){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the path of the request: ");
	if (fgets(req,30,stdin)==NULL) return;
}

void read_file(FILE *fp,char line[]) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		line[pos] = c;
		pos++;
	}
}

void get_request_param(char *filename,char req[]) {
	FILE* fp = NULL;
	fp=fopen(filename,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	read_file(fp,req);
	fclose(fp);	
}
