#ifndef TRACKING_HH
#define TRACKING_HH

#include "G4UserEventAction.hh"
#include "G4Track.hh"
#include "G4AnalysisManager.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4UserTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "event.hh"
//#include "groot.hh"
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

/*
#include "UserTrackingAction.hh"
#include "G4TrackingManager.hh"
#include "G4Electron.hh"


UserTrackingAction::UserTrackingAction() :
    counter(0) {

}


void UserTrackingAction::PostUserTrackingAction(const G4Track*) {

  // The user tracking action class holds the pointer to the tracking manager:
  // fpTrackingManager

  // From the tracking manager we can retrieve the secondary track vector,
  // which is a container class for tracks:
  G4TrackVector* secTracks = fpTrackingManager -> GimmeSecondaries();

  // You can use the secTracks vector to retrieve the number of secondary 
  // electrons
  if(secTracks) { 
     size_t nmbSecTracks = (*secTracks).size();       

     for(size_t i = 0; i < nmbSecTracks; i++) { 
        if((*secTracks)[i] -> GetDefinition() == G4Electron::Definition()) 
              counter++;
     }
  }
}*/
