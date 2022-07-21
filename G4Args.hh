#ifndef G4ARGS_HH
#define G4ARGS_HH
#include "G4RunManager.hh" /* Run */
#include <cstdlib>
#include <iostream>
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include "util.hh"
#include "G4UImanager.hh"
#include <fstream>
#include <cstring>


class MyG4Args 
{

public:
    MyG4Args(int, char**);
    ~MyG4Args();

    G4String GetOutName() const {return OutName;}
    G4String GetMacName() const {return MacName;}
    G4double GetVov() const {return Detection[0];}
    G4int Getnrep() const {return nrep;}
    G4double GetLYSO_Yield() const {return LYSOProps[0];}
    G4double GetLYSO_ScaleResolution() const {return LYSOProps[1];}
    G4double GetLYSO_RiseT() const {return LYSOProps[2];}
    G4double GetLYSO_DecayT() const {return LYSOProps[3];}
    G4double GetGeom_LYSO_L() const {return Geom_LYSO[2];}
    G4double GetGeom_LYSO_thick() const {return Geom_LYSO[0];}
    G4int GetRnd_true() const {return RndGen[0];}
    G4int GetRnd_Part() const {return RndGen[1];}
    G4int GetRnd_Geom() const {return RndGen[2];}
    G4int GetTree_Hits() const {return MainTrees[0];}
    G4int GetTree_Detected() const {return MainTrees[1];}
    G4int GetTree_Stepping() const {return MainTrees[2];}
    G4int GetTree_Tracking() const {return MainTrees[3];}
    G4int GetTree_EndOfEvent() const {return MainTrees[4];}
    G4int GetTree_EndOfRun() const {return MainTrees[5];}
    G4int GetGeomConfig() const {return GeomConfig;}
    G4int GetVis() const {return VisTrue;}
    G4int GetStepSize() const {return StepSize;}
    G4double GetNPhotTiming() const {return NPhotTiming;}
    G4double GetPhotTiming();
    G4int GetTimeTrue() const {return TimeTrue;}
    G4double GetKillTL() const {return KillLTime;}
    G4int GetKillTLTrue() const {return KillLTTrue;}
    G4int GetnEvents() const {return nEvents;}
    G4double GetIncr() const {return incr;}
    void GeomReinit();
    G4int FindEvents(G4String);

    void InitAllCount(){ArgLO = 0;ArgCrossTalk = 0;TotPh = 0;PhHit=0;Edep=0.;nPhotL=0;nPhotR=0;PhotTiming[1]=0.;}
    void InitTotPh(){TotPh = 0;}
    void InitLO(){ArgLO = 0;}
    void InitCT(){ArgCrossTalk = 0;}


    void AddNEdep(){NEdep += 1;}
    void AddEdep(G4double Edepadd){Edep += Edepadd;}
    void AddPhHit(){PhHit += 1;}
    void AddLO(){ArgLO += 1;}
    void AddCT(){ArgCrossTalk += 1;}
    void AddTP(){TotPh += 1;}
    void AddPhotR(){nPhotR += 1;}
    void AddPhotL(){nPhotL += 1;}    
    void AddPhotTiming(G4double , G4double);
    G4int GetLO() const {return ArgLO;}
    G4int GetCT() const {return ArgCrossTalk;}
    G4int GetTP() const {return TotPh;}
    G4int GetNPhotL() const {return nPhotL;}
    G4int GetNPhotR() const {return nPhotR;}
    G4int GetPhHits() const {return PhHit;}
    G4double GetEdep() const {return Edep;}
    G4int GetNEdep() const {return NEdep;}

    void FillEvtLO(G4int evt, G4double val){nEventLO[evt]=val;}  
    G4double GetEvtLO(G4int evt) const {return nEventLO[evt];}  
    void FillEvtTim(G4int evt, G4double val){nEventTiming[evt]=val;}  
    G4double GetEvtTim(G4int evt) const {return nEventLO[evt];}  
    void FillAvgTim(G4int);
    void FillAvgLO(G4int); 
    void FillStdTim(G4int);
    void FillStdLO(G4int);
    G4double GetLOAvg(G4int runid) const {return nRuntLOAvg[runid];}  
    G4double GetLOStd(G4int runid) const {return nRuntLOStd[runid];}  
    G4double GetTimAvg(G4int runid) const {return nRunTimingAvg[runid];}  
    G4double GetTimStd(G4int runid) const {return nRunTimingStd[runid];}  
    G4double GetnEvtEdep(G4int runid) const {return nEdepEvts[runid];}  
private:

    // Default values modifiable by arguments and able to be returned!!!
    G4int Oin=0; 
    G4int nrep=0; 
    G4int MainTrees[6]={0, 1, 0, 0, 1, 0};// Options to write(1)/orNot(0) the different output trees {Arrivals,Detected,Stepping,Tracking,EndOfEvent}
    G4double LYSOProps[4]={40000.,0.,60.,39.1};//Options to modify default LYSO properties {Yield,ScaleResolution,RiseTime,DecayTime}
    G4int RndGen[3]={1,1,1};             // Options regarding the random generator {Init,Particle,Geometry}
    G4double Geom_LYSO[3]={3./2.,3./2.,57./2.};//Options to modify default Geom properties {...}
    G4double Geom_Resin[3]={3./2.,51.5/2.,57./2.};//Options to modify default Geom properties {...}
    G4String OutName, MacName;
    G4double Detection[2]={3.5,1};//Options to modify photon detection
    G4String DefOutName="DefaultOutputName_Run";
    G4double KillSim[4]={0.,0.,0.,0.}; // Options on how to kill the simulation {Method(0 = all tracks have been killed, 1= nphotons have been detected at each SiPM, 2= x time has passed, 3= kills all photons after x local time, 4= use all parameters to kill the simulation) ,nphotons, global time, local time...}
    G4double StepSize=0.;    
    G4int GeomConfig=1;
    G4int VisTrue=1;
    G4int TimeTrue=1;
    G4int NPhotTiming=10;
    G4double AvgTiming;
    G4double PhotTiming[2]={10.,0.};
    G4double TListR[10],TListL[10];
    G4double KillLTime=200;
    G4int KillLTTrue=0;
    G4int nEvents=0;
    G4double *nEventTiming, *nEventLO, *nRunTimingAvg, *nRuntLOAvg, *nRunTimingStd, *nRuntLOStd;        
    G4int *nEdepEvts;
    G4double incr=0;

// Counters!!!
    G4int ArgLO=0;
    G4int ArgCrossTalk=0;
    G4int TotPh=0;
    G4int PhHit=0;
    G4double Edep=0.;
    G4int nPhotR=0;
    G4int nPhotL=0;
    G4int NEdep=0;


};    

#endif
