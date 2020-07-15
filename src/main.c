#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pareto.h"

void
usage(void){
  printf("paretoMe dim file\n");
}

int
main(int argc, char *argv[]){
  FILE* fin;
  double* data;
  unsigned int d, cpt;

  char * line = NULL;
  char* tmp = NULL;
  size_t len = 0;
  ssize_t read;

  if(argc !=3){
    usage();
    exit(EXIT_FAILURE);
  }

  printf("Hello %s %s\n", argv[1], argv[2]);
  d = atoi(argv[1]);
  initArchive(d);


  fin = fopen(argv[2], "r");

  if (fin == NULL){
    perror("fopen: ");
    exit(EXIT_FAILURE);
  }

  read = getline(&line, &len, fin);
  while (read != -1) {
    data = (double*)calloc(d, sizeof(double));

    cpt = 0;
    tmp = strtok(line, " ");

    while (tmp != NULL){
      data[cpt] = atof(tmp);
      tmp = strtok(NULL, " ");
      cpt ++;
    }

    addToPareto(data);

    read = getline(&line, &len, fin);
  }

  printParetoArchive();

  findMin();
  findMax();
  printf("min: ");
  for (unsigned int i=0; i<dim; i++){
    printf("%2lf ", min[i]);
  }
  printf("\nmax: ");
  for (unsigned int i=0; i<dim; i++){
    printf("%2lf ", max[i]);
  }
  printf("\n");

  freeArchive();
  fclose(fin);
  free(line);

  exit(EXIT_SUCCESS);
}
