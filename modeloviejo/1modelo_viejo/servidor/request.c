#include "request.h"

request_t* request_crear() {
    request_t* req = malloc(sizeof(request_t));   
    if (req == NULL) {   
        return NULL;
    }
    req->action = NULL;
    req->http_p = NULL;
    req->header = NULL;
    req->resourse = NULL;
    req->host = NULL;
    return req;
}	

void request_destruir(request_t* req) {
	if (req->action) free(req->action);
	if (req->http_p) free(req->http_p);
	if (req->header) free(req->header);
	if (req->resourse) free(req->resourse);
	if (req->host) free(req->host);
	free(req);
}

bool set_action(request_t* req,char action[]) {
	if (action == NULL) return false;
	req->action = (char*)malloc(sizeof(char)*(strlen(action)+1));
	if (req->action == NULL) return false;
	strcpy(req->action,action);
	return true;
}

bool set_http_p(request_t* req,char http_p[]) {
	if (http_p == NULL) return false;
	req->http_p = (char*)malloc(sizeof(char)*(strlen(http_p)+1));
	if (req->http_p == NULL) return false;
	strcpy(req->http_p,http_p);
	return true;
}

bool set_user_agent(request_t* req,char header[]) {
	if (header == NULL) return false;
	req->header = (char*)malloc(sizeof(char)*(strlen(header)+1));
	if (req->header == NULL) return false;
	strcpy(req->header,header);
	return true;
}

bool set_resourse(request_t* req,char resourse[]) {
	if (resourse == NULL) return false;
	req->resourse = (char*)malloc(sizeof(char)*(strlen(resourse)+1));
	if (req->resourse == NULL) return false;
	strcpy(req->resourse,resourse);
	return true;
}

bool set_host(request_t* req,char host[]) {
	if (host == NULL) return false;
	req->host = (char*)malloc(sizeof(char)*(strlen(host)+1));
	if (req->host == NULL) return false;
	strcpy(req->host,host);
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

void* get_host(request_t* req) {
	return req->host;	
}

