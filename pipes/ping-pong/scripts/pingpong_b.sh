#!/bin/sh

# proc B
while true;
do
    while IFS= read -r msg;
    do
        echo "$msg"
        if [ "$msg" == "ping" ]; then
            echo pong > ./p1
            sleep 1
        fi
    done <<<$(cat ./p2)
done
