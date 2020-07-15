#include <stdio.h>
#include <stdlib.h>

#include "pareto.h"

list_t pareto;
list_t data;
unsigned int dim;

/*init archive*/
void initArchive(unsigned int d){
  pareto=newList();
  data=newList();
  dim = d;
}

FILE* fd;

void transprint(void*a){
  double* alpha=(double*)a;

  for(int i=0; i< dim; i++){
    fprintf(fd,"%f ",alpha[i]);
  }
  fprintf(fd,"\n");
}

void printDataArchive(void){
  fd=fopen("data.txt", "w");
  toAll(data, transprint);
  fclose(fd);
}
void printParetoArchive(void){
  fd=fopen("pareto.txt", "w");
  toAll(pareto, transprint);
  fclose(fd);
}

void freeArchive(void){
  toAll(data, free);
  freeList(pareto);
  freeList(data);
}


double* max = NULL;
double* min = NULL;
double* findMax(void){
  max = (double*)calloc(dim, sizeof(double));
  toAll(pareto, maxMe);
  return max;
}

double* findMin(void){
  min = (double*)calloc(dim, sizeof(double));
  for(unsigned int i=0; i<dim; i++){
    min[i] = 5;
  }
  toAll(pareto, minMe);
  return min;
}

void maxMe(void *a){
  double* alpha=(double*)a;
  for(unsigned int i=0; i<dim; i++){
    if (max[i] < alpha[i]){
      max[i] = alpha[i];
    }
  }
}

void minMe(void *a){
  double* alpha=(double*)a;
  for(unsigned int i=0; i<dim; i++){
    if (min[i] > alpha[i]){
      min[i] = alpha[i];
    }
  }
}

/*
 * domine 1
 * enlarge 0
 * dominé -1
 *
 */
int domine(double* a, double* b){
  int res;

  res=0;

  for(int i=0; i<dim; i++){
    if(a[i]<b[i]){
      res++;
    }
  }

  if(res==dim){
    return 1;
  }
  else if(res==0){
    return -1;
  }

  return 0;
}



/*online mode*/
void addToPareto(double* t){
  node_t p, delete;
  double* pcost, *dcost;
  int flagInsert, dom;

  flagInsert=1;
  p=getHead(pareto);

  while(p!=NULL){
    pcost=((double*)getData(p));

    dom=domine(t,pcost);

    if(dom==1){ /*dommine*/
      delete=p;
      p=next(p);
      removeNode(pareto,delete);
    }
    else if (dom == -1 ){ /*se fait domminer*/
      flagInsert=0;
      p=NULL;
    }
    else{/*profite au front*/
      p=next(p);
    }
  }

  if(flagInsert==1){
    addHead(data,t);
    addHead(pareto,t);
  }
  else{
    addHead(data,t);
  }
}
