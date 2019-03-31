#ifndef SENSOR_H
#define SENSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

float get_sensor_temp(char *file_name,long int pos,bool *reach_end);

#endif

