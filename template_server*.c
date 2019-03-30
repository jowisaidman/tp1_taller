#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void read_file(FILE *fp,char buf[]) {
	char c;
	int pos=0;
	while( (c=fgetc(fp))!=EOF ) {
		buf[pos] = c;
		pos++;
		if (pos>=1024) break;
	}
}

void parser_template(char buf[],float temp) {
	if (buf == NULL) return;
	char *str1 = calloc(strlen(buf),sizeof(char));
	char *str2 = calloc(strlen(buf),sizeof(char));
    char *ptr;
    int pos;
    int max = strlen(buf);
    ptr=strstr(buf,"{{datos}}");
    pos = ptr - buf;
    for (int i=0; i<pos; i++) {
		str1[i]=buf[i];
	}
	pos+=9;
	for (int i=0; i<max-pos; i++) {
		str2[i]=buf[i+pos];
	}
	if(!sprintf(buf,"%s%.2f%s",str1,temp,str2)) printf("Error");
	free(str1);
	free(str2);
}

void get_template(char *file_name,float temp,char buf[]) {
	FILE* fp = NULL;
	fp=fopen(file_name,"r");
	if ((fp==NULL)) {
		printf("Error: the request could not be open\n");
		return;
	}
	read_file(fp,buf);
	parser_template(buf,temp);
	fclose(fp);
}

