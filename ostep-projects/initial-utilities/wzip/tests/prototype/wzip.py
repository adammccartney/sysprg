#! /usr/bin/env python3

"""
wzip.py: prototype of a simple file compression program

    for test purposes only 
    this file is a prototype for wzip.c
    it is used to model program structure

usage: python3 wzip.py file [files...] > file.z

file will typically contain a number of repeated ascii characters.
this program counts the characters
the number of repeats, followed by a single byte ascii char 
are printed to stdout
""" 

import sys


class Token:
    """ "Token holds the char currently being read from a stream
    keeps count of number of occurrences and also a 1 byte representation
    of the token."""

    def __init__(self, ch=0):
        self.ch = ch
        self.chi8 = self._chtoi8(ch)  # converts ch to 1 byte ascii
        self.tokcount = 1

    def __repr__(self):
        return f"ch={self.ch}, chi8={self.chi8}, count={self.tokcount}"

    def __iadd__(self, num):
        self.tokcount += num
        return self

    def _chtoi8(self, ch):
        chi8 = ord(ch)
        if (chi8 < 0) or (chi8 > 127):
            print("Error: ch outside of ascii range 0 - 127")
        return chi8

    def printout(self):
        outstr = f"{self.tokcount}{self.ch}"
        print(outstr)

    def update(self, ch):  # udpates with new token
        self.__init__(ch)
        self.tokcount = 1


class ShortStack:
    "ShortStack holds up to three characters: <ascii char> <\n> <EOF>"

    def __init__(self):
        self.items = []
        self.head = 0
        self.maxsize = 3

    def push(self, item):
        if self.head >= self.maxsize:
            raise Exception("Error: stack overflow")

        self.items.append(item)
        self.head += 1

    def pop(self):
        if self.head == 0:
            raise Exception("Error: stack underflow")

        self.items.pop(self.head - 1)  # list is zero index, head not
        self.head -= 1


# Routines for processing input from files on the command line
def getfilenames(args):
    filenames = []
    for arg in args[:]:
        filenames.append(arg)
    return filenames


def procfiles(filenames):
    "Process files: takes a list of filenames given via argv[], read by line"
    mystack = ShortStack()  # use a stack to manage the
    for fname in filenames[1:]:
        readlines(fname)


def advance(line, current):
    ch = line[current]
    return ch


def peeknext(line, current):
    if current + 1 >= len(line):
        return "\0"
    return line[current + 1]


def isatend(line, current):
    if current >= len(line):
        return True
    return False


def readlines(file):
    ch = '\0'
    lines = list(open(file, "r"))
    for l in lines:
        current = 0
        initchar = l[current]
        curtoken = Token(initchar)
        for c in l:
            ch = advance(l, current)
            nch = peeknext(l, current)
            end = isatend(l, current)
            if end == False:
                current += 1
            if ch == nch:
                curtoken += 1
            if nch != ch:
                curtoken.printout()
                curtoken.update(nch)


# main function
def main():
    args = sys.argv[:]
    files = getfilenames(args)
    procfiles(files)


if __name__ == "__main__":
    main()
