#ifndef G4SIM_HH
#define G4SIM_HH

#include <iostream>
#include "G4RunManager.hh" /* Run */
#include "G4UImanager.hh"

///////* Vis -> Visualizer*//////
#include "G4VisManager.hh"      /* Vis -> Visualizer*/
#include "G4VisExecutive.hh"    /* Vis -> Visualizer*/
#include "G4UIExecutive.hh"     /* Vis -> Visualizer*/
///////* Vis -> Visualizer*//////
/*Simulation created files*/
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include <cstdlib>
#include "G4Args.hh"
#include <unistd.h> /* for exit() */

class G4simulation 
{

public:
    G4simulation(int, char**, G4int Onode = 5, G4int Znode = 1 , G4double* radp = NULL);
    ~G4simulation();
    G4double GetAvgLO(){}
    double GetLO_avg(int runid){return ArgInp->GetLOAvg(runid);}
    double GetLO_std(int runid){return ArgInp->GetLOStd(runid);}
    double GetLD_avg(int runid){return ArgInp->GetLDAvg(runid);}
    double GetLD_std(int runid){return ArgInp->GetLDStd(runid);}

private:

    MyG4Args *ArgInp;
};    

#endif
