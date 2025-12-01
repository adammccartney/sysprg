#!/bin/sh

if [ "$#" -ne 1 ]; then
    echo "usage: ${BASH_SOURCE[0]} <location>"
    exit 1
fi
LOCATION="$1"

echo "running on ${LOCATION} - pipong will transfer a byte back and forth between two processes via a set of pipes..."

count=0
total=0
for sec in 1.0 2.0 4.0 8.0 16.0 32.0;
do
    run=$(./pipong A "$sec" | awk '{tot = $3 + $6} END {print tot / $9}' )
    total=$((total + run))
    count=$((count+1))
done


# Average
result=$((total / count))
printf "no of bytes transferred: %s (b/sec) \n" "$result"

