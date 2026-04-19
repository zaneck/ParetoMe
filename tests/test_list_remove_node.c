#include <assert.h>
#include <stddef.h>

#include "list.h"

int main(void) {
  list_t list = newList();
  assert(list != NULL);

  int a = 1;
  int b = 2;
  int c = 3;

  assert(addTail(list, &a));
  assert(addTail(list, &b));
  assert(addTail(list, &c));

  node_t middle = next(getHead(list));
  removeNode(list, middle);
  assert(getData(getHead(list)) == &a);
  assert(getData(getTail(list)) == &c);

  removeNode(list, getHead(list));
  assert(getData(getHead(list)) == &c);
  assert(getData(getTail(list)) == &c);

  assert(addHead(list, &a));
  removeNode(list, getTail(list));
  assert(getData(getHead(list)) == &a);
  assert(getData(getTail(list)) == &a);

  removeNode(list, getHead(list));
  assert(isEmpty(list));

  freeList(list);
  return 0;
}
