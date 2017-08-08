#!/bin/sh

export WX_SRC_HOME=..
$WX_SRC_HOME/bin/stop.sh

if [ `echo $?` -ne 0 ]
then echo "wx-srv: 重启失败" && exit 1
fi

$WX_SRC_HOME/bin/start.sh

if [ `echo $?` -ne 0 ]
then echo "wx-srv: 重启失败" && exit 1
else echo "wx-srv: 重启成功"
fi

exit 0
