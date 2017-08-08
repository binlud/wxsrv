#include <iostream>
#include <string>
#include <strings.h>
#include <stdlib.h>
#include <fcgi_stdio.h>
#include <fcgiapp.h>
#include "WXBizMsgCrypt.h"
#include "log.hpp"
#include "utils.hpp"
//#include "http.hpp"

//using namespace tinyxml2;
using namespace EncryptAndDecrypt;
using namespace std;

/*
*read me
*��ʾ������Ӧ���˿�Դ��openssl��tinyxml2�⣬�������32λ��64λ�ľ�̬������,
*ʾ��Makefile������������64λ�汾�ģ������Ի���Ϊ32λ�����Makefile������
*windows�����£���Ҫ�Լ���װ��

*openssl�İ汾����openssl-1.0.1h��http://www.openssl.org/
*tinyxml2�İ汾����tinyxml2-2.1.0,    https://github.com/leethomason/tinyxml2
*/

bool accept(unsigned int &req_count)
{
#if 0
    string sEncodingAesKey = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG";
    string sToken = "spamtest";
    string sAppid = "wx68d81e4f937bbd00";
    WXBizMsgCrypt oWXBizMsgCrypt(sToken,sEncodingAesKey,sAppid);

    //decrypt ����
    string sMsgSignature = "003fee52ecc56afb46c00b5c7721be87860ce785";
    string sTimestamp = "1410349438";
    string sNonce = "298025754";
    string sEncryptBase64 = "mfBCs65c67CeJw22u4VT2TD73q5H06+ocrAIxswCaeZ/d/Lw"
                            "0msSZFHY0teqgSYiI1zR2gD2DKrB3TIrmX/liNSDrGqS8jSI/"
                            "WPeKB5VPr7Ezr7gomZAyGCwJSgT1TRFWPfONGJMxuj2nk4faTu"
                            "spAuVIFQ6SHwZuJBZC7mcJp7Cgr9cUhATQWDbOPaE7ukZBTV2Yq"
                            "yzH+UI2AK+J1S47cE79k1RX8t0hcTz/O0hlK8DGXKnvYv88qKQcI"
                            "7z4iaajqHfRVZKBNyOODabs+It+ZfM3dWTeFcPgDbGtIEnpt/EDtu"
                            "uA/zMvtkaKdHdswPnVZQ+xdwbYr3ldGvfT8HlEYEgkgKaThxTFobVl"
                            "wzu2ZkXCjicbP3xdr15Iq48ObgzPpqYuZ3IEoyggZDKClquk0u0orMck4GTF/XyE8yGzc4=";

    cout<<"sEncryptBase64 size:"<<sEncryptBase64.size()<<endl;
    string sPostData= "<xml><ToUserName><![CDATA[toUser]]></ToUserName><Encrypt><![CDATA["
                      + sEncryptBase64
                      + "]]></Encrypt></xml>";
    string sMsg;
    int ret = oWXBizMsgCrypt.DecryptMsg(sMsgSignature,sTimestamp,sNonce,sPostData,sMsg);
    cout<<"DecryptMsg ret:"<<ret<<" size:"<<sMsg.size()<<" msg:"<<endl;
    cout<<sMsg<<endl<<endl<<endl;


    //encrypt ����
    string sToXml = "<xml><ToUserName><![CDATA[oia2TjjewbmiOUlr6X-1crbLOvLw]]></ToUserName>"
                    "<FromUserName><![CDATA[gh_7f083739789a]]></FromUserName>"
                    "<CreateTime>1407743423</CreateTime>"
                    "<MsgType><![CDATA[video]]></MsgType>"
                    "<Video>"
                    "<MediaId><![CDATA[eYJ1MbwPRJtOvIEabaxHs7TX2D-HV71s79GUxqdUkjm6Gs2Ed1KF3ulAOA9H1xG0]]></MediaId>"
                    "<Title><![CDATA[testCallBackReplyVideo]]></Title>"
                    "<Description><![CDATA[testCallBackReplyVideo]]></Description>"
                    "</Video></xml>";

    string sEncryptMsg;
    ret = oWXBizMsgCrypt.EncryptMsg(sToXml,sTimestamp,sNonce,sEncryptMsg);
    cout<<"EncryptMsg ret:"<<ret<<" size:"<<sEncryptMsg.size()<<" msg:"<<endl;
    cout<<sEncryptMsg<<endl;

#endif
    while(FCGI_Accept() >= 0) {

        char *p = NULL;
        char *pData = NULL;
        char *pRequestMethod = NULL;
        char cBuf[1024] = {0};
        pRequestMethod = getenv("REQUEST_METHOD");  //��ȡ����ʽ
        p = getenv("CONTENT_LENGTH");     //�ӻ�������CONTENT_LENGTH�еõ����ݳ���
        int len = 0;
        len = strtol(p, NULL, 10);

        SYSERR_LOG("REQUEST_METHOD = [%s]", pRequestMethod);
        SYSERR_LOG("CONTENT_LENGTH = [%s]", p);

        if (strcasecmp(pRequestMethod,"POST")==0) {
            FCGI_fread(cBuf, len, 1, stdin );
            //string tmp = "{\"fa\":\" OK the method is POST!\"}";
            SYSERR_LOG("req = [%s]", cBuf);
            FCGI_printf( "Status: 200 OK\r\n"
                            "Content-Type: application/json; charset=utf-8\r\n\r\n"
                                    "%s", cBuf );
        }

        if (strcasecmp(pRequestMethod,"GET")==0) {
            FCGI_printf("<p>OK the method is GET!\n</p>");
            pData = getenv("QUERY_STRING");     //�ӻ�������QUERY_STRING�еõ�Form����
            SYSERR_LOG("DATA = [%s]", pData);

            //��ȡ��������
            map<string, string> res;
            const string req = pData;
            jxGetData(req, res );
            string key = "";
            string value = "";
            for(map<string, string>::iterator it = res.begin(); it != res.end(); ++it )
            {
                SYSERR_LOG("[%s] = [%s]", it->first.c_str(),it->second.c_str());
            }

            //��Ӧ�ͻ���
            //΢�����Ľ���
            string sEncodingAesKey = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFG";
            string sToken = "binlud";
            string sAppid = "wx68d81e4f937bbd00";
            WXBizMsgCrypt oWXBizMsgCrypt(sToken,sEncodingAesKey,sAppid);
            string sEncryptMsg = "";
            if( 0 == oWXBizMsgCrypt.ValidateSignature(res["signature"],res["timestamp"],res["nonce"],sEncryptMsg) )
            {

                SYSERR_LOG("Verify success");
                FCGI_printf("Content-type: text/html charset=utf-8\r\n\r\n");
                FCGI_printf(res["echostr"].c_str());

                return 0;
            }

            SYSERR_LOG("Validation failure");
            //FCGI_printf("%s", res["echostr"].c_str());
            //FCGI_printf("Content-type: text/html charset=utf-8\r\n\r\n");
            //FCGI_printf("<title>Hello World!</title>"
            //        "<h1>Hello ���!</h1>");
            //FCGI_printf("<h2>request times: %u</h2>", ++req_count);
            //FCGI_printf("QUERY_STRING: %s</br>", getenv("QUERY_STRING"));
            //FCGI_printf("REMOTE_ADDR: %s</br>", getenv("REMOTE_ADDR"));
        }

    }


    return 0;
}

int main()
{
    unsigned int req_count = 0;
    initLog( "../log/wx-srv.log", "test" );
    SYSERR_LOG("init seccess ok");

    accept(req_count);

    return 0;
}
