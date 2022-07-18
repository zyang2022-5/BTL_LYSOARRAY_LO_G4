#include "tracking.hh"

MyTrackingAction::MyTrackingAction(MyEventAction *eventAction,MyG4Args *MainArgs)
{
    PassArgs=MainArgs;

    fEventAction = eventAction;
}

MyTrackingAction::~MyTrackingAction()
{}

void MyTrackingAction::PreUserTrackingAction(const G4Track*)
{

}

void MyTrackingAction::PostUserTrackingAction(const G4Track*)
{
    // The user tracking action class holds the pointer to the tracking manager:
    // fpTrackingManager
    
    // From the tracking manager we can retrieve the secondary track vector,
    // which is a container class for tracks:
    G4TrackVector* secTracks = fpTrackingManager -> GimmeSecondaries();
    // You can use the secTracks vector to retrieve the number of secondary 
    // electrons
      if (secTracks) { 
         size_t nmbSecTracks = (*secTracks).size();       

         for (size_t i = 0; i < nmbSecTracks; i++) { 
            if ((*secTracks)[i]->GetDefinition() == G4OpticalPhoton::Definition()) {
                PassArgs->AddTP();
            }
        }
    }
}
