#ifndef UTIL_HH
#define UTIL_HH

#define UNUSED(x) (void)(x)

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <algorithm>    // std::swap
#include <vector>       // std::vector
#include <fstream>
#include <limits>
#include <unistd.h> /* for access() */
#include <stdlib.h> /* for getenv() */
#include <stdio.h> /* for sprintf(), fopen() */
#include <ctime>
#include <G4Types.hh>

#include <sstream>
#include <iterator>

FILE *open_file(const char *filename, const char *mode);
char *find_file(const char *filename);
int read_tsv_file(const char *filename, double *energy, double *values, double xunit, double yunit);
int partition(double arr[], int start, int end);
void quickSort(double arr[], int start, int end);
double LYSOMeshVolume(G4double* xv0, G4int Onode, G4int Znode);
double *Str2DChar(std::string strinput, G4int nn);
G4double TetraVolume(G4double *x,G4double *y,G4double *z);

#endif
