#!/usr/bin/python3

"""conversions.py: quick solutions to some memory translation questions.
Exercises revolve around simple two segment memory

   Address Space size = 128     ( 2 ^ 7 )
   Physical memory size = 512   ( 2 ^ 9 )
"""


from dataclasses import dataclass

# Q. 1


@dataclass
class Address:
    vaddr: int
    paddr: int
    seg: int


def whichSeg(vaddr, asize):
    if vaddr >= (asize / 2):
        return 1
    else:
        return 0


vaddr = [108, 87, 53, 33, 65]
valid = []


@dataclass
class Params:
    base0: int
    base1: int
    lim: int
    asize: int


q1params = Params(0, 512, 20, 128)

#
#    takes a list of virtual memory addresses and translates them to physical
#    ones if they fit into the address space
#    returns an array of valid Address objects
#    returns empty list if no valid translations are possible
#
def translate(vaddr, memparams):
    valid = []
    for a in vaddr:
        paddr = 0
        seg = whichSeg(a, memparams.asize)
        if seg == 1:
            paddr = memparams.base1 + (a - memparams.asize)
            if paddr >= memparams.base1 - memparams.lim:
                newaddr = Address(a, paddr, seg)
                valid.append(newaddr)
        if seg == 0:
            if a < memparams.lim:
                paddr = a + memparams.base0
                newaddr = Address(a, paddr, seg)
                valid.append(newaddr)
    return valid


valid = translate(vaddr, q1params)
print(valid)
