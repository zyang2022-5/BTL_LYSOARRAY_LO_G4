#ifndef STEPPING_HH
#define STEPPING_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

#include "construction.hh"
#include "event.hh"
#include "detector.hh"
#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4OpticalPhoton.hh"
#include "G4Args.hh"

class MySteppingAction : public G4UserSteppingAction
{
public:
    MySteppingAction(MyEventAction* eventAction,MyG4Args*);
    ~MySteppingAction();

    virtual void UserSteppingAction(const G4Step*);

private:
    MyEventAction *fEventAction;
    MyG4Args* PassArgs;
};


#endif
