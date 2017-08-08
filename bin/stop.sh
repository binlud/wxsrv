#!/bin/sh

killall -9 wx-srv

if [ `echo $?` -eq 0 ]
then echo shut down ... && sleep 1s
fi

if [ `ps -fC wx-srv | wc -l` -gt 1 ] 
then echo "wx-srv: 模块停止失败" && exit 1
else echo "wx-srv: 模块已停止"
fi
 
exit 0
