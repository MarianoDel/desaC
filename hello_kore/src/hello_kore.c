#include <kore/kore.h>
#include <kore/http.h>


// Routes prototipes -----------------------------------------------------------
int page(struct http_request *);
int login_page(struct http_request *);


// Routes definitions ----------------------------------------------------------
int page(struct http_request *req)
{
	http_response(req, 200, NULL, 0);
	return (KORE_RESULT_OK);
}


int login_page(struct http_request *req)
{
    char sended [] = {"HELLO FIRST PAGE!"};


    kore_log(LOG_NOTICE, "login called");
    http_response_header(req, "content-type", "text/plain");
    http_response(req, 200, sended, sizeof(sended) - 1);
    
    return (KORE_RESULT_OK);
}
