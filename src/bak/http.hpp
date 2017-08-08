#ifndef HTTP_HPP
#define HTTP_HPP

#include <string>
#include <map>
#include <json/json.h>
using namespace std;
#define MAX_HTTP_REQUEST_LENGTH    (1024 * 16)

typedef vector< map <string, string> >   httpOutput;
typedef map <string, string>            *g_pReq, mReq;
typedef map <string, httpOutput>        *g_pRes, mRes;

g_pReq g_req = new mReq;
g_pRes g_res = new mRes;

g_pReq HttpReadReq(void);



#endif
