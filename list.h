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

#if ! defined (LIST_H)
#define LIST_H 1

#include<stdbool.h>

typedef struct list_s* list_t;

extern list_t newList (void);
extern bool isEmpty (list_t);
extern bool addHead (list_t, void*);
extern bool addTail (list_t, void*);
extern void removeHead (list_t);
extern void removeTail (list_t);
extern void toAll (list_t, void (*) (void *));
extern void freeList (list_t);

typedef struct node_s* node_t;

extern node_t getHead(list_t);
extern node_t getTail(list_t);
extern bool hasNext(node_t);
extern bool hasPrevious(node_t);
extern node_t next(node_t);
extern node_t previous(node_t);
extern void* getData(node_t);

extern void removeNode(list_t, node_t);
/* extern void addPrevious(node_t, void*); */
/* extern void addNext(node_t, void*); */

#endif
