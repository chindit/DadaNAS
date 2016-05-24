#!/bin/bash

date=`date +%Y-%m-%d:%H:%M:%S`
has_ssh=`netstat -ant | grep LOCAL_IP:PORT | grep ESTABLISHED`
has_samba=`smbstatus | grep -c '[0-9]*\.[0-9]*\.[0-9]*\.[0-9]*'`
uptime=`uptime | awk '{print $3}'`

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
	    if [ $uptime -gt 10 ];
	    then
            	systemctl hibernate
	    fi
	fi
    fi
else
    echo $date " : No shutdown - SSH" >> "/tmp/log.txt"
fi

