#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
//#include "construction.hh"
#include "Randomize.hh"
//#include "sim.cc"


#include <string.h>
#include "G4AnalysisManager.hh"
//#include "g4root.hh" // NOT FOUND, G4AnalysisManager include as solution

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction();
    ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
private :
    G4String command ;
};

#endif
