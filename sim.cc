/////////////////////////////////// INFORMATION ///////////////////////////////////
/* Main routing for BTL LYSO G4 Simulation
Authors: Guillermo Reales Gutiérrez, ...

Summary: 

Arguments: -> See G4Args.cc Class file

Outputs: 1 .ROOT file per run. Includes 5 trees
    - Arrivals: Information on all photons which impact the photodetector (see detector.cc)
    - Detected: Information on all photons detected at the photodetector (see detector.cc)
    - Stepping: Information extracted at all steps for all particles (see stepping.cc)
    - Tracking: information extracted at the beginning or end of the tracks of  the particles (see tracking.cc)
    - EndOfEvet: information extracted at the end of each event (see event.cc)
    The existence of the tree does not impose that information was stored in it depending on the arguments set for the simulation.

VISUALIZATION: Everythin in between * Vis -> Visualizer* lines of with it at the end of the line is related to the visualization of the geometry. To run in the cluster, these lines must be commented.

*/

/////////////////////////////////// PROGRAM START ///////////////////////////////////

#include <iostream>
#include "G4RunManager.hh" /* Run */
#include "G4UImanager.hh"

///////* Vis -> Visualizer*//////
#include "G4VisManager.hh"      /* Vis -> Visualizer*/
#include "G4VisExecutive.hh"    /* Vis -> Visualizer*/
#include "G4UIExecutive.hh"     /* Vis -> Visualizer*/
///////* Vis -> Visualizer*//////

/*Simulation created files*/
#include "construction.hh"
#include "physics.hh"
#include "action.hh"
#include <cstdlib>
#include "G4Args.hh"


int main(int argc, char** argv) /* argc, argv are the argument passed to the sim*/
{   
    // Set Up / Run  Initialization
    G4RunManager *runManager = new G4RunManager();
    MyG4Args *ArgInp = new MyG4Args(argc, argv);

    runManager -> SetUserInitialization(new MyDetectorConstruction(ArgInp)); /*Define geometry*/
    runManager -> SetUserInitialization(new MyPhysicsList()); /*Define physics*/
    runManager -> SetUserInitialization(new MyActionInitialization(ArgInp)); /*Define actions*/


    runManager -> Initialize();

///////* Vis -> Visualizer*//////
    G4UIExecutive *ui = 0; // ui initialization
    // Show *user interface only if we do not use a macro, or it will be inefficient    
    if (ArgInp->GetVis() ==1) // argc only one option introduced
    {
        ui = new G4UIExecutive(argc,argv); // ui definition-declaration
    }   
    /* Vis -> Visualizer*/  // Initialization
    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();
///////* Vis -> Visualizer*//////

    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    if(ArgInp->GetVis() ==1)
    {
        UImanager->ApplyCommand("/control/execute vis.mac");
        ui->SessionStart();     ///////* Vis -> Visualizer*//////
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



    return 0;
}
