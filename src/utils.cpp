
#include "utils.hpp"

/*
bool jxGetData(const char &src, map<string, string> &res )
{
    unsigned pos1 = 0;
    unsigned pos2 = 0;
    unsigned pos3 = 0;
    string key        = "";
    string value      = "";
    SYSERR_LOG("src = [%s]", src);
    SYSERR_LOG("src.size = [%ld]", sizeof(src));
    for(; ;){
        SYSERR_LOG("pos1 = [%ld]", pos1);
        if( (pos2=src.find("=",pos1,1))!=string::npos ) {
            //1. 取key
            SYSERR_LOG("pos2 = [%ld]", pos2);
            key.assign(src.c_str(), pos1, pos2-pos1);

            if( (pos3=src.find("&", pos2, 1)) != string::npos ) {
                //2. 取value
                SYSERR_LOG("pos3 = [%ld]", pos3);
                value.assign(src.c_str(),pos2+1,pos3-pos2-1);
                res.insert(make_pair(key,value));
                pos1 = ++pos3;
            } else {
                SYSERR_LOG("end = [%ld]", pos3);
                value.assign(src.c_str(), pos2+1,src.size()-pos2-1);
                pos3 = src.size();
            }

        }
    }
    return true;
}
*/

bool jxGetData(const string &src, map<string, string> &res )
{
    size_t pos1 = 0;
    size_t pos2 = 0;
    size_t pos3 = 0;
    string key        = "";
    string value      = "";
    SYSERR_LOG("src = [%s]", src.c_str());
    SYSERR_LOG("src.size = [%ld]", src.size());
    for(; pos3 != string::npos;){

        SYSERR_LOG("pos1 = [%ld]", pos1);
        if( (pos2=src.find("=",pos1,1))!=string::npos ) {
            //1. 取key
            SYSERR_LOG("pos2 = [%ld]", pos2);
            key.assign(src.c_str(), pos1, pos2-pos1);

            if( (pos3=src.find("&", pos2, 1)) != string::npos ) {
                //2. 取value
                SYSERR_LOG("pos3 = [%ld]", pos3);
                value.assign(src.c_str(),pos2+1,pos3-pos2-1);
                res.insert(make_pair(key,value));
                if( pos3 == src.size()-1)
                    pos3 = -1;
                else
                    pos1 = ++pos3;
            } else {
                SYSERR_LOG("end = [%ld]", pos3);
                value.assign(src.c_str(), pos2+1,src.size()-pos2-1);
                res.insert(make_pair(key,value));
            }

        } else {
            key.assign(src.c_str(), pos1, src.size()-pos1-1);
            //value = "";
            res.insert(make_pair(key,""));
            pos3 = -1;
        }
    }
    return true;
}
