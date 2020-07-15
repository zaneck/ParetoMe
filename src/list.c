/* Zstructs for all your C stuff  */
/* Copyright (C) 2015  Valentin Owczarek <val.owczarek@gmail.com>  */

/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or  */
/* (at your option) any later version.  */

/* This program is distributed in the hope that it will be useful,  */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of  */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  */
/* GNU General Public License for more details.  */

/* You should have received a copy of the GNU General Public License  */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "list-local.h"

node_t
newNode (void* data){
  node_t new;

  new = (node_t) malloc (sizeof (struct node_s));

  if (new != NULL){
    new->next = NULL;
    new->previous = NULL;
    new->data = data;
  }
  return new;
}

list_t
newList (void){
  list_t new;

  new = (list_t) malloc (sizeof (struct list_s));

  if (new != NULL){
    new->head = NULL;
    new->tail = NULL;
  }

  return new;
}

bool
isEmpty (list_t a){
  return (a->head == NULL && a->tail == NULL);
}

bool
addHead(list_t a, void* data){
  node_t n;
  n=newNode(data);

  if(n != NULL){
    if(isEmpty(a)){
      a->head=n;
      a->tail=n;
    }
    else{
      n->next=a->head;
      a->head->previous=n;
      a->head=n;
    }
    return true;
  }
  else{
    return false;
  }
}

bool
addTail(list_t a, void* data){
  node_t n;

  n=newNode(data);

  if(n != NULL){
    if(isEmpty(a)){
      a->head=n;
      a->tail=n;
    }
    else{
      n->previous=a->tail;
      a->tail->next=n;
      a->tail=n;
    }
    return true;
  }
  else{
    return false;
  }
}

void removeHead(list_t a){
  node_t n;

  if(!isEmpty(a)){
    n=a->head;

    if(n->next==NULL){
      a->head=NULL;
      a->tail=NULL;
    }
    else{
      a->head=n->next;
      a->head->previous=NULL;
    }    
    free(n);
  }
}

void removeTail(list_t a){
  node_t n;

  if(!isEmpty(a)){
    n=a->tail;

    if(n->previous==NULL){
      a->tail=NULL;
      a->head=NULL;
    }
    else{
      a->tail=n->previous;
      a->tail->next=NULL;
    }
    free(n);
  }
}

void
toAll(list_t a, void (*filter)(void *)){
  node_t n;

  if(!isEmpty(a)){
  
    n=a->head;

    while(n->next!=NULL){
      filter(n->data);
      n=n->next;
    }
    filter(n->data);
  }
}

void
freeList (list_t a){
  node_t n;

   if(!isEmpty(a)){
     n = a->head;
     while (n != a->tail){
       n = n->next;
       free (n->previous);
     }
     free (n);
   }
  free (a);
}

node_t
getHead(list_t a){
  return a->head;
}

node_t
getTail(list_t a){
  return a->tail;
}

bool
hasNext(node_t n){
  return n->next != NULL;
}

bool
hasPrevious(node_t n){
  return n->previous != NULL;
}

node_t
next(node_t n){
  return n->next;
}

node_t
previous(node_t n){
  return n->previous;
}

void*
getData(node_t n){
  return n->data;
}

void
removeNode(list_t l, node_t n){
  if(n->previous != NULL){
    n->previous->next=n->next;
  }
  else{
    l->head = n->next;
  }

  if(n->next != NULL){
     n->next->previous=n->previous;
  }
  else{
    l->tail=n->previous;
  }
  
  free(n);
}

/* void */
/* addPrevious(node_t n, void* data){ */
/*   node_t new; */

/*   new= newNode(data); */
  
/* } */

/* void */
/* addNext(node_t, void*){ */
/*   node_t new; */

/*   new= newNode(data); */
  
/* } */
