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

#include "G4sim.hh"


int main(int argc, char** argv) /* argc, argv are the argument passed to the sim*/
{   
    G4simulation *sim = new G4simulation(argc, argv);
    return 0;
}
