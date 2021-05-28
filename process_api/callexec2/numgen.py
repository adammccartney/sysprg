#!/bin/python3

# numgen.py: generate <numvals> unsigned 32 bit ints, write to <outputfile>


import sys
import random
import math
from argparse import ArgumentParser


def processinput(ulimit, numvals, outputfile):
    lowlim = 0  # floor is zero
    ofile = open(outputfile, "w+")  # open file for writing
    #  random.seed(5)
    for i in range(0, numvals):
        anint = random.randint(lowlim, ulimit)
        ostring = str(anint) + "\n"
        ofile.write(ostring)
    ofile.close()


def main(argv):
    parser = ArgumentParser(description="numgen: random numbers to file")
    parser.add_argument(
        "--ulimit",
        metavar="upperlim",
        type=int,
        help="set the upper limit of range for random gen",
    )
    parser.add_argument(
        "--numvals",
        metavar="values",
        type=int,
        help="specify the total number of values",
    )
    parser.add_argument("--ofile", metavar="outfile", type=str, help="write to file")
    myargs = parser.parse_args()  # create a namespace with argv[]
    ulimit = myargs.ulimit
    numvals = myargs.numvals
    outputfile = myargs.ofile
    processinput(ulimit, numvals, outputfile)


if __name__ == "__main__":
    main(sys.argv[1:])
