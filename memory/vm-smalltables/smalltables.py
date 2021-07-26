#!/usr/bin/python3

PDE_SIZE = 32 # unrealistic page size (1024 x 32 bytes = 32KB) 
PDBR = 0x6c

PDE_MASK = 0x7c00 # page directory entry
PDE_SHIFT = 10

PTE_MASK = 0x03e0 # page table entry 
PTE_SHIFT = 5

VPN_MASK = PDE_MASK | PTE_MASK
VPN_SHIFT = PTE_SHIFT

"""
for each virtual address that we want to translate, 
we first perform the following operation
this will tell us which page directory entry to access
i.e. what our virtual page number is
Then we calculate the page directory index
Then check if the address is valid
"""


vaddr1 = 0x611c
VPN1 = (vaddr1 & VPN_MASK) >> VPN_SHIFT
PDIndex1 = (VPN1 & PDE_MASK) >> PDE_SHIFT
PDEAddr1 = PDBR + (PDIndex1 * PDE_SIZE) # decimal 108 in this example 
"""
we would then use an access memory routine to get the PDEAddr
PDE = AccessMemory(PDEAddr1)
then check validity
if valid, begin to access the Page Table Entry
"""

vaddr2 = 0x3da8
VPN2 = (vaddr2 & VPN_MASK) >> VPN_SHIFT
PDIndex2 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr2 = PDBR + (PDIndex2 * PDE_SIZE) # decimal 108 in this example 

vaddr3 = 0x17f5
VPN3 = (vaddr3 & VPN_MASK) >> VPN_SHIFT
PDIndex3 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr3 = PDBR + (PDIndex3 * PDE_SIZE) # decimal 108 in this example 

vaddr4 = 0x7f6c
VPN4 = (vaddr4 & VPN_MASK) >> VPN_SHIFT
PDIndex4 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr4 = PDBR + (PDIndex4 * PDE_SIZE) # decimal 108 in this example 

vaddr5 = 0x0bad
VPN5 = (vaddr5 & VPN_MASK) >> VPN_SHIFT
PDIndex5 = (VPN2 & PDE_MASK) >> PDE_SHIFT
PDEAddr5 = PDBR + (PDIndex5 * PDE_SIZE) # decimal 108 in this example 
