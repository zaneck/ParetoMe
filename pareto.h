#ifndef ARCHIVE_H
#define ARCHIVE_H

#include "list.h"

extern list_t archive;
extern list_t data;
extern unsigned int dim;

/*init archive*/
extern void initArchive(unsigned int);
extern void printDataArchive(void);
extern void printParetoArchive(void);
extern void freeArchive(void);

/*online mode*/
extern void addToPareto(double*);

extern double* max;
extern double* min;

extern void minMe(void *a);
extern double* findMin(void);
extern void maxMe(void *a);
extern double* findMax(void);
#endif
