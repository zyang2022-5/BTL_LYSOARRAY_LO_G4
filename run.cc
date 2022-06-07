#include "run.hh"

MyRunAction :: MyRunAction()
{ // Constructor
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    // Content of output.root (tuples created only once in the constructor)
    man->CreateNtuple("Photons","Photons");   // rows
    man->CreateNtupleIColumn("fEvent"); // columns ,I == Integer
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("fWlen");
    man->CreateNtupleDColumn("fMeanPath");
    man->FinishNtuple(0); // Finish our first tuple or Ntuple number 0

    man->CreateNtuple("Hits","Hits");   // rows
    man->CreateNtupleIColumn("fEvent"); // columns ,I == Integer
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->CreateNtupleDColumn("ftime");
    man->CreateNtupleDColumn("fPDE");
    man->CreateNtupleDColumn("fwlen");
    man->CreateNtupleDColumn("fMeanPath");
    man->FinishNtuple(1); // Finish our first tuple or Ntuple number 0

    man->CreateNtuple("Scoring","Scoring");   // rows
    man->CreateNtupleDColumn("fEdep"); 
    man->CreateNtupleDColumn("fPhot"); 
    man->CreateNtupleDColumn("fHits"); 
    man->CreateNtupleDColumn("fPDE");
    man->CreateNtupleDColumn("fLOapprox");
    man->CreateNtupleDColumn("fPrimaryX");
    man->CreateNtupleDColumn("fPrimaryZ");
    man->CreateNtupleDColumn("fGlueL");
    man->CreateNtupleDColumn("fResinL");
    man->CreateNtupleDColumn("fDetXpos");
    man->CreateNtupleDColumn("fDetYpos");
    man->CreateNtupleDColumn("fevt");
    man->FinishNtuple(2); // Finish our first tuple or Ntuple number 0

    man->CreateNtuple("ScoringKilled","ScoringKilled");   // rows
    man->CreateNtupleDColumn("fKilledLength"); 
    man->CreateNtupleDColumn("ftime");
    man->CreateNtupleDColumn("fX");
    man->CreateNtupleDColumn("fY");
    man->CreateNtupleDColumn("fZ");
    man->FinishNtuple(3); // Finish our first tuple or Ntuple number 0

}
MyRunAction :: ~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    G4int timeseed1=time(NULL);
    G4int timeseed2=time(NULL)+128493729;
    G4int timeseed3=time(NULL)/23839;
    G4String command ="/random/setSeeds "+std::to_string(timeseed1)+" "+std::to_string(timeseed2)+" "+std::to_string(timeseed3);
    UImanager->ApplyCommand(command); 
    G4cout<<command<< G4endl;
    G4double rand=G4UniformRand();
    G4cout<<" Random number: " << rand << G4endl;
    G4AnalysisManager *man = G4AnalysisManager::Instance();

    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;


    man->OpenFile("output"+strRunID.str()+".root");

//UImanager->ApplyCommand("/run/reinitializeGeometry");


}
void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    //G4UImanager *UImanager = G4UImanager::GetUIpointer();
    G4double GLUE_L = 0.05+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
    G4double RESIN_L =0.3 +0.4*G4UniformRand();   RESIN_L=RESIN_L/2.;
    G4double XposTol = -0.15+G4UniformRand()*0.3;
    G4double YposTol = -0.15+G4UniformRand()*0.3;
    //G4double LYSO_L = 20+G4UniformRand()*20;
    //command="/detector/LYSO_L " + std::to_string(LYSO_L);
    //UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/GLUE_L " + std::to_string(GLUE_L);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/RESIN_L " + std::to_string(RESIN_L);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/XposTol " + std::to_string(XposTol);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/YposTol " + std::to_string(YposTol);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    //UImanager->ApplyCommand("/run/reinitializeGeometry");  //G4cout<< command << G4endl;
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();// Write out the root file to avoid damaging it
    man->CloseFile();

}
