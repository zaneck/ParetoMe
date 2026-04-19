#include <assert.h>
#include <stddef.h>

#include "list.h"

int main(void) {
  list_t list = newList();
  assert(list != NULL);

  removeHead(list);
  removeTail(list);
  assert(isEmpty(list));

  int a = 1;
  int b = 2;

  assert(addHead(list, &a));
  removeHead(list);
  assert(isEmpty(list));

  assert(addTail(list, &a));
  removeTail(list);
  assert(isEmpty(list));

  assert(addTail(list, &a));
  assert(addTail(list, &b));
  removeHead(list);
  assert(getData(getHead(list)) == &b);
  assert(getData(getTail(list)) == &b);

  assert(addHead(list, &a));
  removeTail(list);
  assert(getData(getHead(list)) == &a);
  assert(getData(getTail(list)) == &a);

  freeList(list);
  return 0;
}
