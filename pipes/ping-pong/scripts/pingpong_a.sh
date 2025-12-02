#!/bin/sh

# two named pipes
if [ ! -f p1 ]; then
    mkfifo p1
fi
if [ ! -f p2 ]; then
    mkfifo p2
fi

# proc A
while true;
do
    while IFS= read -r msg;
    do
        echo "$msg"
        if [ "$msg" == "pong" ]; then
            echo ping > ./p2
            sleep 1
        fi
    done <<<$(cat ./p1)
done
