#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pareto.h"

int
main(int argc, char *argv[]){
  FILE* fin;
  double* data;
  unsigned int d, cpt;

  char * line = NULL;
  char* tmp = NULL;
  size_t len = 0;
  ssize_t read;

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

  freeArchive();
  fclose(fin);
  free(line);

  exit(EXIT_SUCCESS);
}
