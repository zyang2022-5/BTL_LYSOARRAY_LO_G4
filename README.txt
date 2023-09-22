# README

## BTL LYSO Scintillation Simulation

This code creates simulations of the impact of Na gamma rays and Muons onto a scintillator.

The output provides information of the Light Output and timing resolution of the attached SiPMs.

Furthermore the code is expected to work with a NSGAII genetic algorithm to study the shape optimization of the crystals.

## Installation and running

Download the latest code using:

`git clone --single-branch --branch g/f/G4asfunction/src/TierII-gmsh https://github.com/grealesguti/BTL_LYSOARRAY_LO_G4.git`

The code is expected to work in conjonction with Gmsh for the shape of the crystals.

The singularity file () provides the required libraries to run the code.

An example installed and run using:

`
singularity exec <path-to-the-singularity-file> make -f Makefile.localgmsh
singularity exec <path-to-the-singularity-file> ./sim -GeomConfig 11 -runevt 10
`

or with a more complex example and a given path:

`singularity exec ../../SingDir/sandG4Gmsh ./sim -GeomConfig 11 -runevt 10 -Volume -nDetected -incrSiPM 125 -LYSO_L 20 -ResinMach -rnd 0 -Znode 1 -Ypos {1-1} -Acte -BC400`

### How to run under a singularity file (Singularity has changed its name!! Apptainer, same fucntionality)

Download the singularity file from:
`singularity build <singularity_file_name>.sif docker-daemon://<docker_registry>/<repository>:<tag>`


The singularity file needs to be in your node as a sandbox. Once downloaded the file remember to run:
`singularity build --sandbox <sandboxname> <path/singularity_file_name.sif>`

Now introduce the path to the singularity file within your submission file
`+Singularity=<path>`

To run it locally remember to install singularity:
https://www.linuxwave.info/2022/02/installing-singularity-in-ubuntu-2004.html

###Installing G4Sipm
To enable to the realistic Sipm simulation using the G4Sipm package. Install it through source:
https://github.com/ntim/g4sipm

Since the simulation is running on Geant4 v11. Implement the changes stated in this merge request for the G4Sipm package before building to incorporate Geant4 V11 compatibility:
https://github.com/ntim/g4sipm/pull/5/commits/53d953ee2427309cfa4d6b94d60f09a28ba1fa20

Update the Dynamic Linker Path to include the directory containing libg4sipm.so (alternatively you can use LD_LIBRARY_PATH)

Run the simulation normally with the added command `-G4Sipm`.

### Running in TierII

Login into TierII: `username@login-1.hep.caltech.edu`

Set your respective proxy: `voms-proxy-init -voms cms -rfc -valid 192:0 --bits 2048`

Run using one of the example submission and job files in their respective folders.

### Running numerical optimization

We need to have root in our python environment
```
export LCGENV_PATH=/cvmfs/sft.cern.ch/lcg/releases/
eval "` $LCGENV_PATH/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt ROOT `"
```
Navigate to the folder NSGAII/PyNSGA/NSGAII and run:

`python example/G4example.py`

This file contains the python scripts that executes the NSGAII algorithm.

It is useful to run under tmux to avoid the code from stopping if you log out. An example to how to open a new tmux window:
`tmux new -s sessionname`

The root file that contain the resulting individuals can be found in NSGAII/PyNSGA/NSGAII/ROOT

Standard use is with 100 individuals. Notice that the algorithm can break if we use Short jobs that can not run for longer than 2h. In these cases try to reduce the duration of the simulation with simpler geometries.
- TODO : penalize with large objectives if the files are not found or can not be opened. remove all submited jobs before continuing.

## Optional arguments

The commands work as follow `-command argument`, in `[]` we set the number of options, if an option is given as zero, the opposite takes 1.


-GeomConfig [1 2 3 11 13] : This argument changes the configuration of the detector. Configuration 1-3 provide use the default geometry construction of G4 while 11 and 13 use Gmsh and allow shape changes through splines and the -YPos argument. Config. 3 and 13 use 16 crystals while the rest use a single one.
-Zelem [8] : number of mesh nodes along half of Z of the LYSO crystal
-Znode [1] : number of sections of the LYSO mesh
-nX [3] : number of X nodes in 1 quadrant
-nodesec [4] : number of Z nodes in 1 quadrant
-Ypos {-1-1} : value that multiplies the 3mm by default of each spline control node of the mesh.
-incrV [1-199] : value between 1 and 199 that provides the 1D study of 2 half tetrahedra for the crystal shape decreasing and increasing the LYSO thickness in the middle and edges of the crystal

-o name :  name identifies the name.root file created in the Results folder with the information of the simulation.
-runevt [10] : argument that provide how many events will be run. If not given and if you use G4vis.cc you can use the graphical interface.

-rnd [1 0 0] : taking 3 arguments. The second argument sets no random position to the particle impacts, the second sets no random geometry tolerances during the detector construction.
-rndangle : the particle impacts in random directions and not only along the Z direction
-gunmesh [4 15] : provides a uniform impact pattern in a quarter of the crystal along X and Z. With the given defaults it would run 60 events.
-Muon : sets the particle impact to 2MeV Muons.

-ESRbackpainted : changes the surface coating model so that there is an air gap between the ESR and the LYSO
-noESR : no ESR coating is applied to the crystal

-LYSO_L [28.5] : where the number given is half the lenght of the LYSO crystal along Z
-Acte : command that maintains the XxZ area constant increasint X if LYSO_L is changed
-matchSiPM : command that sets the SiPM width X equal to the LYSO crystal width
-incrSiPM [25-200]: increases in percentage the height of the SiPM from its default 3mm 
-SiPM_Z
-RESIN_Z
-RESIN_Z1000
-Glue_Z
-Glue_Z1000
-RESIN_W

-GmshView: opens Gmsh before running G4. Right now does not allow to continue the simulation.
-SaveSTL: saves an .stl file of the mesh for further postprocessing

-BC408: changes the crystal material from LYSO to BC408 (polymer).

-TileV0 : changes configuration from bar to tile. (TODO: right now the design is assymmetrical, the triangulation needs to be changed in Gmsh)
-ForceBottomLine: To always be used with -TileV0. Forces a given flat bottom surface.
-SZloc [0-1] : changes the SiPM location along Z at the bottom of the tile as a percentage of LYSO_L.

-G4Sipm : changes the G4VSensitiveDetectors to the ones implemented through G4Sipm package. This realistic simulation of the Sipm allows for digitized results and voltage readouts similar to the output of a real Sipm. The Sipm Can be custom defined to fit the simulation target.

## How to start the graphical interface

In the sim.cc file change 

//#include "src_G4/G4simTierII.hh"
#include "src_G4/G4sim.hh"

and 

//G4simulationNOVIS *sim1 = new G4simulationNOVIS(runManager,argc, argv, Onode, Znode, radinit);
G4simulation *sim = new G4simulation(argc, argv);

now remake and if no -runevt is given you should be runnign the gui.

THIS CAN NOT WORK IN TIERII!!!

