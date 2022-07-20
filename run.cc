#include "run.hh"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

MyRunAction :: MyRunAction(G4String OutName,MyG4Args* MainArgs)
{ // Constructor
    
    PassArgs=MainArgs;
    // Saving output name from args
    OutputName=OutName;

    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Content of output.root (tuples created only once in the constructor)

        // Tuple containing all data from the Photons arriving to the detector
            man->CreateNtuple("Arrivals","Arrivals");   // Photons
            man->CreateNtupleIColumn("fEvent"); // columns ,I == Integer
            man->CreateNtupleDColumn("fX");
            man->CreateNtupleDColumn("fY");
            man->CreateNtupleDColumn("fZ");
            man->CreateNtupleDColumn("fWlen");
            man->CreateNtupleDColumn("fMeanPath");
            man->FinishNtuple(0); // Finish our first tuple or Ntuple number 0

        // Tuple containing all data from the Photons being detected
            man->CreateNtuple("Detected","Detected");   // Hits
            man->CreateNtupleIColumn("fEvent"); // columns ,I == Integer
            man->CreateNtupleDColumn("fX");
            man->CreateNtupleDColumn("fY");
            man->CreateNtupleDColumn("fZ");
            man->CreateNtupleDColumn("ftimeG");
            man->CreateNtupleDColumn("ftimeL");
            man->CreateNtupleDColumn("fPDE");
            man->CreateNtupleDColumn("fwlen");
            man->CreateNtupleDColumn("fMeanPath");
            man->FinishNtuple(1); // Finish our first tuple or Ntuple number 0

        // Tuple containing all information regarding all killed photons
            man->CreateNtuple("Stepping","Stepping");   // rows
            man->CreateNtupleDColumn("fKilledLength"); 
            man->CreateNtupleDColumn("ftime");
            man->CreateNtupleDColumn("fX");
            man->CreateNtupleDColumn("fY");
            man->CreateNtupleDColumn("fZ");
            man->FinishNtuple(2); // Finish our first tuple or Ntuple number 0

        // Tuple containing all information regarding all killed photons
            man->CreateNtuple("Tracking","Tracking");   // rows
            man->CreateNtupleDColumn("test");
            man->FinishNtuple(3); // Finish our first tuple or Ntuple number 0

         // Tuple containing all data fregarding the estimations written to screen at the end of each event
            man->CreateNtuple("EndOfEvent","EndOfEvent");   // Scoring
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
            man->CreateNtupleDColumn("fDetXpos2");
            man->CreateNtupleDColumn("fDetYpos2");
            man->CreateNtupleDColumn("fCrossTalkCount");
            man->CreateNtupleDColumn("fCrossTalkMev");
            man->CreateNtupleDColumn("fTiming");
            man->CreateNtupleDColumn("fevt");
            man->FinishNtuple(4); // Finish our first tuple or Ntuple number 0

         // Tuple containing all data fregarding the estimations written to screen at the end of each event
            man->CreateNtuple("EndOfRun","EndOfRun");   // Scoring
            man->CreateNtupleDColumn("fLOAvg"); 
            man->CreateNtupleDColumn("fLOStd"); 
            man->CreateNtupleDColumn("fTimAvg"); 
            man->CreateNtupleDColumn("fTimStd");
            man->CreateNtupleDColumn("frun");
            man->FinishNtuple(5); // Finish our first tuple or Ntuple number 0

}
MyRunAction :: ~MyRunAction()
{}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    command="/vis/initialize ";
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/vis/drawVolume";
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    command="/vis/scene/add/trajectories smooth";
    UImanager->ApplyCommand(command);  G4cout<< command << G4endl;


    // Initialization of G4 random generator through computer time
    G4int timeseed1=time(NULL);
    G4int timeseed2=time(NULL)+128493729;
    G4int timeseed3=time(NULL)/23839;
    command ="/random/setSeeds "+std::to_string(timeseed1)+" "+std::to_string(timeseed2)+" "+std::to_string(timeseed3);
    UImanager->ApplyCommand(command); 
    G4cout<<command<< G4endl;
    G4double rand=G4UniformRand();  // Test of random number written to screen
    G4cout<<" Random number: " << rand << G4endl;


    G4AnalysisManager *man = G4AnalysisManager::Instance();

    // Get current Event number 
    G4int runID = run->GetRunID();
    std::stringstream strRunID;
    strRunID << runID;

    struct stat st;

    if (stat("Results", &st) == -1) {
        printf("Creating directory \"Results\" since it doesn't exist.\n");
        mkdir("Results", 0700);
    }

    // Creation of Output file
    man->OpenFile("./Results/"+OutputName+strRunID.str()+".root");

}
void MyRunAction::EndOfRunAction(const G4Run* run)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    // Fill G4Args Run values
    G4int runid=run-> GetRunID();
    G4cout<<"#################### " <<G4endl;
    G4cout<<"### END OF RUN: " << run-> GetRunID() << " ### " <<G4endl;
    if(PassArgs->Getnrep()>0){
        for (G4int i = 0; i < PassArgs->GetnEvents(); i=i+1){
                PassArgs-> FillAvgTim(runid);
                PassArgs-> FillAvgLO(runid); 
                PassArgs-> FillStdTim(runid);
                PassArgs-> FillStdLO(runid);
        }
        if(PassArgs->GetTree_EndOfRun()==1){
        // Storage single values per run
            for (G4int j = 0; j < runid; j=j+1){
                G4AnalysisManager *man = G4AnalysisManager::Instance();
                man->FillNtupleDColumn(5, 0, PassArgs->GetLOAvg(j));
                man->FillNtupleDColumn(5, 1, PassArgs->GetLOStd(j));
                man->FillNtupleDColumn(5, 2, PassArgs->GetTimAvg(j));
                man->FillNtupleDColumn(5, 3, PassArgs->GetTimStd(j));
                man->FillNtupleDColumn(5, 4, PassArgs->GetnEvtEdep(j));
                // geometry values
                man->FillNtupleDColumn(5, 5, j);
                man->AddNtupleRow(5);
            }
        }
    G4cout<<"* Average LO: " << PassArgs->GetLOAvg(runid) <<G4endl;
    G4cout<<"* Std LO: " << PassArgs->GetLOStd(runid) <<G4endl;
    G4cout<<"* Average Timing: " << PassArgs->GetTimAvg(runid) <<G4endl;
    G4cout<<"* Std Timing: " << PassArgs->GetTimStd(runid) <<G4endl;
    G4cout<<"* Number of events with energy deposition: " << PassArgs->GetnEvtEdep(runid) <<G4endl;
    }
    G4cout<<"### END OF RUN: " << run-> GetRunID() << " ### " <<G4endl;
    G4cout<<"#################### " <<G4endl;
    // Modify random parameter in the geometry 
    //  #### This does nothing unless we do another run, the geometry can only be changed in between runs not events !!!
    if(PassArgs->GetRnd_Geom()==1)
        {
        G4double GLUE_L = 0.15+0.1*G4UniformRand();   GLUE_L=GLUE_L/2.;
        G4double RESIN_L =0.5+0.2*G4UniformRand();   RESIN_L=RESIN_L/2.;
        G4double XposTol = -0.08+G4UniformRand()*0.16;
        G4double YposTol = -0.08+G4UniformRand()*0.16;
        G4double XposTol2 = -0.08+G4UniformRand()*0.16;
        G4double YposTol2 = -0.08+G4UniformRand()*0.16;
        command="/detector/GLUE_L " + std::to_string(GLUE_L);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        command="/detector/RESIN_L " + std::to_string(RESIN_L);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        command="/detector/XposTol " + std::to_string(XposTol);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        command="/detector/YposTol " + std::to_string(YposTol);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        command="/detector/XposTol2 " + std::to_string(XposTol2);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        command="/detector/YposTol2 " + std::to_string(YposTol2);
        UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
        PassArgs->GeomReinit();
        }

    
    // Close the output file for this event
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->Write();// Write out the root file to avoid damaging it
    man->CloseFile();

}
