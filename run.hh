#ifndef RUN_HH
#define RUN_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4UImanager.hh"
//#include "construction.hh"
#include "Randomize.hh"
//#include "sim.cc"
#include "G4GenericMessenger.hh"

#include <string.h>
#include "G4AnalysisManager.hh"
//#include "g4root.hh" // NOT FOUND, G4AnalysisManager include as solution

class MyRunAction : public G4UserRunAction
{
public:
    MyRunAction(G4String OutName);
    ~MyRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    G4String OutputName, file_name;
private :
    G4String command;
    G4GenericMessenger *fMessenger;
};

#endif
