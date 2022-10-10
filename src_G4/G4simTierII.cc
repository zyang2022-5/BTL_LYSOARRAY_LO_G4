#include "G4simTierII.hh"

G4simulationNOVIS::G4simulationNOVIS(int mainargc,char** mainargv, G4int Onode , G4int Znode , G4double* radp)
{
    G4RunManager *runManager = new G4RunManager();

    G4cout<< mainargc << "\n" << mainargv[0]<< "\n" << mainargv[1] << G4endl;
  // Set Up / Run  Initialization

    ArgInp = new MyG4Args(mainargc, mainargv);
    
    if(radp == NULL){
        if(ArgInp->Getrad2Y() == 1 && ArgInp->GetIncr() == 0){
            G4cout<< " !!! WARNING !!! rad2Y set to 1 but no arguments were passed, set to 1. " <<G4endl;  
            G4double radinit[2]={1,2};
            //radinit[0]=1;radinit[1]=1;
            G4double* radones=radinit;
            ArgInp->DefaultRadiusVect();
            ArgInp->SetCoordVect();
            ArgInp->SetYVect(radones);      
        }else if(ArgInp->GetIncr() > 0){
            G4double radinit[2]={1,ArgInp->GetIncr()};
            //radinit[0]=1;radinit[1]=1;
            G4double* radones=radinit;
            ArgInp->DefaultRadiusVect();
            ArgInp->SetCoordVect();
            ArgInp->SetYVect(radones); 
        }else{
            ArgInp->DefaultRadiusVect();
        }
    }else{
        if(ArgInp->Getrad2Y() == 1){
            ArgInp->DefaultRadiusVect();
            ArgInp->SetCoordVect();
            ArgInp->SetYVect(radp);
        }else{
            ArgInp->SetRadiusVect(radp,Onode,Znode);
        }
    }

    runManager -> SetUserInitialization(new MyDetectorConstruction(ArgInp)); /*Define geometry*/
    runManager -> SetUserInitialization(new MyPhysicsList()); /*Define physics*/
    runManager -> SetUserInitialization(new MyActionInitialization(ArgInp)); /*Define actions*/

    runManager -> Initialize();

///////* Vis -> Visualizer*//////
//    G4UIExecutive *ui = 0; // ui initialization
    // Show *user interface only if we do not use a macro, or it will be inefficient    
//    if (ArgInp->GetVis() ==1) // argc only one option introduced
//    {
//        ui = new G4UIExecutive(mainargc,mainargv); // ui definition-declaration
//    }   
    /* Vis -> Visualizer*/  // Initialization
//    G4VisManager *visManager = new G4VisExecutive();
//    visManager->Initialize();
///////* Vis -> Visualizer*//////

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if(ArgInp->GetVis() ==1)
    {
//        UImanager->ApplyCommand("/control/execute vis.mac");
//        ui->SessionStart();     ///////* Vis -> Visualizer*//////
    }
    else
    {
                    G4String command = "/control/execute ";  
                    G4String fileName = ArgInp->GetMacName();
                    G4int nrep = ArgInp->Getnrep();
                    for (int i = 1; i <= nrep; i=i+1){
                        UImanager->ApplyCommand(command+fileName);
                    }
        }

    /*G4int runid=0;
    G4cout<< " ### Fom G4Args.cc, the LD_avg is "<< ArgInp->GetLOAvg(runid) <<G4endl;     
    LDavg=ArgInp->GetLOAvg(runid);

  */

  delete runManager;

}
G4simulationNOVIS::~G4simulationNOVIS()
{}

