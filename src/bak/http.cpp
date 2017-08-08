#include "http.hpp"

char g_szBuffer[MAX_HTTP_REQUEST_LENGTH] = {0};
g_pReq readRequest( void )
{

    Json::Features features;
    Json::Reader reader(features);
    Json::Value json;
    //FCGI_fread(cBuf, len, 1, stdin );
}
