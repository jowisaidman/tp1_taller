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


void crear_sensor_server(sensor_t *sensor);

int read_temp(sensor_t *sensor,char *nombre_arch, int posicion);

float get_sensor_temp(sensor_t *sensor,char *nombre_arch, int posicion);

int sensor_reach_end(sensor_t *sensor);

void destruir_sensor_server(sensor_t *sensor);

#endif

