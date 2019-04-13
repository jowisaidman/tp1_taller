#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <arpa/inet.h>

typedef struct sensor {
    int *esta_al_final;
} sensor_t;

//Crea un tda sensor
void crear_sensor_server(sensor_t *sensor);

//Recibe por parametro un puntero a un sensor_t un string y 
//la posicion sobre la cual debe leer un archivo y devuelve 
//el numero que se  encuentra en dicha posicion
int read_temp(sensor_t *sensor,char *nombre_arch, int posicion);

float get_sensor_temp(sensor_t *sensor,char *nombre_arch, int posicion);

//Indica si el sensor no tiene mas temperaturas posibles
int sensor_reach_end(sensor_t *sensor);

void destruir_sensor_server(sensor_t *sensor);

#endif

