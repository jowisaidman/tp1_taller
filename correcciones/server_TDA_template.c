#include "server_TDA_template.h"

void crear_template(template_t *template,const char* file_name) {
	FILE* fp = NULL;
	fp=fopen(file_name,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	fseek(fp,0,SEEK_END);
	int tam = ftell(fp)+2;	
	fclose(fp);
	template->contenido = calloc(tam,sizeof(char));
	template->contenido_parte1 = calloc(tam,sizeof(char));
	template->contenido_parte2 = calloc(tam,sizeof(char));
}

void read_file(FILE *fp,template_t *template) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		template->contenido[pos] = c;
		pos++;
	}
}

void parser_template(template_t *template) {
	char *ptr;
	int pos;
	int max = strlen(template->contenido);
	const char* remplaza = "{{datos}}";
	ptr=strstr(template->contenido,remplaza);
	pos = ptr - template->contenido;
	for (int i=0; i<pos; i++) {
		template->contenido_parte1[i]=template->contenido[i];
	}
	template->largo_parte1 = pos;
	pos+=strlen(remplaza);
	for (int i=0; i<max-pos; i++) {
		template->contenido_parte2[i]=template->contenido[i+pos];
	}
	template->largo_parte2 = max-pos;
}

void armar_template(template_t *template,char* file_name) {
	FILE* fp = NULL;
	fp=fopen(file_name,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	read_file(fp,template);
	parser_template(template);
	fclose(fp);
}

char* get_template_parte1(template_t *template) {
	return template->contenido_parte1;
}

char* get_template_parte2(template_t *template) {
	return template->contenido_parte2;
}

int get_largo_parte2(template_t *template) {
	return template->largo_parte2;
}

int get_largo_parte1(template_t *template) {
	return template->largo_parte1;
}

void destruir_template(template_t *template) {
	free(template->contenido);
	free(template->contenido_parte1);
	free(template->contenido_parte2);
}
	

	
