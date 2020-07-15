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

#if ! defined (LIST_LOCAL_H)
#define LIST_LOCAL_H 1

struct node_s{
  node_t next;
  node_t previous;
  void *data;
};


struct list_s{
  node_t head;
  node_t tail;
};

extern node_t newNode (void*);

#endif
