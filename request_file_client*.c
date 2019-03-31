#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

void get_request_stdin(char msg[]){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the the request (<c> to continue): \n");
	char req[512];
	char buf[512];
	while (true) {
		if (fgets(req,512,stdin)==NULL) return;
		if (strcmp(req,"c\n") == 0) break;
		snprintf(buf,sizeof(char)*512,"%s%s",msg,req);
		memcpy(msg,buf,strlen(buf));
	}
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
