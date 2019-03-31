#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

int read_temp(char *file_name,long int pos,bool *reach_end) {
	FILE *fp;
	uint16_t s1;
	if ((fp = fopen(file_name,"rb")) == NULL){
		printf("Error! opening file");
		return 0;
	}
	fseek(fp,pos+2,SEEK_SET); 
	if (!fread(&s1, 2, 1, fp)) {
		*reach_end=true;
		fseek(fp,pos,SEEK_SET);
		if(!fread(&s1, 2, 1, fp)) return 0; /*archivo vacio,asumi que devuelve 0*/
	} else {
		fseek(fp,pos,SEEK_SET);
		if(!fread(&s1, 2, 1, fp)) return 0;  
	}
	s1 = htons(s1);
	fclose(fp);
	return s1;
}


float get_sensor_temp(char *file_name,long int pos,bool *reach_end) {
	int number_read = read_temp(file_name,pos,reach_end);
	float temp = ((number_read-2000.0f)/100.0f);
	return temp;
}

