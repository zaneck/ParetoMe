#include <assert.h>
#include <stddef.h>

#include "list.h"

static int call_count = 0;
static int call_sum = 0;

static void count_and_sum(void *data) {
  call_count++;
  call_sum += *(int *)data;
}

int main(void) {
  list_t empty = newList();
  assert(empty != NULL);
  toAll(empty, count_and_sum);
  assert(call_count == 0);
  freeList(empty);

  list_t single = newList();
  assert(single != NULL);
  int one = 1;
  assert(addTail(single, &one));
  toAll(single, count_and_sum);
  assert(call_count == 1);
  assert(call_sum == 1);
  freeList(single);

  list_t many = newList();
  assert(many != NULL);
  int two = 2;
  int three = 3;
  int four = 4;
  assert(addTail(many, &two));
  assert(addTail(many, &three));
  assert(addTail(many, &four));

  toAll(many, count_and_sum);
  assert(call_count == 4);
  assert(call_sum == 10);

  freeList(many);
  return 0;
}
