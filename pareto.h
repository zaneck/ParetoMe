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

#endif
