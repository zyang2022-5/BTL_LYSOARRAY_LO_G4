#ifndef UTIL_HH
#define UTIL_HH

#define UNUSED(x) (void)(x)

#include <stdio.h>
#include "G4Types.hh"

FILE *open_file(const char *filename, const char *mode);
char *find_file(const char *filename);
int read_tsv_file(const char *filename, double *energy, double *values, double xunit, double yunit);
int partition(double arr[], int start, int end);
void quickSort(double arr[], int start, int end);
double LYSOMeshVolume(G4double* xv0, G4int Onode, G4int Znode);
#endif
