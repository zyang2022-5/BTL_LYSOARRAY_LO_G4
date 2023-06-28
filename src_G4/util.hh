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
#include <set>
#include <gmsh.h>

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


std::vector<std::vector<double>> appendReversedInitialVector(const std::vector<std::vector<double>>& inputVector);
std::vector<std::vector<double>> reverseSubvectors(const std::vector<std::vector<double>>& inputVector) ;
std::vector<std::vector<double>> combineAllIndices(const std::vector<double>& vec1, const std::vector<double>& vec2); 
std::vector<std::vector<int>> combineAllIndicesint(const std::vector<int>& vec1, const std::vector<int>& vec2);
std::vector<std::vector<double>> calculatePoints(const std::vector<double>& Y, double Xin, double Zmin, double Zmax, double Yzero);
std::vector<int> createGmshPoints(const std::vector<std::vector<double>>& points, int taginit);
std::vector<int> createGmshLines(const std::vector<int>& , int , const std::set<int>& );
int createGmshSurface(const std::vector<int>& lineTags, int taginit);
std::vector<double> extractRange(const std::vector<double>& Y_all, int sec, int nodesec);
void printVectorOfVectors(const std::vector<std::vector<int>>& vec);
void printVector(const std::vector<int>& vec);
void printVectordouble(const std::vector<double>& vec);
std::vector<int> getElementsAtIndex(const std::vector<std::vector<int>>& vec, int index, int a, int b);
std::vector<double> generateEquispacedSegments(double Xmin, double Xmax, int numElements);
std::vector<double> convertArrayToVector(const double* arr, size_t size);
std::vector<int> getOddElementsAtIndex(const std::vector<std::vector<int>>& vec, int index, int a, int b);
std::vector<int> getEvenElementsAtIndex(const std::vector<std::vector<int>>& vec, int index, int a, int b);
std::vector<int> getValuesAtIndices(const std::vector<int>& vec, const std::vector<int>& indices);



#endif
