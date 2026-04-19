#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "pareto.h"

int main(void) {
  initArchive(2);

  double a1[2] = {1.0, 1.0};
  double a2[2] = {2.0, 2.0};
  double a3[2] = {0.5, 3.0};

  assert(domine(a1, a2) == 1);
  assert(domine(a2, a1) == -1);
  assert(domine(a2, a3) == 0);

  double *p1 = (double *)calloc(2, sizeof(double));
  double *p2 = (double *)calloc(2, sizeof(double));
  double *p3 = (double *)calloc(2, sizeof(double));
  assert(p1 != NULL && p2 != NULL && p3 != NULL);

  p1[0] = 2.0;
  p1[1] = 2.0;
  p2[0] = 1.0;
  p2[1] = 1.0;
  p3[0] = 0.5;
  p3[1] = 3.0;

  addToPareto(p1);
  addToPareto(p2);
  addToPareto(p3);

  printDataArchive();
  printParetoArchive();

  FILE *data_file = fopen("data.txt", "r");
  FILE *pareto_file = fopen("pareto.txt", "r");
  assert(data_file != NULL);
  assert(pareto_file != NULL);
  fclose(data_file);
  fclose(pareto_file);

  findMin();
  findMax();

  assert(min != NULL);
  assert(max != NULL);
  assert(min[0] <= max[0]);
  assert(min[1] <= max[1]);

  free(min);
  free(max);
  min = NULL;
  max = NULL;

  freeArchive();
  return 0;
}
