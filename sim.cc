
/* Outer Shell for geant4 Simulation */
#include <iostream>

/*All packages used must be included*/
#include "G4RunManager.hh" /* Run */
#include "G4UImanager.hh"
//#include "G4VisManager.hh" /* Vis -> Visualizer*/
//#include "G4VisExecutive.hh"
//#include "G4UIExecutive.hh"

/*Simulation created files*/
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include <cstdlib>


int main(int argc, char** argv) /* argc, argv are the argument passed to the sim*/
{   
    // Set Up / Run  Initialization
    G4RunManager *runManager = new G4RunManager();
    
    runManager -> SetUserInitialization(new MyDetectorConstruction()); /*Define geometry*/
    runManager -> SetUserInitialization(new MyPhysicsList()); /*Define physics*/

    G4int Oin=0; 
    G4String OutName;
    for (int j = 1; j < argc; j=j+1){
                if(strcmp(argv[j],"-o")==0)
                {   
                    OutName = argv[j+1];      
                    G4cout<< " ### Changed Ouput name to : " << OutName<<G4endl;            
                    Oin=1;         
                }
        }

    if (Oin == 0 ) {  OutName = "WhateverTestName";   }

    runManager -> SetUserInitialization(new MyActionInitialization(OutName)); /*Define actions*/


    runManager -> Initialize();
    
    //G4UIExecutive *ui = 0; // ui initialization
    // Show *user interface only if we do not use a macro, or it will be inefficient    
    //if (argc ==1) // argc only one option introduced
    //{
     //   ui = new G4UIExecutive(argc,argv); // ui definition-declaration
    //}   
    /* Vis -> Visualizer*/  // Initialization
    //G4VisManager *visManager = new G4VisExecutive();
    //visManager->Initialize();
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    //if(ui)
    //{
    //    UImanager->ApplyCommand("/control/execute vis.mac");
    //    ui->SessionStart();
    //}
    //else
    //{
        /*for (int j = 1; j <= argc; j=j+1){
                if(strcmp(argv[j],"-m")==1)
                {
                    G4cout<< "Macro File: " << G4endl;
                    G4String command = "/control/execute ";  
                    G4String fileName = argv[1];
                    G4int nrep =  atoi(argv[2]);
                    for (int i = 1; i <= nrep; i=i+1){
                        UImanager->ApplyCommand(command+fileName);}
                }
        }*/
        for (int j = 1; j < argc; j=j+1){
                G4cout<< to_string(j)<< " INPUT: " << argv[j]<<G4endl;
                if(strcmp(argv[j],"-mn")==0)
                {   //G4cout<< "INSIDE IF"<<G4endl;
                    G4String command = "/control/execute ";  
                    G4String fileName = argv[j+1];
                    G4int nrep =  atoi(argv[j+2]);
                    for (int i = 1; i <= nrep; i=i+1){
                        UImanager->ApplyCommand(command+fileName);
                    }
                }
                else if (strcmp(argv[j],"-m")==0)
                {   //G4cout<< "INSIDE IF"<<G4endl;
                    G4String command = "/control/execute ";  
                    G4String fileName = argv[j+1];                  
                    UImanager->ApplyCommand(command+fileName);
                }
        }
        /*G4String command = "/control/execute ";  
        G4String fileName = argv[1];
        G4int nrep =  atoi(argv[2]);
        for (int i = 1; i <= nrep; i=i+1){
            UImanager->ApplyCommand(command+fileName);
        }*/
 //   }


    return 0;
}
