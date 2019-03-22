#include <stdio.h>
#include <string.h>

int main(){
    char cadena[] = "GET /sensor HTTP/1.1\n\rUser-Agent: sensor-client",
    delimitador[] = "\n\r ";
    char *token = strtok(cadena, delimitador);
    if(token != NULL){
        while(token != NULL){
            // Sólo en la primera pasamos la cadena; en las siguientes pasamos NULL
            printf("Token: %s\n", token);
            token = strtok(NULL, delimitador);
        }
    }
}
