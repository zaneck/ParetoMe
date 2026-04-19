#include <assert.h>
#include <stddef.h>

#include "list.h"

int main(void) {
  list_t list = newList();
  assert(list != NULL);
  assert(isEmpty(list));

  int a = 1;
  int b = 2;
  int c = 3;

  assert(addTail(list, &a));
  assert(getData(getHead(list)) == &a);
  assert(getData(getTail(list)) == &a);

  assert(addHead(list, &b));
  assert(getData(getHead(list)) == &b);
  assert(getData(getTail(list)) == &a);

  assert(addTail(list, &c));
  assert(getData(getHead(list)) == &b);
  assert(getData(getTail(list)) == &c);

  node_t head = getHead(list);
  node_t middle = next(head);
  node_t tail = getTail(list);

  assert(hasNext(head));
  assert(!hasPrevious(head));
  assert(hasNext(middle));
  assert(hasPrevious(middle));
  assert(previous(middle) == head);
  assert(!hasNext(tail));
  assert(hasPrevious(tail));

  freeList(list);
  return 0;
}
