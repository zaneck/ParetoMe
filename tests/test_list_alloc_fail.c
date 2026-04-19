#include <assert.h>
#include <stddef.h>
#include <stdlib.h>

#include "list.h"

static int fail_next_alloc = 0;

static void *failing_malloc(size_t size) {
  if (fail_next_alloc) {
    fail_next_alloc = 0;
    return NULL;
  }
  return malloc(size);
}

int main(void) {
  int a = 1;

  setListMemoryHooks(failing_malloc, free);

  fail_next_alloc = 1;
  assert(newList() == NULL);

  setListMemoryHooks(NULL, NULL);
  list_t list = newList();
  assert(list != NULL);

  setListMemoryHooks(failing_malloc, free);

  fail_next_alloc = 1;
  assert(!addHead(list, &a));

  fail_next_alloc = 1;
  assert(!addTail(list, &a));

  setListMemoryHooks(NULL, NULL);
  freeList(list);
  return 0;
}
