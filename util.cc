#include "util.hh"
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

/* Read in data from a tab or space separated file with two columns. `filename`
 * should be the tsv file. `energy` and `values` should be double arrays big
 * enough to hold all the values. The values in the first column are multiplied
 * by `xunit` and the values in the second column are multiplied by `yunit`.
 *
 * Returns -1 on error and the number of rows in the file on success.
 *
 * Example:
 *
 *     G4double lyso_rindex_ene[1000], lyso_rindex_values[1000];
 *
 *     n = read_tsv_file("lyso_rindex.dat", lyso_rindex_ene, lyso_rindex_values, eV, 1);
 *
 *     if (n == -1) {
 *         fprintf(stderr, "error reading lyso_rindex.dat!\n");
 *         exit(1);
 *     }
 *
 *     mptScint->AddProperty("RINDEX", lyso_rindex_ene, lyso_rindex_values, n);
 *
 */
int read_tsv_file(const char *filename, double *energy, double *values, double xunit, double yunit)
{
    int i, j;
    char line[256];
    char *str;
    double value;
    int n;

    FILE *f = fopen(filename, "r");

    if (!f) {
        fprintf(stderr, "failed to '%s': %s\n", filename, strerror(errno));
        return -1;
    }

    i = 0;
    n = 0;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        if (len && (line[len-1] != '\n')) {
            fprintf(stderr, "got incomplete line on line %i: '%s'\n", i, line);
            goto err;
        }

        i += 1;

        if (!len) continue;
        else if (line[0] == '#') continue;

        str = strtok(line," \n");

        j = 0;
        while (str) {
            value = strtod(str, NULL);
            switch (j) {
            case 0:
                energy[n] = value*xunit;
                break;
            case 1:
                values[n] = value*yunit;
                break;
            }
            j += 1;
            str = strtok(NULL," \n");
        }

        if (j > 1)
            n += 1;
    }

    fclose(f);

    return n;

err:
    fclose(f);

    return -1;
}

///////////////
// Last line
///////////////
std::istream& ignoreline(std::ifstream& in, std::ifstream::pos_type& pos)
{
    pos = in.tellg();
    return in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getLastLine(std::ifstream& in)
{
    std::ifstream::pos_type pos = in.tellg();

    std::ifstream::pos_type lastPos;
    while (in >> std::ws && ignoreline(in, lastPos))
        pos = lastPos;

    in.clear();
    in.seekg(pos);

    std::string line;
    std::getline(in, line);
    return line;
}

///////////////
// QUICKSORTING
///////////////
int partition(double arr[], int start, int end)
{
 
    double pivot = arr[start];
 
    int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i] <= pivot)
            count++;
    }
 
    // Giving pivot element its correct position
    int pivotIndex = start + count;
    std::swap(arr[pivotIndex], arr[start]);
 
    // Sorting left and right parts of the pivot element
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while (arr[i] <= pivot) {
            i++;
        }
 
        while (arr[j] > pivot) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            std::swap(arr[i++], arr[j--]);
        }
    }
 
    return pivotIndex;
}

void quickSort(double arr[], int start, int end)
{
 
    // base case
    if (start >= end)
        return;
 
    // partitioning the array
    int p = partition(arr, start, end);
 
    // Sorting the left part
    quickSort(arr, start, p - 1);
 
    // Sorting the right part
    quickSort(arr, p + 1, end);
}
