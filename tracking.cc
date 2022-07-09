#include "tracking.hh"


MyTrackingAction::MyTrackingAction(MyEventAction *eventAction,MyG4Args *MainArgs)
{
    PassArgs=MainArgs;
    trPhCount=0.;
    fEventAction = eventAction;
}

MyTrackingAction::~MyTrackingAction()
{}

void MyTrackingAction::PreUserTrackingAction(const G4Track*)
{
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
  // You can use the secTracks vector to retrieve the number of secondary 
  // electrons
      if(secTracks) { 
         size_t nmbSecTracks = (*secTracks).size();       

         for(size_t i = 0; i < nmbSecTracks; i++) { 
            if((*secTracks)[i] -> GetDefinition() == G4Gamma::Definition()) 
                    trPhCount++;
                    fEventAction->AddPh(1.);/*
                    track = (*secTracks)[i];
                    //if(*secTracks)[i] -> GetTrackStatus() == iskilled) { // How to define if it is killed/alive??? Not needed, the PostUser... provides info when the track is about to end
                        const G4Step* aStep  = track->GetStep();                        
                        TlengthK =  track->GetTrackLength();
                        TimeK    =  track->GetGlobalTime();
                        VolK = track->GetVolume();
                        StEnd=track-> GetCurrentStepNumber();
                        preSP = aStep->GetPreStepPoint();
                        //TranslVol     =  preSP->GetPosition();
                        TranslVol = VolK ->GetTranslation();
                        man->FillNtupleDColumn(3, 0,  TlengthK);
                        man->FillNtupleDColumn(3, 1,  TimeK);// D==double
                        man->FillNtupleDColumn(3, 2,  TranslVol[0]);
                        man->FillNtupleDColumn(3, 3,  TranslVol[1]);
                        man->FillNtupleDColumn(3, 4,  TranslVol[2]);
                        man->AddNtupleRow(3);
                    //}*/
            }

          }

 
}
