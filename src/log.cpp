/*
 * log.cpp -- 日志处理模块
 *
 * 版权所有(c)，2017 - 2018 xx有限公司
 * 核心技术，如需分发请在法律许可范围。
 *
 * 本文件是通用日志处理模块
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/time.h>

#include<stdarg.h>
#include<time.h>
#include<sys/errno.h>
#include<ctype.h>
#include<unistd.h>
#include<sys/syscall.h>
#include"log.hpp"
//#include "cfg.hpp"
//#include "utils.hpp"
//#include "version.h"
//#include "backend.hpp"

static FILE *gLogFp = NULL;
static LOG_TYPE gLogLevel = INFO;

pid_t gettid()
{
    return syscall(SYS_gettid);
}

int strIcmp(const char* p1, const char* p2)
{
    while (*p1) {
        char c1 = *p1;
        char c2 = *p2;

        if (c1 >= 'A' && c1 <= 'Z') {
            c1 += 'a' - 'A';
        }
        if (c2 >= 'A' && c2 <= 'Z') {
            c2 += 'a' - 'A';
        }
        if (c1 != c2) {
            return c1 - c2;
        }
        p1++;
        p2++;
    }
    return *p1 - *p2;
}

bool initLog( string logPath, string logLevel )
{
    //char logPath[32];

    //if ( !isFastCGI() ) {
    if ( false ) {
        gLogFp = stderr;
        RUN_LOG( "日志模块初始化成功，目前不在 FastCGI 下运行。" );
    } else {
        //READ_CONFIG_STRING( logPath.c_str(), "log_path" ); //日志文件路径可以加到初始化函数参数中

        gLogFp = fopen( logPath.c_str(), "ab" );
        if ( gLogFp == NULL ) {
            ERR_LOG( "日志模块初始化失败，因为打开日志文件 %s 时遇到错误: [%d] %s。",
                     logPath.c_str(), errno, strerror(errno) );
            return false;
        }

        RUN_LOG( "日志模块初始化成功，正在 FastCGI 下运行。" );
    }
    //string logLevel = readConfig( "log_level" ); //可加到初始化函数接口中

    if ( !strIcmp("ERROR",logLevel.c_str()) ) { //猜测 大写转换为小写，待验证
        gLogLevel = ERROR;
    }
    // else if ( !strIcmp("WARN",logLevel.c_str())){
    //     gLogLevel = WARN;
    // }
    else if ( !strIcmp("INFO",logLevel.c_str())) {
        gLogLevel = INFO;
    } else if ( !strIcmp("DEBUG",logLevel.c_str())) {
        gLogLevel = DEBUG;
    } else if (!strIcmp("test",logLevel.c_str()) ) {
        gLogLevel = TEST;
    } else {
        gLogLevel = INFO;
    }
    return true;
}

void logMsg( LOG_TYPE type, const char *fmt, ... )
{
    va_list          args;
    //time_t           now;
    struct tm       *buf;
    struct timeval   tv;
    struct timezone  tz;

    if ( gLogFp == NULL ) {
        gLogFp = stderr;
        //return;
    }

    if ( type > gLogLevel ) {
        return;
    }

    //now = time( NULL );
    //buf = localtime( &now );

    // get time data
    gettimeofday( &tv, &tz );
    // formate time to struct
    buf = localtime( &tv.tv_sec );

    //省略1
    //fprintf( gLogFp, "TS"  " " "%s %04d-%02d-%02d %02d:%02d:%02d.%06ld %d %d ",
    fprintf( gLogFp, "%s %04d%02d%02d %02d:%02d:%02d.%06ld %d %d ",
             ( type == ERROR ? "ERR" : type == DEBUG ? "DBG" : type == INFO ? "RUN" : "---" ),
             buf->tm_year + 1900,
             buf->tm_mon + 1,
             buf->tm_mday,
             buf->tm_hour,
             buf->tm_min,
             buf->tm_sec,
             tv.tv_usec,
             getpid(),
             gettid());

    va_start( args, fmt );
    vfprintf( gLogFp, fmt, args );
    va_end( args );

    fflush( gLogFp );

    return;
}

/*
void logBuffer( LOG_TYPE type, const char *module, const char *buffer, size_t size, string logMsg, bool bMark )
{
    string dataString( buffer );
    static char spaceLine[49] = "                                                ";
    int lineWidth = 16;
    //time_t now;
    struct tm *buf;
    struct timeval   tv;
    struct timezone  tz;

    if ( gLogFp == NULL ) {
        return;
    }

    if ( type > gLogLevel ) {
        return;
    }

    if ( bMark ) {
        // 屏蔽敏感字段
        TradeBackend::markSensitiveField( dataString );
    }

    //now = time( NULL );
    //buf = localtime( &now );

    // get time data
    gettimeofday( &tv, &tz );
    // formate time to struct
    buf = localtime( &tv.tv_sec );

    size_t i=0;
    while(1) {
        fprintf( gLogFp, KDS_PROGRAM_NAME " " "%s %04d-%02d-%02d %02d:%02d:%02d.%06ld %d %d ",
                 ( type == ERROR ? "ERR" : type == DEBUG ? "DBG" : type == INFO ? "RUN" : "---" ),
                 buf->tm_year + 1900,
                 buf->tm_mon + 1,
                 buf->tm_mday,
                 buf->tm_hour,
                 buf->tm_min,
                 buf->tm_sec,
                 tv.tv_usec,
                 getpid(),
                 gettid());

        fprintf( gLogFp, "%s %06lx ", (string(module)+" - "+logMsg).c_str(), i );

        int j=0;
        for( j=0; i<dataString.length() && j<lineWidth; j++, i++ ) {
            fprintf( gLogFp, "%02x ", (unsigned char)dataString[i] );
        }

        fwrite( spaceLine, (lineWidth - j) * 3, 1, gLogFp );
        fprintf( gLogFp, "  " );

        i -= j;
        for( j=0; i<dataString.length() && j<lineWidth; j++, i++ ) {
            fprintf( gLogFp, "%c", isprint( dataString[i] ) ? dataString[i] : '.' );
        }

        fprintf( gLogFp, "\n" );

        if ( i>=size ) {
            break;
        }
    }
}
*/

#if 0
void logMap( LOG_TYPE type, const char *module, map <string, string> &hash, string strId, string logAppend )
{
    if ( gLogFp == NULL ) {
        return;
    }

    //logMsg( type, "%s {\n", module );
    //string log = logAppend + "\n{\n";
    string log = logAppend + "{";

    for( map <string, string>::iterator it = hash.begin(); it != hash.end(); ) {
        string strKey = it->first;
        string strVal = it->second;

        if ( TradeBackend::isSensitive(strKey, strId) ) {
            strVal = "******";
        }

        //logMsg( type, "%s     \"%s\": \"%s\",\n", module, strKey.c_str(), strVal.c_str() );
        //log += string( "    \"" ) + strKey.c_str() + "\": \"" + strVal.c_str() + "\",\n";
        log += string( "\"" ) + strKey.c_str() + "\":\"" + strVal.c_str() + "\"";

        ++it;
        if ( it != hash.end() ) {
            log += ",";
        }
    }

    log += "}";

    //logMsg( type, "%s }\n", module );
    logMsg( type, "%s - %s\n", module, log.c_str() );
}
#endif

#if 0
void logMaps( LOG_TYPE type, const char *module, vector< map<string, string> > &v, string strId, string logAppend )
{
    if ( gLogFp == NULL ) {
        return;
    }

    //string log = logAppend + "\n";
    string log = logAppend;

    for ( unsigned int i = 0; i < v.size(); i++ ) {
        //log += "{\n";
        log += "{";

        for( map <string, string>::iterator it = v[i].begin(); it != v[i].end(); ) {
            string strKey = it->first;
            string strVal = it->second;

            if ( TradeBackend::isSensitive(strKey, strId) ) {
                strVal = "******";
            }

            //log += string( "    \"" ) + strKey.c_str() + "\": \"" + strVal.c_str() + "\",\n";
            log += string( "\"" ) + strKey.c_str() + "\":\"" + strVal.c_str() + "\"";

            ++it;
            if ( it != v[i].end() ) {
                log += ",";
            }
        }

        log += "}";
    }

    logMsg( type, "%s - %s\n", module, log.c_str() );
}
#endif

# define SPACE_STRING " "
CommonLogArg g_logArgs;
static CommonLogArg *g_pLogArgs = &g_logArgs;

void CommonLogArg::setCommonLogArg( LOG_ARG args )
{
    if ( g_pLogArgs == NULL ) {
        return;
    }

    g_pLogArgs->logArg = args;
}

void CommonLogArg::setCommonLogArg( CommonLogArg com )
{
    if ( g_pLogArgs == NULL ) {
        return;
    }

    g_pLogArgs->logArg = com.logArg;
}

void CommonLogArg::setCommonLogArg( LOG_ARG_TYPE type, LOG_ARG args )
{
    if ( g_pLogArgs == NULL ) {
        return;
    }

    // SESSIONID = 1,                // CONTEXT 的内存地址，每个业务请求唯一，作为 SESSION
    // USER_UNIQID,                  // 用户唯一 ID，即 客户标识类型+客户标识
    // HTTP_REQENV,                  // HTTP 请求环境参数
    // REQUEST_URI,                  // 业务请求功能路径
    // CURRENT_ACTIONID,             // 当前柜台业务调用动作 ID
    // CURRENT_FUNCID,               // 当前柜台业务调用动作 ID 的柜台功能号
    // CURRENT_ACTIONTBID,           // 当前柜台业务调用动作 ID 的柜台 ID
    // REQ_STAR_TTIME,               // 接收到 HTTP 请求时的时间，微妙数
    // LOG_ARG_TYPE_COUNT,           // 类型总数
    switch ( type ) {
    case SESSIONID:
        g_pLogArgs->logArg.sessionid = args.sessionid;
        break;
    case USER_UNIQID:
        g_pLogArgs->logArg.userUniqId = args.userUniqId;
        break;
    case HTTP_REQENV:
        g_pLogArgs->logArg.httpReqEnv = args.httpReqEnv;
        break;
    case REQUEST_URI:
        g_pLogArgs->logArg.requestUri = args.requestUri;
        break;
    case CURRENT_ACTIONID:
        g_pLogArgs->logArg.currentActionId = args.currentActionId;
        break;
    case CURRENT_FUNCID:
        g_pLogArgs->logArg.currentFuncId = args.currentFuncId;
        break;
    case CURRENT_ACTIONTBID:
        g_pLogArgs->logArg.currentActionTbId = args.currentActionTbId;
        break;
    case REQ_STAR_TTIME:
        g_pLogArgs->logArg.reqStartTime = args.reqStartTime;
        break;
    default:
        ERR_LOG( "unknown logArgType, setting failed" );
    }

}

void CommonLogArg::setCommonLogArg( LOG_ARG_TYPE type, string value )
{
    if ( g_pLogArgs == NULL ) {
        return;
    }

    if ( value == " " || value == "" ) {
        DBG_LOG( "illegal value, ignore" );
        return;
    }

    // SESSIONID = 1,                // CONTEXT 的内存地址，每个业务请求唯一，作为 SESSION
    // USER_UNIQID,                  // 用户唯一 ID，即 客户标识类型+客户标识
    // HTTP_REQENV,                  // HTTP 请求环境参数
    // REQUEST_URI,                  // 业务请求功能路径
    // CURRENT_ACTIONID,             // 当前柜台业务调用动作 ID
    // CURRENT_FUNCID,               // 当前柜台业务调用动作 ID 的柜台功能号
    // CURRENT_ACTIONTBID,           // 当前柜台业务调用动作 ID 的柜台 ID
    // REQ_STAR_TTIME,               // 接收到 HTTP 请求时的时间，微妙数
    // LOG_ARG_TYPE_COUNT,           // 类型总数
    switch ( type ) {
    case SESSIONID:
        g_pLogArgs->logArg.sessionid =  value;
        break;
    case USER_UNIQID:
        g_pLogArgs->logArg.userUniqId = value;
        break;
    case HTTP_REQENV:
        g_pLogArgs->logArg.httpReqEnv = value;
        break;
    case REQUEST_URI:
        g_pLogArgs->logArg.requestUri = value;
        break;
    case CURRENT_ACTIONID:
        g_pLogArgs->logArg.currentActionId = value;
        break;
    case CURRENT_FUNCID:
        g_pLogArgs->logArg.currentFuncId = value;
        break;
    case CURRENT_ACTIONTBID:
        g_pLogArgs->logArg.currentActionTbId = value;
        break;
    case REQ_STAR_TTIME:
        g_pLogArgs->logArg.reqStartTime = atol( value.c_str() );
        break;
    default:
        ERR_LOG( "unknown logArgType, setting failed" );
    }

}

string CommonLogArg::getAllCommonLogMsg()
{
    string logMsg = "";
    long usedtime = 0;
    struct timeval t;
    long currentTime;
    char tmp[80] = {0};

    gettimeofday( &t, NULL );
    currentTime = 1000000*t.tv_sec + t.tv_usec;
    usedtime = currentTime - logArg.reqStartTime;
    sprintf( tmp, "%ld", usedtime/1000 );

    // 会话 ID
    logMsg += logArg.sessionid + SPACE_STRING;
    // 耗时
    logMsg += string( tmp ) + SPACE_STRING;
    // 用户唯一 ID
    logMsg += logArg.userUniqId + SPACE_STRING;
    // 业务 uri 功能路径
    logMsg += logArg.requestUri + SPACE_STRING;
    // 业务流程 ID
    logMsg += logArg.currentActionId + SPACE_STRING;
    // 柜台标识
    logMsg += logArg.currentActionTbId + SPACE_STRING;
    // 柜台接口功能号
    logMsg += logArg.currentFuncId + SPACE_STRING;

    return logMsg;
}

string CommonLogArg::getCommonLogMsg()
{
    string logMsg = "";
    long usedtime = 0;
    struct timeval t;
    long currentTime;
    char tmp[80] = {0};

    gettimeofday( &t, NULL );
    currentTime = 1000000*t.tv_sec + t.tv_usec;
    usedtime = currentTime - logArg.reqStartTime;
    sprintf( tmp, "%ld", usedtime/1000 );

    // 会话 ID
    logMsg += logArg.sessionid + SPACE_STRING;
    // 耗时
    logMsg += string( tmp ) + SPACE_STRING;
    // 用户唯一 ID
    logMsg += logArg.userUniqId + SPACE_STRING;
    // 业务 uri 功能路径
    logMsg += string("-") + SPACE_STRING;
    // 业务流程 ID
    logMsg += string("-") + SPACE_STRING;
    // 柜台标识
    logMsg += string("-") + SPACE_STRING;
    // 柜台接口功能号
    logMsg += string("-") + SPACE_STRING;

    return logMsg;
}

void setCommonLogArg( LOG_ARG args )
{
    g_logArgs.setCommonLogArg( args );
}

void setCommonLogArg( CommonLogArg com )
{
    g_logArgs.setCommonLogArg( com );
}

void setCommonLogArg( LOG_ARG_TYPE type, LOG_ARG args )
{
    g_logArgs.setCommonLogArg( type, args );
}

void setCommonLogArg( LOG_ARG_TYPE type, string value )
{
    g_logArgs.setCommonLogArg( type, value );
}

string getCommonLogMsg()
{
    return g_logArgs.getCommonLogMsg();
}

string getAllCommonLogMsg()
{
    return g_logArgs.getAllCommonLogMsg();
}

void initCommonLogMsg()
{
    // 初始化日志属性全局变量
    CommonLogArg comLogArg;
    setCommonLogArg( comLogArg );
}
