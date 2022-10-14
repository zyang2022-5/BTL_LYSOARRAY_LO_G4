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
#include <unistd.h> /* for access() */
#include <stdlib.h> /* for getenv() */
#include <stdio.h> /* for sprintf(), fopen() */
#include <ctime>
#include <G4Types.hh>

#include <sstream>
#include <iterator>

/* Equivalent to fopen() but uses find_file() to find the path to the filename. */
FILE *open_file(const char *filename, const char *mode)
{
    char *path;

    path = find_file(filename);

    if (!path) return NULL;

    return fopen(path,mode);
}

/* Returns the full path to `filename` by searching the current working
 * directory and the path specified by the environment variable
 * BTL_LYSOARRAY_LO_G4_DATA.
 *
 * Returns a pointer to the full path on success or NULL if the file isn't
 * found. */
char *find_file(const char *filename)
{
    static char path[256];
    char *dir;

    /* First check in the current working directory. */
    sprintf(path, "%s", filename);
    if (access(path, F_OK) == 0) return path;

    /* Now check in the directory specified by the environment variable
     * BTL_LYSOARRAY_LO_G4_DATA. */
    dir = getenv("BTL_LYSOARRAY_LO_G4_DATA");
    if (dir) {
        sprintf(path, "%s/%s", dir,filename);
        if (access(path, F_OK) == 0) return path;
    }

    dir = "data";
    if (dir) {
        sprintf(path, "%s/%s", dir,filename);
        if (access(path, F_OK) == 0) return path;
    }

    return NULL;
}
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

    FILE *f = open_file(filename, "r");

    if (!f) {
        fprintf(stderr, "failed to open '%s': %s\n", filename, strerror(errno));
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

        str = strtok(line," \t\n");

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
            str = strtok(NULL," \t\n");
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

///////////////
// DATETIME
///////////////

std::string datetime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,80,"%d-%m-%Y %H-%M-%S",timeinfo);
    return std::string(buffer);
}

/////////////////////////////////////
// Triangular mesh volume calculation
/////////////////////////////////////

double LYSOMeshVolume(G4double* xv0, G4int Onode, G4int Znode){

        // initialization of symmetry faces for testing
        G4double Pi=atan(1)*4;
        G4double DTheta=Pi/(Onode-1);
            G4double theta = -Pi/2;G4double tolxy=1e-10; G4double LYSO_L=57.;
            G4double x[3];            G4double y[3]; G4double z[3]; 
            double Vol=0.;       
        // End face
            for(int i = 0; i < Onode-1; i++){ // 1 less triangle than nodes                
                x[1] = xv0[i]*cos(theta);if (abs(x[1])<tolxy){x[1]=0;}
                y[1] = xv0[i]*sin(theta);if (abs(y[1])<tolxy){y[1]=0;}
                z[1]=LYSO_L/2;
                theta = theta + DTheta;
                x[2] = xv0[i+1]*cos(theta);if (abs(x[2])<tolxy){x[2]=0;}
                y[2] = xv0[i+1]*sin(theta);if (abs(y[2])<tolxy){y[2]=0;}
                z[2]=LYSO_L/2;
                x[0]=0.;y[0]=0.;z[0]=LYSO_L/2;
                Vol=Vol+(   -x[2]*y[1]*z[0]   +x[1]*y[2]*z[0]   +x[2]*y[0]*z[1]
                                -x[0]*y[2]*z[1]   -x[1]*y[0]*z[2]   +x[0]*y[1]*z[2])/6;
                // First and fourth irregular tetrahedron : 1/3*Ab*h=1/3*(t*L/2)*t = 10.6875
                // Second and third irregular tetrahedron : 1/3*Ab*h=1/3*(t*t/2)*L = 10.6875
                /*
                std::cout <<"prod: "<<-x[2]*y[1]*z[0]<< std::endl;
                std::cout <<"prod: "<<+x[1]*y[2]*z[0]<< std::endl;
                std::cout <<"prod: "<<+x[2]*y[0]*z[1]<< std::endl;
                std::cout <<"prod: "<<-x[0]*y[2]*z[1]<< std::endl;
                std::cout <<"prod: "<<-x[1]*y[0]*z[2]<< std::endl;
                std::cout <<"prod: "<<+x[0]*y[1]*z[2]<< std::endl;
                std::cout <<"rad: "<<xv0[i]<< " "<<xv0[i+1]<< " "<< std::endl;
                std::cout <<"x: "<<x[0]<< " "<<x[1]<< " "<<x[2]<< " "<<x[3]<< " "<< std::endl;
                std::cout <<"y: "<<y[0]<< " "<<y[1]<< " "<<y[2]<< " "<<y[3]<< " "<< std::endl;
                std::cout <<"z: "<<z[0]<< " "<<z[1]<< " "<<z[2]<< " "<<z[3]<< " "<< std::endl;
                std::cout <<"End Face: " << i <<" Accumulated Volume: "<<Vol<< std::endl;*/
            }
        // Lateral faces
            G4double z0,z1,x0,x2,x1,x3,y0,y2,y1,y3,LYSOalt;
            LYSOalt=LYSO_L/2;
            for (int j = 0; j < Znode; j++){
                theta = -Pi/2;
               for(int i = 1; i < Onode; i++){ // 1 less triangle than nodes
                    z0=+LYSOalt-LYSOalt/Znode*j;z1=+LYSOalt-LYSOalt/Znode*(j+1);

                    x0 = xv0[j*Onode-1+i]*cos(theta); if (abs(x0)<tolxy){x0=0;}
                    x2 = xv0[(j+1)*Onode-1+i]*cos(theta);if (abs(x2)<tolxy){x2=0;}
                    y0 = xv0[j*Onode-1+i]*sin(theta);if (abs(y0)<tolxy){y0=0;}
                    y2 = xv0[(j+1)*Onode-1+i]*sin(theta);if (abs(y2)<tolxy){y2=0;}
                    theta = theta + DTheta;
                    x1 = xv0[j*Onode-1+i+1]*cos(theta);if (abs(x1)<tolxy){x1=0;}
                    x3 = xv0[(j+1)*Onode-1+i+1]*cos(theta);if (abs(x3)<tolxy){x3=0;}
                    y1 = xv0[j*Onode-1+i+1]*sin(theta);if (abs(y1)<tolxy){y1=0;}
                    y3 = xv0[(j+1)*Onode-1+i+1]*sin(theta);if (abs(y3)<tolxy){y3=0;}

                    x[0]=x3;x[1]=x1;x[2]=x0;
                    y[0]=y3;y[1]=y1;y[2]=y0;
                    z[0]=z1;z[1]=z0;z[2]=z0;
                Vol=Vol+(   -x[2]*y[1]*z[0]   +x[1]*y[2]*z[0]   +x[2]*y[0]*z[1]
                                -x[0]*y[2]*z[1]   -x[1]*y[0]*z[2]   +x[0]*y[1]*z[2])/6;
                    x[0]=x3;x[1]=x0;x[2]=x2;
                    y[0]=y3;y[1]=y0;y[2]=y2;
                    z[0]=z1;z[1]=z0;z[2]=z1;
                Vol=Vol+(   -x[2]*y[1]*z[0]   +x[1]*y[2]*z[0]   +x[2]*y[0]*z[1]
                                -x[0]*y[2]*z[1]   -x[1]*y[0]*z[2]   +x[0]*y[1]*z[2])/6;
                /*
                std::cout <<"prod: "<<-x[2]*y[1]*z[0]<< std::endl;
                std::cout <<"prod: "<<+x[1]*y[2]*z[0]<< std::endl;
                std::cout <<"prod: "<<+x[2]*y[0]*z[1]<< std::endl;
                std::cout <<"prod: "<<-x[0]*y[2]*z[1]<< std::endl;
                std::cout <<"prod: "<<-x[1]*y[0]*z[2]<< std::endl;
                std::cout <<"prod: "<<+x[0]*y[1]*z[2]<< std::endl;
                std::cout <<"rad: "<<xv0[i]<< " "<<xv0[i+1]<< " "<< std::endl;
                std::cout <<"x: "<<x[0]<< " "<<x[1]<< " "<<x[2]<< " "<<x[3]<< " "<< std::endl;
                std::cout <<"y: "<<y[0]<< " "<<y[1]<< " "<<y[2]<< " "<<y[3]<< " "<< std::endl;
                std::cout <<"z: "<<z[0]<< " "<<z[1]<< " "<<z[2]<< " "<<z[3]<< " "<< std::endl;
                std::cout <<"End Face: "<< j << " " << i <<" Accumulated Volume: "<<Vol<< std::endl;*/
                }
            }

        Vol=Vol*4.;
                std::cout <<"End Volume: " <<" Accumulated Volume: "<<Vol<< std::endl;
    

return Vol;
}

/////////////////////////////////////
// Triangular mesh volume calculation
/////////////////////////////////////
double *Str2DChar(std::string strinput, G4int nn){
    // The string must be in the format "{d-d-d...}"
    //Replace {} and comma's with whitespace.
    strinput.front() = strinput.back() = ' ';
    std::replace(strinput.begin(), strinput.end(), '-', ' ');

    //Create stringstream from string.
    auto my_stream = std::istringstream(strinput);
    G4double *Arr;
    Arr = new G4double[nn];
    for (int j = 0; j < nn; j=j+1){
        my_stream >> Arr[j]; 
        std::cout << "Index: "<<j << " Array: "<< Arr[j] <<"\n";
    }

    return Arr;
}


