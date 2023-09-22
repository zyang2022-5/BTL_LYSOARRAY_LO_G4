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
#include <ctime>
#include <string.h>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <stdio.h>
#include "G4LogicalVolume.hh"


class MyG4Args 
{

public:
    MyG4Args(int, char**);
    ~MyG4Args();

    void SetNSGAII();
    G4String GetOutName() const {return OutName;}
    G4String GetMacName() const {return MacName;}
    G4double GetVov() const {return Detection[0];}
    G4int GetSiPMmaterial() const {return SiPMmaterial;}

    G4int GetScintMat() const {return scint;}

	G4int GetESRTrue() const {return ESR;}
    G4int Getrndangle() const {return rndangle;}
    G4int GetYSymtrue() const {return NoYSym;}
    G4int Getnrep() const {return nrep;}
    G4int GetZelem() const {return Zelem;}
    G4int GetReflSiPM() const {return reflSiPM;}

    G4double GetGeom_DET_L() const {return DET_L;}
    G4double GetGeom_RESIN_L() const {return RESIN_Z;}

    G4double GetLYSO_Yield() const {return LYSOProps[0];}
    G4double GetLYSO_ScaleResolution() const {return LYSOProps[1];}
    G4double GetLYSO_RiseT() const {return LYSOProps[2];}
    G4double GetLYSO_DecayT() const {return LYSOProps[3];}
    G4double GetGeom_LYSO_L() const {return Geom_LYSO[2];}
    G4double GetGeom_LYSO_thick() const {return Geom_LYSO[0];}
    G4double GetGeom_Resin_width() const {return Geom_Resin[1];}
    G4double GetGeom_DET_T() const {return DET_T;}
    G4double GetGeom_DET_TX() const {return DET_TX;}
    G4double GetGeom_DET_TX_tol() const {return DET_TX_tol;}
    G4double GetGeom_DET_TY_tol() const {return DET_TY_tol;}

    G4double GetGeom_RESIN_H() const {return RESIN_H;}
    G4double GetGeom_RESIN_Y() const {return RESIN_Y;}
    G4double GetGeom_SiPM_Y() const {return SiPM_Y;}
    G4double Get_GLUE_Y() const {return Glue_Y;}
    void SetGeom_GLUE_Y(G4double val) { Glue_Y=val;}
    G4int GetRootCreate() const {return RootCreate;}
    G4int GetRnd_true() const {return RndGen[0];}
    G4int GetRnd_Part() const {return RndGen[1];}
    G4int GetRnd_Geom() const {return RndGen[2];}
    G4int GetTree_Hits() const {return MainTrees[0];}
    G4int GetTree_Detected() const {return MainTrees[1];}
    G4int GetTree_Stepping() const {return MainTrees[2];}
    G4int GetTree_Tracking() const {return MainTrees[3];}
    G4int GetTree_EndOfEvent() const {return MainTrees[4];}
    G4int GetTree_EndOfRun() const {return MainTrees[5];}
    G4int SaveMesh() const {return SaveSTL;}
    G4int GetGeomConfig() const {return GeomConfig;}
    G4int GetVis() const {return VisTrue;}
    G4int GetStepSize() const {return StepSize;}
    G4int GetNSGAII() const {return NSGAII;}
    G4double GetNPhotTiming() const {return NPhotTiming;}
    G4double GetPhotTiming();
    G4double GetGunX(G4int evt) const {return nGunPosX[evt];}
    G4double GetGunY(G4int evt) const {return nGunPosY[evt];}
    G4int GetTimeTrue() const {return TimeTrue;}
    G4double GetKillTL() const {return KillLTime;}
    G4int GetKillTLTrue() const {return KillLTTrue;}
    G4int GetnEvents() const {return nEvents;}
    G4int GetnResinMach() const {return ResinMach;}
    G4double GetnResinMachN() const {return ResinMachN;}
    G4int GetESRFinish() const {return ESRFinish;}

    G4int GetSpline() const {return Spline;}
    G4int GetFR4Refl() const {return FR4refl;}
    G4int GetforceBottomLine() const {return forceBottomLine;}
    G4int GetTile() const {return Tile;}

    G4double GetIncr() const {return incr;}
    G4double GetGlueZ() const {return Glue_Z;}
    G4double GetIncrS() const {return incrS;}
    G4double GetIncrV() const {return incrV;}
    G4double Getrad2Y() const {return rad2Y;}
    void GeomReinit();
    G4int FindEvents(G4String);

    void InitAllCount(){ArgLO = 0;ArgCrossTalk = 0;TotPh = 0;PhHit=0;Edep=0.;MuonEdep=0.;nPhotL=0;nPhotR=0;PhotTiming[1]=0.;MuonLYSOTrackLength=0.;}
    void InitTotPh(){TotPh = 0;}
    void InitLO(){ArgLO = 0;}
    void InitCT(){ArgCrossTalk = 0;}


    void AddNEdep(){NEdep += 1;}
    void AddMuonEdep(G4double Edepadd){MuonEdep += Edepadd;}
    void AddEdep(G4double Edepadd){Edep += Edepadd;}
    void AddMuonLYSOTrackLength(G4double TLadd){MuonLYSOTrackLength += TLadd;}

    void AddPhHit(){PhHit += 1;}
    void AddLO(){ArgLO += 1;}
    void AddCT(){ArgCrossTalk += 1;}
    void AddTP(){TotPh += 1;}
    void AddPhotR(){nPhotR += 1;}
    void AddPhotL(){nPhotL += 1;}    
    void AddPhotTiming(G4double , G4double);
    void matchSiPMf();

    
    void AddVolume(G4double Vol){Volume += Vol;}
    void InitVolume(){Volume = 0;}


    G4double GetMuonLYSOTrackLength() const { return MuonLYSOTrackLength;}

    G4int GetLO() const {return ArgLO;}
    G4int GetCT() const {return ArgCrossTalk;}
    G4int GetTP() const {return TotPh;}
    G4int GetNPhotL() const {return nPhotL;}
    G4int GetNPhotR() const {return nPhotR;}
    G4int GetPhHits() const {return PhHit;}
    G4double GetEdep() const {return Edep;}
    G4double GetMuonEdep() const {return MuonEdep;}
    G4int GetNEdep() const {return NEdep;}
    
    G4double GetPartDir(G4int i){return PartDir[i];}
    G4double GetPartXDispl(){return PartDisplX;}

    void FillEvtLO(G4int evt, G4double val){nEventLO[evt]=val;}  
    void FillEvtLSt(G4int evt, G4double val){nEventLSt[evt]=val;}  

    void FillEvtLD(G4int evt, G4double val){nEventLD[evt]=val;}  
    G4double GetEvtLO(G4int evt) const {return nEventLO[evt];}  
    G4double GetEvtLSt(G4int evt) const {return nEventLSt[evt];}  

    void FillEvtTim(G4int evt, G4double val){nEventTiming[evt]=val;}  
    G4double GetEvtTim(G4int evt) const {return nEventLO[evt];}  
    void FillAvgTim(G4int);
    void FillAvgLO(G4int); 
    void FillStdTim(G4int);
    void FillStdLO(G4int);
    void DefaultRadiusVect();
    void SetRadiusVect(G4double*, G4int, G4int);
    
    G4double GetLOIQR() const {return IQRLO;}  
    G4double GetLDIQR() const {return IQRLD;}  
    G4double GetLStIQR() const {return IQRLSt;}  

    G4double GetLOAvg(G4int runid) const {return nRuntLOAvg[runid];}  
    G4double GetLOStd(G4int runid) const {return nRuntLOStd[runid];}  
    
    G4double GetLOP50(G4int runid) const {return nRuntLOP50[runid];}  
    G4double GetLCP50(G4int runid) const {return nRuntLCP50[runid];}  
    G4double GetLStAvg(G4int runid) const {return nRuntLStAvg[runid];}  
    G4double GetLStP50(G4int runid) const {return nRuntLStP50[runid];}  
    
    G4double GetLDAvg(G4int runid) const {return nRuntLDAvg[runid];}  
    G4double GetLDStd(G4int runid) const {return nRuntLDStd[runid];}  
    G4double GetTimAvg(G4int runid) const {return nRunTimingAvg[runid];}  
    G4double GetTimStd(G4int runid) const {return nRunTimingStd[runid];}  
    G4double GetnEvtEdep(G4int runid) const {return nEdepEvts[runid];}  
    G4int GetZnode() const {return Znode;}
    G4int GetnX() const {return nX;}
    G4int Getnodesec() const {return nodesec;}
    G4double GetVolume() const {return Volume;}
    G4int SetVolume(G4double V){Volume=V;}
    G4int GetOnode() const {return Onode;}
    int GetRunEvt() const {return runevt;}
    G4double* GetNodeRadValues() const {return xv0;}
    G4int GetMuonFlag() const {return Muon;}
    void SetLYSOVolumeXY();
    void SetCoordVect();
    void SetYVect(G4double*);
    G4double GetXvec(int ind) const {return xv[ind];}
    G4double GetYvec(int ind) const {return yv[ind];}
    G4double GetYvecincr(int ind) const {return yvincr[ind];}
    G4double* GetYincr() const {return yincr;}
    G4int GetYstr() const {return Ystr;}
    G4int GetGmshView() const {return GmshView;}

    G4double GetGeomIndv(G4int runid) const {return RndGenIndv[runid];}  
    G4double GetSZloc(){return SZ_loc;}
    G4int GetG4SipmState() const {return RealSipm;}


    bool IsVolumeInList(const G4LogicalVolume* volume);
    void InitfScoringVolumeVec(std::vector<G4LogicalVolume*> fScoringVolumeVecinit) {fScoringVolumeVec=fScoringVolumeVecinit;}  
    void PushfScoringVolumeVec(G4LogicalVolume *LYSOTet_Logic) {fScoringVolumeVec.push_back(LYSOTet_Logic);}  

    
private:

    // Default values modifiable by arguments and able to be returned!!!
    G4int Oin=0; 
    G4int nrep=0; 
    G4int MainTrees[7]={0, 1, 0, 0, 1, 1, 1};// Options to write(1)/orNot(0) the different output trees {Arrivals,Detected,Stepping,Tracking,EndOfEvent,DigiCollection}
    G4double LYSOProps[4]={40000.,0.,60.,39.1};//Options to modify default LYSO properties {Yield,ScaleResolution,RiseTime,DecayTime}
    G4int RndGen[3]={1,1,1};             // Options regarding the random generator {Init,Particle,Geometry}
    G4double Geom_LYSO[3]={3./2.,3./2.,57./2.};//Options to modify default Geom properties {...}
    G4double Geom_Resin[3]={3./2.,51.5/2.,57./2.};//Options to modify default Geom properties {...}
    G4String OutName, MacName;
    G4double Detection[2]={3.5,1};//Options to modify photon detection
    G4String DefOutName="DefaultOutputName_Run";
    G4double KillSim[4]={0.,0.,0.,0.}; // Options on how to kill the simulation {Method(0 = all tracks have been killed, 1= nphotons have been detected at each SiPM, 2= x time has passed, 3= kills all photons after x local time, 4= use all parameters to kill the simulation) ,nphotons, global time, local time...}
    G4double StepSize=0.;    
    G4int GeomConfig=11;
    G4int VisTrue=1;
    G4int TimeTrue=1;
    G4int NPhotTiming=10;
    G4double AvgTiming;
    G4double PhotTiming[2]={10.,0.};
    G4double TListR[10],TListL[10];
    G4double KillLTime=200;
    G4int KillLTTrue=0;
    G4int nEvents=0;
    G4double *nEventTiming, *nEventLO,*nEventLSt, *nEventLD, *nRunTimingAvg, *nRuntLOAvg,*nRuntLOP50,*nRuntLCP50,*nRuntLStAvg,*nRuntLStP50, *nRuntLDAvg, *nRunTimingStd, *nRuntLOStd, *nRuntLDStd, *nGunPosX, *nGunPosY, *xv, *yv, *yincr, *yvincr;        
    G4int *nEdepEvts;
    G4double incr=0;
    G4double incrS=0;
    G4double incrV=0;
    G4int Znode=1;
    G4int nX=3;
    G4int nodesec=4;
    G4int Onode=5;
    G4double* xv0 = NULL;
    G4int Muon = 0;
    G4int dateflag;
    G4int RootCreate=1;
    G4int NSGAII=0;
    G4int rad2Y=0;
// Counters!!!
    G4int ArgLO=0;
    G4int ArgCrossTalk=0;
    G4int TotPh=0;
    G4int PhHit=0;
    G4double Edep=0.;
    G4double MuonEdep=0.;
    G4int nPhotR=0;
    G4int nPhotL=0;
    G4int NEdep=0;
    G4int Ystr=0;
    G4double Volume=0.;
    G4int ResinMach=0;
    G4double ResinMachN=-1;
    G4int SiPMmatch=0;
    G4int SiPMmaterial=2;
    G4int AreaCte=0;
    G4int Spline=1;
    G4int reflSiPM=0;

    G4double DET_YMAX=3;
    G4double DET_XMAX=3;

    G4double DET_L=0.3/2.;

    G4double DET_T=3./2;
    G4double RESIN_Z=0.25;

    G4double DET_TX=3./2;
    G4double DET_TX_tol=0.;
    G4double DET_TY_tol=0.;

    G4double Glue_Y=3.;
    G4double Glue_Z=0.2;
    G4double RESIN_H=6.5/2;
    G4double RESIN_Y=RESIN_H-0.5-Geom_LYSO[0];
    G4double SiPM_Y=+Geom_LYSO[0]+0.5-RESIN_H;
    std::string YposStr;
    int runevt=0;
    char datechar [22];
    int Zelem=10;
    int NoYSym=0;
    G4int FR4refl=0;
    
    G4double PartDir[3];
    G4double PartDisplX=0;
    G4double PartAngle=0;
    G4int rndangle=0;
    G4int SaveSTL=0;
    G4int scint=1;
    G4int ESR=1;
	G4int RndGenIndv[6] = {0};
	G4int forceBottomLine = 0;
	G4int Tile = 0;
    G4double SZ_loc=0.5;
    G4int GmshView = 0;
    G4double MuonLYSOTrackLength=0;
    G4int ESRFinish=0;
	std::vector<G4LogicalVolume*> fScoringVolumeVec;
    G4double IQRLO = 0;
    G4double IQRLD = 0;
    G4double IQRLSt = 0;
    G4int RealSipm = 0;

};    

#endif
