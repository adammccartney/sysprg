/* vector.c: c program that models the vector (cpp) datastructure
 * a vector in this context is an array with dynamic resizing */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct vector {
  size_t sz;
  int nitems;
  int *elem;
};

void init_vector(struct vector *v, int sz) {
  if (sz < 1) {
    perror("Error: trying to initialize a vector with size less than 1.\n");
  }
  v->sz = sz;
  v->elem = (int *)malloc(sz * sizeof(int));
  v->nitems = 0; /* start with zero items */
}

void push_vector(struct vector *v, int item) {
  /* assume that the item to be added may overflow the allotted memory */

  if (v->nitems + 1 > v->sz) {
    int newsize = v->sz + 1;
    v->elem = (int *)realloc(v->elem, newsize * sizeof(int));
  }
  v->elem[v->nitems] = item;
  v->nitems++;
}

int pop_vector(struct vector *v) {
  /* checks vector contains elements */
  /* pops the last item added to vector */
  /* decreases number of items */
  if (v->nitems < 1) {
    perror("Error: vector is empty");
    return -1;
  }
  int val;
  val = v->elem[v->nitems - 1]; /* my head hurts ... off by one */
  v->nitems--;
  return val;
}

void print_vector(struct vector *v) {
  int i;
  for (i = 0; i < v->nitems; i++) {
    printf("v[%d]: %d\n", i, v->elem[i]);
  }
}

void free_vector(struct vector *v) {
  v->sz = 0;
  free(v->elem);
}

int main() {
  struct vector myvec;
  init_vector(&myvec, 3);
  push_vector(&myvec, 1);
  push_vector(&myvec, 2);
  push_vector(&myvec, 3);
  print_vector(&myvec);
  int r1, r2, r3;
  r1 = pop_vector(&myvec);
  r2 = pop_vector(&myvec);
  r3 = pop_vector(&myvec);
  printf("r1: %d, r2: %d, r3: %d\n", r1, r2, r3);
  int arr[5] = {0, 1, 2, 3, 4};
  int len = 5;
  int i;
  for (i = 0; i < len; i++) {
    push_vector(&myvec, arr[i]);
  }
  print_vector(&myvec);
  free_vector(&myvec);
  return 0;
}
