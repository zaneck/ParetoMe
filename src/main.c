#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include "pareto.h"

void
usage(void){
  printf("usage: paretoMe -d dim file\n");
  printf("   or: paretoMe dim file\n");
}

static int
parse_dimension(const char *arg, unsigned int *out_dim){
  char *end = NULL;
  unsigned long value;

  errno = 0;
  value = strtoul(arg, &end, 10);
  if (errno != 0 || end == arg || *end != '\0' || value == 0 || value > UINT_MAX) {
    return 0;
  }

  *out_dim = (unsigned int)value;
  return 1;
}

static int
parse_line_to_point(char *line, unsigned int d, double *data){
  unsigned int i;
  char *saveptr = NULL;
  char *token = strtok_r(line, " \t\r\n", &saveptr);

  for (i = 0; i < d; i++) {
    char *end = NULL;

    if (token == NULL) {
      return 0;
    }

    errno = 0;
    data[i] = strtod(token, &end);
    if (errno != 0 || end == token || *end != '\0') {
      return 0;
    }

    token = strtok_r(NULL, " \t\r\n", &saveptr);
  }

  if (token != NULL) {
    return 0;
  }

  return 1;
}

static int
parse_cli_args(int argc, char *argv[], unsigned int *out_dim, const char **out_input_path){
  int opt;
  int dim_set = 0;
  const char *input_path = NULL;

  opterr = 0;
  optind = 1;

  while ((opt = getopt(argc, argv, "hd:")) != -1) {
    switch (opt) {
      case 'd':
        if (!parse_dimension(optarg, out_dim)) {
          return 0;
        }
        dim_set = 1;
        break;
      case 'h':
      default:
        return 0;
    }
  }

  if (!dim_set) {
    if (optind >= argc || !parse_dimension(argv[optind], out_dim)) {
      return 0;
    }
    optind++;
  }

  if (optind >= argc) {
    return 0;
  }

  input_path = argv[optind++];
  if (optind != argc) {
    return 0;
  }

  *out_input_path = input_path;
  return 1;
}

int
main(int argc, char *argv[]){
  FILE *fin;
  double *data;
  unsigned int d = 0;
  const char *input_path = NULL;

  char *line = NULL;
  size_t len = 0;
  ssize_t read;
  unsigned long line_no = 0;

  if (!parse_cli_args(argc, argv, &d, &input_path)) {
    usage();
    return EXIT_FAILURE;
  }

  initArchive(d);

  fin = fopen(input_path, "r");

  if (fin == NULL) {
    perror("fopen: ");
    freeArchive();
    return EXIT_FAILURE;
  }

  read = getline(&line, &len, fin);
  while (read != -1) {
    line_no++;
    data = (double *)calloc(d, sizeof(double));
    if (data == NULL) {
      perror("calloc: ");
      freeArchive();
      fclose(fin);
      free(line);
      return EXIT_FAILURE;
    }

    if (!parse_line_to_point(line, d, data)) {
      fprintf(stderr, "invalid input at line %lu\n", line_no);
      free(data);
      freeArchive();
      fclose(fin);
      free(line);
      return EXIT_FAILURE;
    }


    addToPareto(data);

    read = getline(&line, &len, fin);
  }

  printParetoArchive();
  
  findMin();
  findMax();
  printf("min: ");
  for (unsigned int i = 0; i < dim; i++) {
    printf("%2lf ", min[i]);
  }
  printf("\nmax: ");
  for (unsigned int i = 0; i < dim; i++) {
    printf("%2lf ", max[i]);
  }
  printf("\n");

  free(min);
  min = NULL;
  free(max);
  max = NULL;

  freeArchive();
  fclose(fin);
  free(line);

  return EXIT_SUCCESS;
}
