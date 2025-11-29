#!/usr/bin/env bash

player1 () {
    while true
    do read n
        if "$n" == "ping"; then
            echo pong > fifo2
        fi
    done <<<$(cat fifo1)
}

player2 () {
    while true
    do read n
        if "$n" == "pong"; then
            echo ping > fifo1
        fi
    done <<<$(cat fifo2)
}

mkfifo fifo1
mkfifo fifo2

(echo "ping" > fifo1; (player1; player2))
