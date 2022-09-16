#ifndef EVENT_HH
#define EVENT_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4AnalysisManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"
//#include "groot.hh"

#include "run.hh"
#include "construction.hh"
#include <string.h>
#include "G4Args.hh"

class MyEventAction : public G4UserEventAction
{
public :
    MyEventAction(MyRunAction*,MyG4Args*);
    ~MyEventAction();

    virtual void BeginOfEventAction(const G4Event*);
    virtual void EndOfEventAction(const G4Event*);

    void AddEdep(G4double edep){fEdep += edep;}
    void AddPh(G4double PhCount){fPhCount += PhCount;}
    void AddLO(G4double LOc){fLO += LOc;}

private :
    G4double fEdep;
    G4double fPhCount;
    G4double fLO,GenX,GenZ;
    G4double PDE420;
    G4PhysicsOrderedFreeVector *PDE;
    G4double PXd ;
    G4double PZd ;
    G4String PX ;
    G4String PZ ;
    G4String GLUE_Lstr;
    G4String command ;
    MyG4Args* PassArgs;

};

#endif
