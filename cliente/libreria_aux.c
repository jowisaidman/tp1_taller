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
	read_line(fp,req);	
}

char* read_line(FILE *fp,char line[]){
	int pos=0;
	char c;
	while((c = fgetc(fp)) != '\n') { //[1]se le deberia pasar un maximo de longitud?
		//if (pos>=1000) return NULL;[1]
		if (c==EOF) break;
		line[pos] = c;
		pos++;
	}
	return line;
}
