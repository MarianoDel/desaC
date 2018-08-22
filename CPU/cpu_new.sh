#!/bin/bash

#que proceso?
PROCESS="asterisk"

/bin/echo `/bin/date`>> /tmp/cronerrors
echo "el proceso $PROCESS" >> /tmp/cronerrors

#Busco el pid del proceso
PROCESSPID=`/bin/ps ax |/bin/grep $PROCESS -m 1|/bin/awk '{ print $1 }'`
echo "con el pid $PROCESSPID" >> /tmp/cronerrors

#Busco el uso de cpu
CPUSAGE=`/usr/bin/top -b -n 1 -p $PROCESSPID |/bin/grep $PROCESS |/bin/awk '{print $9}'`
echo "uso del cpu: $CPUSAGE" >> /tmp/cronerrors

#cp /var/log/asterisk/messages /tmp/messages000
#cp /var/log/asterisk/errors /tmp/errors000

COND=`echo $CPUSAGE'>'30 | bc -l`
if [ $COND -eq 1 ]
#if [ "$CPUSAGE" -lt "30" ]
then
	#kill -9 $PROCESSPID
	#sleep 5	
	#/usr/sbin/asterisk
	echo "rebooting" >> /tmp/cronerrors
fi

echo `date`> /var/log/asterisk/messages
echo `date`> /var/log/asterisk/errors

