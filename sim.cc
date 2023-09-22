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

//#include "G4RunManager.hh" /* Run */
#include "src_G4/G4simTierII.hh"
//#include "src_G4/G4sim.hh"
#include "src_G4/util.hh"
#include <iostream>
#include <string>
#include <stdarg.h>


int main(int argc, char** argv) /* argc, argv are the argument passed to the sim*/
{   

    G4int Onode=5,Znode=2;
    G4double* radv;
    /*    radv = new G4double[Onode*(Znode+1)];   
        G4double Pi=atan(1)*4;
        G4double DTheta=Pi/(Onode-1);
        // radius vector initialization
            for(int i = 0; i < Znode+1; i++){
                for (int j = 1; j < Onode+1; j++){
                    if(j==1 || j==3 || j==5){radv[i*Onode-1+j]=3./2.;}
                    else{radv[i*Onode-1+j]=pow(2*pow(3./2.,2),0.5);}
                }
            }
    G4double Vol = LYSOMeshVolume(radv, Onode,  Znode); // Solution in [mm³]
    // Volume Calculation 57*3*3=513
    */
    // Standard usage    
    G4double* radinit;
    radinit = new G4double[2];   
    radinit[0]=1;
    radinit[1]=0.5;
    //G4double radinit[2]={1,2};
    G4simulationNOVIS *sim = new G4simulationNOVIS(argc, argv);


    //G4simulationNOVIS *sim1 = new G4simulationNOVIS(runManager,argc, argv, Onode, Znode, radinit);
   // G4simulation *sim = new G4simulation(argc, argv);
    // Example on how to pass always the same arguments
/*
    G4int argc1=3;
    char *args[] = {
        (char*)"sim",
        (char*)"-m",
        (char*)"run.mac"
    };    

    G4simulationNOVIS *sim = new G4simulationNOVIS(argc1, args, Onode, Znode, radv);
*/
    G4cout<< " ### Fom sim.cc, the LO_avg is "<< sim->GetLO_avg(0) <<G4endl;     
    return 0;
}
