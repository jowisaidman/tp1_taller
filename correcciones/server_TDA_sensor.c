#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>
#include "server_TDA_sensor.h"

void crear_sensor_server(sensor_t *sensor) { 
	sensor->esta_al_final = (int*)calloc(1, sizeof(int));
	*sensor->esta_al_final = 0;
}

int read_temp(sensor_t *sensor,char *nombre_arch, int posicion) {
	FILE *fp;
	uint16_t s1;
	if ((fp = fopen(nombre_arch,"rb")) == NULL){
		printf("Error! opening file");
		return 0;
	}
	fseek(fp,posicion+2,SEEK_SET); 
	if (!fread(&s1, 2, 1, fp)) {
		*sensor->esta_al_final=1;
		fseek(fp,posicion,SEEK_SET);
		if(!fread(&s1, 2, 1, fp)) return 0; /*archivo vacio,asumi que devuelve 0*/
	} else {
		fseek(fp,posicion,SEEK_SET);
		if(!fread(&s1, 2, 1, fp)) return 0;  
	}
	s1 = htons(s1);
	fclose(fp);
	return s1;
}

float get_sensor_temp(sensor_t *sensor,char *nombre_arch, int posicion) {
	int number_read = read_temp(sensor,nombre_arch,posicion);
	float temp = ((number_read-2000.0f)/100.0f);
	return temp;
}

int sensor_reach_end(sensor_t *sensor) {
	return *sensor->esta_al_final;	
}

void destruir_sensor_server(sensor_t *sensor){
	free(sensor->esta_al_final);
}








