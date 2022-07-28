#include "detector.hh"
#include "util.hh"


MySensitiveDetector::MySensitiveDetector(G4String name, MyG4Args* MainArgs) : G4VSensitiveDetector(name)
{
    double wavelength[1000], qe[1000];

    PassArgs = MainArgs;

    PDE = new G4PhysicsOrderedFreeVector();
    G4double Vov = PassArgs->GetVov();
    G4double Eff420 = (0.393 * 1.0228) * (1 - exp(-0.583*Vov));

    int n = read_tsv_file("eff.dat", wavelength, qe, 1, Eff420);

    if (n == -1) {
        fprintf(stderr, "error reading eff.dat! Did you remember to source the env.sh file?\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
        PDE->InsertValues(wavelength[i], qe[i]);

    countdet = 0;
}

MySensitiveDetector::~MySensitiveDetector()
{}
G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
    UNUSED(ROhist);
    G4Track *track = aStep->GetTrack();
    G4double Tlength = track->GetTrackLength();

    // As soon as the photon impacts it stops tracking (as it is taken from pre-step point the post-point information is still available and plotted**)
    track -> SetTrackStatus(fStopAndKill); 

    
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();
    UNUSED(postStepPoint);

    G4ThreeVector posPhoton = preStepPoint->GetPosition();
    G4ThreeVector momPhoton = preStepPoint->GetMomentum();


    G4double wlen = (1.239841939*eV/momPhoton.mag())*1E+03;


    const G4VTouchable *touchable = aStep->GetPreStepPoint()->GetTouchable();

    G4VPhysicalVolume *physVol=touchable->GetVolume();
    G4ThreeVector posDetector = physVol ->GetTranslation(); // Translated position of the volume

    G4double timeG=preStepPoint->GetGlobalTime();// time restarted every time an event starts
    G4double timeL=preStepPoint->GetLocalTime(); // starts counting when the particle is created. Differece in case of decay.
    //UNUSED(timeL);
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();

    // Fill columns of output file
    G4AnalysisManager *man = G4AnalysisManager::Instance();

if (PassArgs->GetTree_Hits() == 1){
    man->FillNtupleIColumn(0, 0,  evt);
    man->FillNtupleDColumn(0, 1,  posDetector[0]/mm);// D==double
    man->FillNtupleDColumn(0, 2,  posDetector[1]/mm);
    man->FillNtupleDColumn(0, 3,  posDetector[2]/mm);
    man->FillNtupleDColumn(0, 4,  wlen);
    man->FillNtupleDColumn(0, 5,  Tlength/mm);
    man->AddNtupleRow(0);
}

    PassArgs->AddPhHit();
    G4double PDElim =PDE->Value(wlen);
if (PassArgs->GetTree_Detected() == 1){
    if (G4UniformRand() < PDElim){
        man->FillNtupleIColumn(1, 0,  evt);
        man->FillNtupleDColumn(1, 1,  posPhoton[0]/mm);// D==double
        man->FillNtupleDColumn(1, 2,  posPhoton[1]/mm);
        man->FillNtupleDColumn(1, 3,  posPhoton[2]/mm);
        man->FillNtupleDColumn(1, 4,  timeG/ps);
        man->FillNtupleDColumn(1, 5,  timeL/ps);
        man->FillNtupleDColumn(1, 6,  PDElim);
        man->FillNtupleDColumn(1, 7,  wlen);
        man->FillNtupleDColumn(1, 8,  Tlength/mm);
        man->AddNtupleRow(1);
        countdet=countdet+1;
        //G4cout<< "Photon "<< PassArgs->GetLO() <<" GTiming : " << timeG/ps << G4endl;
        if (PassArgs->GetGeomConfig()==3 && posPhoton[0]/mm>-3.1 && posPhoton[0]/mm<-0.01){
            PassArgs->AddLO();
            if(posPhoton[2]/mm>0){
                PassArgs->AddPhotR();
            }else{
                PassArgs->AddPhotL();
            }
            if(PassArgs->GetTimeTrue()==1){PassArgs->AddPhotTiming(posPhoton[2]/mm , timeG/ps);}
        } else if (PassArgs->GetGeomConfig()==3){PassArgs->AddCT();
        } else if (PassArgs->GetGeomConfig()==1){
            PassArgs->AddLO();
            if(posPhoton[2]/mm>0){
                PassArgs->AddPhotR();
            }else{
                PassArgs->AddPhotL();
            }
            if(PassArgs->GetTimeTrue()==1){PassArgs->AddPhotTiming(posPhoton[2]/mm , timeG/ps);}
        }
    }
}

    return true;
}
