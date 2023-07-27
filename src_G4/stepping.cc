#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction,MyG4Args *MainArgs)
{
    PassArgs=MainArgs;
    fEventAction = eventAction;

}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
    G4LogicalVolume *volume =step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
//////////////////////////////////////////////////////////////////////////////
// Gamma Information
//////////////////////////////////////////////////////////////////////////////

    /*if (volume == fDetectorVolume) {
        fEventAction->AddLO(1.);
        G4ThreeVector momPhoton = preStepPoint->GetMomentum();
    }*/


//////////////////////////////////////////////////////////////////////////////
// General defitinions
//////////////////////////////////////////////////////////////////////////////

	if(PassArgs-> GetGeomConfig()==11){int vec =1; std::vector<G4LogicalVolume*> fScoringVolumeVec;fScoringVolumeVec  = detectorConstruction->GetScoringVolumeVec();}
	else{G4LogicalVolume *fScoringVolume  = detectorConstruction->GetScoringVolume();int vec=0;}
    
    //G4LogicalVolume *fDetectorVolume  = detectorConstruction->GetDetectorVolume();
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4StepPoint *postStepPoint = step->GetPostStepPoint();
    G4double TlengthK;
    G4double TimeK=preStepPoint->GetGlobalTime();
    G4double TimeKL=preStepPoint->GetLocalTime();
    G4double TimeKLLim=PassArgs->GetKillTL();
    G4ThreeVector TranslVol;
    G4ThreeVector TranslVol2;
    G4Track *track = step -> GetTrack();

//////////////////////////////////////////////////////////////////////////////
// Killed tracks Information and actions
//////////////////////////////////////////////////////////////////////////////

    if(PassArgs->GetKillTLTrue()==1 && TimeKL/ps>TimeKLLim && PassArgs->GetEdep()>0){
        G4cout<< "Track killed at time: "<< TimeKL/ps << G4endl;
        track -> SetTrackStatus(fStopAndKill); 
    }

    if(PassArgs->GetTree_Stepping()==1){
        if(track -> GetTrackStatus() != fAlive) {                     
                                TlengthK =  track->GetTrackLength();
                                TimeK=preStepPoint->GetGlobalTime();
                                //VolK = track->GetVolume();
                                //StEnd=track-> GetCurrentStepNumber();
                                //preSP = aStep->GetPreStepPoint();
                                TranslVol     =  preStepPoint->GetPosition();
                                //TranslVol = VolK ->GetTranslation();
                                man->FillNtupleDColumn(2, 0,  TlengthK/mm);
                                man->FillNtupleDColumn(2, 1,  TimeK/ps);// D==double
                                man->FillNtupleDColumn(2, 2,  TranslVol[0]/mm);
                                man->FillNtupleDColumn(2, 3,  TranslVol[1]/mm);
                                man->FillNtupleDColumn(2, 4,  TranslVol[2]/mm);
                                man->AddNtupleRow(2);
        }
    }

//////////////////////////////////////////////////////////////////////////////
// Energy Deposition Information
//////////////////////////////////////////////////////////////////////////////

    if(step -> GetTrack() -> GetDefinition() == G4MuonPlus::Definition()) {
            G4double edepM = step->GetTotalEnergyDeposit();  
            PassArgs->AddMuonEdep(edepM);
            
        // Calculate track length for muons within the LYSO volume
        G4LogicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
        if (PassArgs->GetGeomConfig()==11 || PassArgs->GetGeomConfig()==13){

			if (PassArgs->IsVolumeInList(currentVolume)) {
				G4double trackLengthMuon = step->GetStepLength();
				PassArgs->AddMuonLYSOTrackLength(trackLengthMuon);
				// Use trackLengthMuon for further calculations or store it as needed
			}
		}
    }

    if(step -> GetTrack() -> GetDefinition() != G4OpticalPhoton::Definition()) {
        //if(volume != fScoringVolume)
        //    return;
        TranslVol     =  preStepPoint->GetPosition();
        if (PassArgs->GetGeomConfig()==3 || PassArgs->GetGeomConfig()==13){
            if(TranslVol[0]/mm<-0.01&&  TranslVol[0]/mm>-1*(PassArgs->GetGeom_LYSO_thick())*2){
                G4double edep = step->GetTotalEnergyDeposit();  
                PassArgs->AddEdep(edep);
                PassArgs->AddNEdep();
            }
        }else{
            G4double edep = step->GetTotalEnergyDeposit();  
            PassArgs->AddEdep(edep);
            PassArgs->AddNEdep();
        }

    }
}
