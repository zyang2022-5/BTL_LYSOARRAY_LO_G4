#include "tracking.hh"


MyTrackingAction::MyTrackingAction(MyEventAction *eventAction)
{
    trPhCount=0.;
    fEventAction = eventAction;
}

MyTrackingAction::~MyTrackingAction()
{}

void MyTrackingAction::PreUserTrackingAction(const G4Track* track)
{
//    if (track->GetTrackLength() < 90.)
//    {fpTrackingManager->SetStoreTrajectory(1);}
//    else
//    {fpTrackingManager->SetStoreTrajectory(0);}
	trPhCount=0.;
}

void MyTrackingAction::PostUserTrackingAction(const G4Track*)
{

  // The user tracking action class holds the pointer to the tracking manager:
  // fpTrackingManager
    
  // From the tracking manager we can retrieve the secondary track vector,
  // which is a container class for tracks:
    G4TrackVector* secTracks = fpTrackingManager -> GimmeSecondaries();
    G4double TlengthK;
    G4double TimeK;
    G4VPhysicalVolume *VolK;
    G4ThreeVector TranslVol;
    G4Track *track ;
    G4int StEnd;
    G4StepPoint *preSP;
    G4AnalysisManager *man = G4AnalysisManager::Instance();
      if(secTracks) { 
         size_t nmbSecTracks = (*secTracks).size();       

         for (size_t i = 0; i < nmbSecTracks; i++) { 
            if ((*secTracks)[i] -> GetDefinition() == G4OpticalPhoton::Definition()) {
                trPhCount++;
                fEventAction->AddPh(1.);
	    }
        }

    }
}
