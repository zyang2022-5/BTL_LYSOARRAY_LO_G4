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

#include <set>

// The Gmsh C++ API is entirely defined in the `gmsh.h' header (which contains
// the full documentation of all the functions in the API):
#include <gmsh.h>

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
// String char to array
/////////////////////////////////////
double *Str2DChar(std::string strinput, G4int nn){
    // The string must be in the format "{d-d-d...}"
    //Replace {} and comma's with whitespace.
    std::cout << "Reading String: " <<"\n";
    strinput.front() = strinput.back() = ' ';
    std::cout << strinput <<"\n";
    std::replace(strinput.begin(), strinput.end(), '-', ' ');
    std::cout << strinput <<"\n";
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

/////////////////////////////////////
// Triangular mesh volume calculation
/////////////////////////////////////
double TetraVolume(G4double *x,G4double *y,G4double *z){
		
		G4double x1,x2,x3,x4,y1,y2,y3,y4,z1,z2,z3,z4;
		
		x1=x[0];y1=y[0];z1=z[0];
		x2=x[1];y2=y[1];z2=z[1];
		x3=x[2];y3=y[2];z3=z[2];
		x4=x[3];y4=y[3];z4=z[3];
		double det1, det2, det3;
		
		det1=+(x1-x4)*( (y2-y4)*(z3-z4)-(y3-y4)*(z2-z4) );
		det2=-(y1-y4)*( (x2-x4)*(z3-z4)-(x3-x4)*(z2-z4) );
		det3=+(z1-z4)*( (x2-x4)*(y3-y4)-(x3-x4)*(y2-y4) );

		std::cout<< "Coords Vol: "
		<< x[0]<<" "<< x[1]<<" "<< x[2]<<" "<< x[3]<<" "
		<< y[0]<<" "<< y[1]<<" "<< y[2]<<" "<< y[3]<<" "
		<< z[0]<<" "<< z[1]<<" "<< z[2]<<" "<< z[3]<<" "
		<< std::endl;
		
		std::cout<< "Dets Vol: "
		<< det1<<" "<< det2<<" "<< det3<<" "
		<< std::endl;

		G4double vol;
		vol= abs(1/6.*(det1+det2+det3)) ;
		std::cout<< "Vol: "<< vol<< std::endl;
		return vol;
}


std::vector<std::vector<double>> appendReversedInitialVector(const std::vector<std::vector<double>>& inputVector)
{
    std::vector<std::vector<double>> result = inputVector;  // Make a copy of the input vector

    if (!result.empty())
    {
        std::vector<double>& initialVector = result.front();  // Get a reference to the initial vector

        if (initialVector.size() > 1)
        {
            std::reverse(initialVector.begin(), initialVector.end() - 1);  // Reverse the initial vector excluding its last value
        }
    }

    return result;  // Return the modified vector
}

std::vector<std::vector<double>> reverseSubvectors(const std::vector<std::vector<double>>& inputVector) {
    std::vector<std::vector<double>> resultVector = inputVector; // Create a copy of the input vector
    
    std::reverse(resultVector.begin(), resultVector.end()); // Reverse the order of the subvectors
    
    return resultVector; // Return the resulting vector of vectors
}

std::vector<std::vector<double>> combineAllIndices(const std::vector<double>& vec1, const std::vector<double>& vec2) {
    std::vector<std::vector<double>> combinedVector;
    
    size_t size = vec1.size();
    
    for (size_t n = 0; n < size; ++n) {
        std::vector<double> innerVec;
        innerVec.push_back(vec2[n]);
        innerVec.push_back(vec2[(n + 1) % size]);
        innerVec.push_back(vec1[(n + 1) % size]);
        innerVec.push_back(vec1[n]);
        
        combinedVector.push_back(innerVec);
    }
    
    return combinedVector;
}

std::vector<std::vector<int>> combineAllIndicesint(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<std::vector<int>> combinedVector;
    
    size_t size = vec1.size();
    
    for (size_t n = 0; n < size; ++n) {
        std::vector<int> innerVec;
        innerVec.push_back(vec1[n]);
        innerVec.push_back(vec1[(n + 1) % size]);
        innerVec.push_back(vec2[(n + 1) % size]);
        innerVec.push_back(vec2[n]);
        
        combinedVector.push_back(innerVec);
    }
    
    return combinedVector;
}
std::vector<std::vector<double>> calculatePoints(const std::vector<double>& Y, double Xin, double Zmin, double Zmax, double Yzero)
{
    std::vector<std::vector<double>> points;

    // Calculate the step size for Z values
    double step = (Zmax - Zmin) / (Y.size() - 1);

    // Create symmetrical points with respect to Zmin
    for (int i = Y.size() - 1; i >= 1; --i)
    {
        double Z = Zmin - (i ) * step;  // Symmetrical Z values
        points.push_back({Xin, Y[i], Z});
        std::cout<< Xin<< " "<< Y[i]<< " "<< Z<< std::endl;
    }

    for (size_t i = 0; i < Y.size(); ++i)
    {
        double Z = Zmin + i * step;
        points.push_back({Xin, Y[i], Z});
        std::cout<< Xin<< " "<< Y[i]<< " "<< Z<< std::endl;
    }


    std::vector<std::vector<double>> additionalPoints = {{Xin, Yzero, Zmax}, {Xin, Yzero, -Zmax}};
    points.insert(points.end(), additionalPoints.begin(), additionalPoints.end());

    return points;
}

std::vector<int> createGmshPoints(const std::vector<std::vector<double>>& points, int taginit)
{
    std::vector<int> pointTags;

    // Create points
    int count = 1 ;
    for (const auto& point : points)
    {
        double x = point[0];
        double y = point[1];
        double z = point[2];

        int tag;
        tag = taginit+count;
        gmsh::model::geo::addPoint(x, y, z, 0.0, tag);
        pointTags.push_back(tag);
        count=count+1;
    }

    return pointTags;
}

std::vector<int> createGmshLines(const std::vector<int>& pointTags, int taginit, const std::set<int>& excludedIndices = {})
{
    std::vector<int> lineTags;
    int lineTag;

    int count = 1;
    int pointCount = pointTags.size();

    // Create lines between successive points, excluding the lines with excluded indices
    for (int i = 0; i < pointCount - 1; i++)
    {
        if (excludedIndices.count(i) > 0)
            continue;

        lineTag = taginit + count;
        std::cout<<lineTag<<" ";
        gmsh::model::geo::addLine(pointTags[i], pointTags[i + 1], lineTag);
        lineTags.push_back(lineTag);
        count++;
    }

    // Create line between the last and first points, excluding the line with the excluded index
    if (pointCount > 1 && excludedIndices.count(pointCount - 1) == 0)
    {
        lineTag = taginit + count;
        std::cout<<lineTag<<" ";
        gmsh::model::geo::addLine(pointTags[pointCount - 1], pointTags[0], lineTag);
        lineTags.push_back(lineTag);
    }
	std::cout<<std::endl;
    return lineTags;
}


int createGmshSurface(const std::vector<int>& lineTags, int taginit)
{
    int surfaceTag = taginit;

    // Create line loop
    int lineLoopTag = surfaceTag;
    gmsh::model::geo::addCurveLoop(lineTags, lineLoopTag);

    // Create surface
    gmsh::model::geo::addPlaneSurface({lineLoopTag}, surfaceTag);

    return surfaceTag;
}
std::vector<double> extractRange(const std::vector<double>& Y_all, int sec, int nodesec)
{
    std::vector<double> Y(Y_all.begin() + sec*nodesec, Y_all.begin() + sec*nodesec + nodesec);
    return Y;
}
void printVectorOfVectors(const std::vector<std::vector<int>>& vec)
{
	int i=0;
    for (const auto& innerVec : vec)
    {
		std::cout<< i<< " ";
        for (const auto& value : innerVec)
        {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        i++;
    }
}

void printVector(const std::vector<int>& vec)
{
    for (const auto& value : vec)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
void printVectordouble(const std::vector<double>& vec)
{
    for (const auto& value : vec)
    {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}
std::vector<int> getElementsAtIndex(const std::vector<std::vector<int>>& vec, int index, int a, int b)
{
    std::vector<int> result;

    for (int i = a; i <= b; ++i)
    {
        if (i >= 0 && i < vec.size())
        {
            const auto& subvector = vec[i];
            if (index >= 0 && index < subvector.size())
            {
                result.push_back(subvector[index]);
            }
        }
    }

    return result;
}

std::vector<double> generateEquispacedSegments(double Xmin, double Xmax, int numElements)
{
    std::vector<double> segments;

    if (numElements <= 1)
    {
        segments.push_back(Xmin);
        return segments;
    }

    double step = (Xmax - Xmin) / (numElements - 1);
    for (int i = 0; i < numElements; ++i)
    {
        double X = Xmin + i * step;
        segments.push_back(X);
    }

    return segments;
}

