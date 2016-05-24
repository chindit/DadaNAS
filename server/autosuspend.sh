#!/bin/bash

has_ssh=`netstat -ant | grep LOCAL_IP:PORT | grep ESTABLISHED`
has_samba=`smbstatus | grep -c '[0-9]*\.[0-9]*\.[0-9]*\.[0-9]*'`
uptime=`uptime -s`
dateCal=`date --date "$uptime" +%s`
date=`date +%s`

if [ "x$has_ssh" = "x" ];
then
    has_file='/srv/http/tmp/noshutdown'
    if [ -e $has_file ];
    then
        echo $date " : No shutdown - FILE" >> "/tmp/log.txt"
    else
        if [ $has_samba -gt 0 ];
        then
            echo $date " : No shutdown - SHARE" >> "/tmp/log.txt"
        else
            if [ $date -gt $(( $dateCal + 600 )) ];
            then
                systemctl hibernate
            fi
        fi
    fi
else
    echo $date " : No shutdown - SSH" >> "/tmp/log.txt"
fi

