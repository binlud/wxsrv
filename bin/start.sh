#!/bin/sh

export WX_SRC_HOME=..
../../3rd-srv/bin/spawn-fcgi -a 127.0.0.1 -p 8000 -F 1 -d $WX_SRC_HOME/bin -- $WX_SRC_HOME/bin/wx-srv
#../../3rd-srv/bin/spawn-fcgi -a 127.0.0.1 -p 8000 -f wx-srv

fcgi_status=$?
if [ "$fcgi_status" -eq 0 ]
then echo starting ... && sleep 3s
fi

if [ `ps -fC wx-srv | wc -l` -gt 1 ] && [ "$fcgi_status" -eq 0 ]
then echo "wx-srv: 模块启动成功" && exit 0
elif [ `ps -fC wx-srv | wc -l` -gt 1 ] && [ "$fcgi_status" -eq 255 ]
then echo "wx-srv: 服务已存在"
elif [ `ps -fC wx-srv | wc -l` -gt 1 ] && [ "$fcgi_status" -ne 0 ]
then echo "wx-srv: 服务已存在，本次启动失败，请检查启动脚本"
else
    echo "wx-srv: 模块启动失败"
fi

exit 1
