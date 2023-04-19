#ifndef HTTP_CLIENT_H  
#define HTTP_CLIENT_H

typedef struct http_message
{
    char  * measurement;
    double value;

} http_message;


int send_message(char * host, char * auth, char * sensor, float value);


#endif