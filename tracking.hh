#ifndef TRACKING_HH
#define TRACKING_HH

#include "G4UserEventAction.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4OpticalPhoton.hh"
#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "event.hh"
#include "G4Args.hh"
#include "run.hh"

class MyTrackingAction : public G4UserTrackingAction
{
public :
    MyTrackingAction(MyEventAction* eventAction,MyG4Args*);
    ~MyTrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

private :
    G4double trPhCount;
    MyEventAction *fEventAction;
    MyG4Args* PassArgs;
};

#endif
