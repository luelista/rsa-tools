#!/bin/sh

N=$1
GROUP_COUNT=$2
LOGFILE=$3
if [ -z $N ] ; then
	echo Missing parameter N
	exit 1
fi
if ! [ "$GROUP_COUNT" -gt 1 ] ; then
	echo Missing parameter GROUP_COUNT
	exit 1
fi

if [ -z $LOGFILE ] ; then
	LOGFILE=brutefor.log
fi

i=1
echo -n "Spawning $GROUP_COUNT processes ... "
while [ $i -le $GROUP_COUNT ]
do
	echo -n "$i "
	xterm -e ./brute_force "$N" "$GROUP_COUNT" "$i" "$LOTFILE" &
	i=`expr $i + 1`
done
echo "ok"



