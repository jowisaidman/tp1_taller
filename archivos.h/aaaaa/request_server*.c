#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct request {
    void* action;
    void* resourse;
    void* http_p;
    void* header;
} request_t;

request_t* request_crear() {
    request_t* req = malloc(sizeof(request_t));   
    if (req == NULL) {   
        return NULL;
    }
    req->action = NULL;
    req->http_p = NULL;
    req->header = NULL;
    req->resourse = NULL;
    return req;
}	

void request_destruir(request_t* req) {
	if (req->action) free(req->action);
	if (req->http_p) free(req->http_p);
	if (req->header) free(req->header);
	if (req->resourse) free(req->resourse);
	free(req);
}

bool set_action(request_t* req,char action[]) {
	if (action == NULL) return false;
	req->action = (char*)malloc(sizeof(char)*(strlen(action)+1));
	if (req->action == NULL) return false;
	snprintf(req->action,strlen(action)+1,"%s",action);
	return true;
}

bool set_http_p(request_t* req,char http_p[]) {
	if (http_p == NULL) return false;
	req->http_p = (char*)malloc(sizeof(char)*(strlen(http_p)+1));
	if (req->http_p == NULL) return false;
	snprintf(req->http_p,strlen(http_p)+1,"%s",http_p);
	return true;
}

bool set_user_agent(request_t* req,char header[]) {
	if (header == NULL) return false;
	req->header = (char*)malloc(sizeof(char)*(strlen(header)+1));
	if (req->header == NULL) return false;
	snprintf(req->header,strlen(header)+1,"%s",header);
	return true;
}

bool set_resourse(request_t* req,char resourse[]) {
	if (resourse == NULL) return false;
	req->resourse = (char*)malloc(sizeof(char)*(strlen(resourse)+1));
	if (req->resourse == NULL) return false;
	snprintf(req->resourse,strlen(resourse)+1,"%s",resourse);
	return true;
}

void* get_action(request_t* req) {
	return req->action;	
}

void* get_user_agent(request_t* req) {
	return req->header;	
}

void* get_http_p(request_t* req) {
	return req->http_p;	
}

void* get_resourse(request_t* req) {
	return req->resourse;	
}

void parser(char msg[],request_t* req) {
	if (msg == NULL) return;
	char *delim = " \n\r:";
	char *token;
	int pos = 0;
    while ((token = strtok_r(msg, delim,&msg))){
        if (pos == 0) set_action(req,token);
        if (pos == 1) set_resourse(req,token);
        if (pos == 2) set_http_p(req,token);
        if (strcmp((const char*)token,"User-Agent")==0) {
			token = strtok_r(msg, delim,&msg);
			set_user_agent(req,token);
		}
        pos++;
    }
}

int request_is_valid(request_t* req) {
	const char* action = (const char*)get_action(req);
	if (strcmp(action,"GET") != 0) return 400;
	const char* resource = (const char*)get_resourse(req);
	if (strcmp(resource,"/sensor") != 0) return 404;
	const char* http_p = (const char*)get_http_p(req);
	if (strcmp(http_p,"HTTP/1.1") != 0) return 0;
	return 1;
}

