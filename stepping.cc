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
// Electron Information
//////////////////////////////////////////////////////////////////////////////
    G4LogicalVolume *fScoringVolume  = detectorConstruction->GetScoringVolume();
    G4LogicalVolume *fDetectorVolume  = detectorConstruction->GetDetectorVolume();
    //if(step -> GetTrack() -> GetTrackID() != 1) {
    if(step -> GetTrack() -> GetDefinition() == G4Electron::Definition()) {
        if(volume != fScoringVolume)
            return;

        G4double edep = step->GetTotalEnergyDeposit();  
        fEventAction->AddEdep(edep);
    }
//////////////////////////////////////////////////////////////////////////////
// Gamma Information
//////////////////////////////////////////////////////////////////////////////
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    G4double TlengthK;
    G4double TimeK;
    G4VPhysicalVolume *VolK;
    G4ThreeVector TranslVol;
    G4Track *track = step -> GetTrack();

    if(volume == fDetectorVolume)
        {
        fEventAction->AddLO(1.);
        G4ThreeVector momPhoton = preStepPoint->GetMomentum();
        G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;
    }


//////////////////////////////////////////////////////////////////////////////
// Killed tracks Information
//////////////////////////////////////////////////////////////////////////////
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
}
