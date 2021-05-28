#!/bin/bash
# cleanall.sh: recurse directory tree and call make clean on each leaf

for d in ./*/*/
do
    (cd "$d" && make clean && $command)
done

exit 0;

