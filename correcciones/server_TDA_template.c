#define MAX_TEMP_LEN 1024

#include "server_TDA_template.h"

void crear_template(template_t *template,float temp) {
	template->contenido = calloc(MAX_TEMP_LEN,sizeof(char));
	template->temp = (float*)calloc(1, sizeof(int));
	*template->temp = temp;
}

void read_file(FILE *fp,template_t *template) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		template->contenido[pos] = c;
		pos++;
		if (pos>=MAX_TEMP_LEN) break;
	}
}

void parser_template(template_t *template) {
	char *str1 = calloc(strlen(template->contenido),sizeof(char));
	char *str2 = calloc(strlen(template->contenido),sizeof(char));
    char *ptr;
    int pos;
    int max = strlen(template->contenido);
    const char* remplaza = "{{datos}}";
    ptr=strstr(template->contenido,remplaza);
    pos = ptr - template->contenido;
    for (int i=0; i<pos; i++) {
		str1[i]=template->contenido[i];
	}
	pos+=strlen(remplaza);
	for (int i=0; i<max-pos; i++) {
		str2[i]=template->contenido[i+pos];
	}
	if(!snprintf(template->contenido,sizeof(char)*1024,"%s%.2f%s",
	str1,*template->temp,str2)) printf("Error");
	free(str1);
	free(str2);
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

void destruir_template(template_t *template) {
	free(template->contenido);
	free(template->temp);
}

char* get_template(template_t *template) {
	return template->contenido;	
}
	
