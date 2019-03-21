#include <string.h>
#include <stdio.h>
#include "libreria_aux.h"

void get_request(char req[]){
	/*Recibe como parametro una cadena y modifica la misma poniendo
	 * en ella un request del usuario dado por entrada estandar.*/
	printf("Enter the path of the file: ");
	if (fgets(req,30,stdin)==NULL) return;
	req[strcspn ( req, "\n" )] = '\0';
}
