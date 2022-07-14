#include "run.hh"

MyRunAction :: MyRunAction(G4String OutName)
{ // Constructor

 /*   file_name = "default";
    fMessenger = new G4GenericMessenger(this, "/analysis/", "name stuff");
    fMessenger->DeclareProperty("filename", file_name, "name of the output root file");

    // Saving output name from args
    OutputName=OutName;
*/
    file_name = OutName;	

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Content of output.root (tuples created only once in the constructor)

        // Tuple containing all data from the Photons arriving to the detector
            man->CreateNtuple("Photons","Photons");   // rows
            man->CreateNtupleIColumn("fEvent"); // columns ,I == Integer
            man->CreateNtupleDColumn("fX");
            man->CreateNtupleDColumn("fY");
            man->CreateNtupleDColumn("fZ");
            man->CreateNtupleDColumn("fWlen");
            man->CreateNtupleDColumn("fMeanPath");
            man->FinishNtuple(0); // Finish our first tuple or Ntuple number 0

        // Tuple containing all data from the Photons being detected
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

         // Tuple containing all data fregarding the estimations written to screen at the end of each event
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
	    man->CreateNtupleDColumn("iX");
	    man->CreateNtupleDColumn("iZ");
            man->FinishNtuple(2); // Finish our first tuple or Ntuple number 0

        // Tuple containing all information regarding all killed photons
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

    // Initialization of G4 random generator through computer time
    G4int timeseed1=time(NULL);
    G4int timeseed2=time(NULL)+128493729;
    G4int timeseed3=time(NULL)/23839;
    G4String command ="/random/setSeeds "+std::to_string(timeseed1)+" "+std::to_string(timeseed2)+" "+std::to_string(timeseed3);
    UImanager->ApplyCommand(command); 
    G4cout<<command<< G4endl;
    G4double rand=G4UniformRand();  // Test of random number written to screen
    G4cout<<" Random number: " << rand << G4endl;


    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Get current Event number 
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

 //   file_name = "default";
    fMessenger = new G4GenericMessenger(this, "/analysis/", "name stuff");
    fMessenger->DeclareProperty("filename", file_name, "name of the output root file");

    OutputName=file_name;

    // Creation of Output file
    man->OpenFile("./Results/"+OutputName+strRunID.str()+".root");

}
void MyRunAction::EndOfRunAction(const G4Run*)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    // Modify random parameter in the geometry 
    //  #### This does nothing unless we do another run, the geometry can only be changed in between runs not events !!!
    G4double GLUE_L = 0.05+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
    G4double RESIN_L =0.3 +0.4*G4UniformRand();   RESIN_L=RESIN_L/2.;
    G4double XposTol = -0.15+G4UniformRand()*0.3;
    G4double YposTol = -0.15+G4UniformRand()*0.3;

    command="/detector/GLUE_L " + std::to_string(GLUE_L);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/RESIN_L " + std::to_string(RESIN_L);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/XposTol " + std::to_string(XposTol);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/detector/YposTol " + std::to_string(YposTol);
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;

    // Close the output file for this event
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();// Write out the root file to avoid damaging it
    man->CloseFile();

}
