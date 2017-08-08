# ifndef __KDS_MOBILE_STOCK_TRADE_SERVICE_LOG_H__
# define __KDS_MOBILE_STOCK_TRADE_SERVICE_LOG_H__

# include <string>
# include <map>
# include <vector>

using namespace std;

bool initLog( string logPath, string logLevel );

typedef enum tagLogType {
    ERROR = 1,
    INFO,
    DEBUG,
    TEST,
} LOG_TYPE;

void logMsg( LOG_TYPE type, const char *fmt, ... );
//void logBuffer( LOG_TYPE type, const char *module, const char *buffer, size_t size, string logMsg="", bool bMark=false );
void logMap( LOG_TYPE type, const char *module, map <string, string> &hash, string strId="", string logMsg="" );
void logMaps( LOG_TYPE type, const char *module, vector< map<string, string> > &v, string strId="", string logMsg="" );

typedef struct tagLogArg {
    string sessionid;             // context 的内存地址，每个业务请求唯一，作为 session
    string userUniqId;            // 用户唯一 ID，即 客户标识类型+客户标识
    string httpReqEnv;            // http 请求环境参数
    string requestUri;            // 业务请求功能路径
    string currentActionId;       // 当前柜台业务调用动作 ID
    string currentFuncId;         // 当前柜台业务调用动作 ID 的柜台功能号
    string currentActionTbId;     // 当前柜台业务调用动作 ID 的柜台 ID
    long   reqStartTime;          // 接收到 http 请求时的时间，微妙数
} LOG_ARG;

typedef enum tagLogArgType {
    SESSIONID = 1,                // CONTEXT 的内存地址，每个业务请求唯一，作为 SESSION
    USER_UNIQID,                  // 用户唯一 ID，即 客户标识类型+客户标识
    HTTP_REQENV,                  // HTTP 请求环境参数
    REQUEST_URI,                  // 业务请求功能路径
    CURRENT_ACTIONID,             // 当前柜台业务调用动作 ID
    CURRENT_FUNCID,               // 当前柜台业务调用动作 ID 的柜台功能号
    CURRENT_ACTIONTBID,           // 当前柜台业务调用动作 ID 的柜台 ID
    REQ_STAR_TTIME,               // 接收到 HTTP 请求时的时间，微妙数
    LOG_ARG_TYPE_COUNT,           // 类型总数
} LOG_ARG_TYPE;

class CommonLogArg;
class CommonLogArg
{
public:
    // 类成员

protected:
    LOG_ARG logArg;

public:
    // 类方法
    CommonLogArg()
    {
        logArg.sessionid         = "-";    // context 的内存地址，每个业务请求唯一，作为 session
        logArg.userUniqId        = "-";    // 用户唯一 ID，即 客户标识类型+客户标识
        logArg.httpReqEnv        = "-";    // http 请求环境参数
        logArg.requestUri        = "-";    // 业务请求功能路径
        logArg.currentActionId   = "-";    // 当前柜台业务调用动作 ID
        logArg.currentFuncId     = "-";    // 当前柜台业务调用动作 ID 的柜台功能号
        logArg.currentActionTbId = "-";    // 当前柜台业务调用动作 ID 的柜台 ID
        logArg.reqStartTime      = 0;      // 接收到 http 请求时的时间，微妙数
    }

    void setCommonLogArg( LOG_ARG args );
    void setCommonLogArg( CommonLogArg com );
    void setCommonLogArg( LOG_ARG_TYPE type, LOG_ARG args );
    void setCommonLogArg( LOG_ARG_TYPE type, string value );
    string getCommonLogMsg();
    string getAllCommonLogMsg();
};

void initCommonLogMsg();
void setCommonLogArg( LOG_ARG args );
void setCommonLogArg( CommonLogArg com );
void setCommonLogArg( LOG_ARG_TYPE type, LOG_ARG args );
void setCommonLogArg( LOG_ARG_TYPE type, string value );
string getCommonLogMsg();
string getAllCommonLogMsg();

# ifndef LOG_MODULE
# define LOG_MODULE "LOG "
# endif

# define SYSERR_LOG( fmt, args... ) \
    ERR_LOG( fmt, ##args )
    //缩写2.
    //ERR_LOG( "SYSTEM %s"fmt, getAllCommonLogMsg().c_str(), ##args )

# define SYSDBG_LOG( fmt, args... ) \
    DBG_LOG( "SYSTEM %s"fmt, getCommonLogMsg().c_str(), ##args )

# define SYSTEST_LOG( fmt, args... ) \
    TEST_LOG( "SYSTEM %s"fmt, getCommonLogMsg().c_str(), ##args )

# define CHKERR_LOG( fmt, args... ) \
    ERR_LOG( "CHECK  %s"fmt, getAllCommonLogMsg().c_str(), ##args )

# define CHKRUN_LOG( fmt, args... ) \
    RUN_LOG( "CHECK  %s"fmt, getCommonLogMsg().c_str(), ##args )

# define CLIREQ_LOG( fmt, args... ) \
    RUN_LOG( "CLIREQ %s"fmt, getAllCommonLogMsg().c_str(), ##args )

# define CLIRES_LOG( fmt, args... ) \
    RUN_LOG( "CLIRES %s"fmt, getAllCommonLogMsg().c_str(), ##args )

# define RECVERR_LOG( fmt, args... ) \
    ERR_LOG( "RECVBD %s"fmt, getAllCommonLogMsg().c_str(), ##args )

# define ANALYS_LOG( fmt, args... ) \
    RUN_LOG( "ANALYS %s"fmt, getCommonLogMsg().c_str(), ##args )

# define ERR_LOG( fmt, args... ) \
    logMsg( ERROR, "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )
    //logMsg( ERROR, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )

# define DBG_LOG( fmt, args... ) \
    logMsg( DEBUG, LOG_MODULE " - " fmt "\n", ##args )
//logMsg( DEBUG, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )

# define TEST_LOG( fmt, args... ) \
    logMsg( TEST, LOG_MODULE " - " fmt "\n", ##args )
//logMsg( TEST, LOG_MODULE " " "%s:%d:%s: "fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args )

# define RUN_LOG( fmt, args... ) \
    logMsg( INFO, LOG_MODULE " - " fmt "\n", ##args )

# endif
