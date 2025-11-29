#!/bin/bash

PID=""

while inotifywait -q ./pipong.c; do
    echo -e "\n\n"

    # If a previous instance is running, kill it
    if [[ -n "$PID" ]] && kill -0 "$PID" 2>/dev/null; then
        echo "Killing running instance ($PID)…"
        kill "$PID"
        wait "$PID" 2>/dev/null
    fi

    echo "Starting new instance…"
    make clean && make
    ./pipong A &
    PID=$!
done

