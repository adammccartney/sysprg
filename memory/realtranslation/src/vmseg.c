/* real.c: short program to show how to use binary operators,
 * masks and shits to set up address space */

#include <stdio.h>
#include <stdlib.h>

#define SEG0_MASK 0x0
#define SEG1_MASK 0x1000
#define SEG_SHIFT 12
#define OFFSET_MASK 0xFFF
#define ADDRESS_SPACE_SZ 128

enum ExceptionType { PROTECTION_FAULT = 1, NUM_EXCEPTIONS };

void RaiseException(enum ExceptionType extyp) {
  if (extyp == 1) {
    printf("Error: Segmentation Fault\n");
  }
}

int main() {
  int Segment, Offset, PhysAddr;
  int VirtualAddress = SEG1_MASK + 108;

  /* define address space for stack and heap */
  int Base[2] = {SEG0_MASK, SEG1_MASK};
  int Bounds[2] = {SEG0_MASK + ADDRESS_SPACE_SZ, SEG1_MASK - ADDRESS_SPACE_SZ};
  // get top two bits of a 14-bit VA
  Segment = (VirtualAddress & SEG1_MASK) >> SEG_SHIFT;

  Offset = VirtualAddress & OFFSET_MASK;

  if (Offset >= Bounds[Segment]) {
    RaiseException(PROTECTION_FAULT);
  } else {
    PhysAddr = Base[Segment] + Offset;
    printf("Phys Addr: %d\n", PhysAddr);
    // Register = AccessMemory(PhysAddr);
  }

  printf("Segment: %d\n", Segment);
  printf("Offset: %d\n", Offset);

  return 0;
}
