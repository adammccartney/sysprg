/* null.c: simple program to test memory facilities and tools in os */

/* Description:
 * program first creates a pointer to an integer, sets it to null and then
 * tries to dereference it.
 * */

#include <stdio.h>
#include <stdlib.h>

int main() {
  int SIZE = 100;

  int *data;
  data = (int *)malloc(SIZE * (sizeof(int)));
  data[100] = 0;

  int i;
  for (i = 0; i < SIZE; i++) {
    free(data[i]);
  }

  printf("%d", data[55]);

  return 0;
}
