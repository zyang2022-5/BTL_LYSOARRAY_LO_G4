########################################
### LYSO bar/tile Simulation for BTL ###
########################################

Author: Guillermo Reales
Date: 08-Jun-2022

Summary:

This code generates different geometries of LYSO scintillators to estimate their Light Output (LO) with respect to the SiPM detectors.

The code is expected to work with the Singularity container that can be downloaded through the command: singularity pull library://grealesguti/test/test
If this provides an error try the UNIQE ID as: singularity pull library:sha256.4fa8d22a3afd9f8ba6f20494e629965887344f672f5a3f726ad9b7160396ae4b
The G4 version used is G4 v11.0.0

An example of how to run within a cluster with HTcondor can be found in the folder "HTexample".

How to compile in your machine
* Follow the steps in the Yvideo -> https://www.youtube.com/watch?v=Lxb4WZyKeCE&list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL
	- Use G4 version 11.0.0

How to run in your machine:
* Download the Singularity Container stated above
* Modify SingAction.sh for your Container folder
* Create buildSing and buildSing/Results folder within the folder that contains this repository
* cd into the folder that contain this repository
* modify the run.mac file (These are the commands that will be executed by G4)
* run the command: sh SingAction.sh

How to run in TierII:
* download the repository in a new folder in your node
* create a Results folder
* run Geant4SourceCVMFS.sh
* run the Makefile
* modify the run.mac file (These are the commands that will be executed by G4)
* modify the G4SimScriptCVMFS.sh if required
* run G4SimScriptCVMFS.sh

Arguments:
-m Macroname.mac    : run one time a macro file
-mn i Macroname.mac : repeat a macro file i times
-o OutputName       : Desired Output name for the root files without file extension type

Outputs: Root files per event number containing:
    * Information on the photons that reach the detector
    * Information on the photons that are detected
    * General information on the LO per event
    * Information on all photons killed
        - Position where they where killed


Schematics of the code:
    * sim.cc (main)
        ->  construction.*
            ->  detector.*
        ->  physics.*
        ->  action.*
            -> generator.*
            -> run.*
            -> event.*
            -> stepping.*
            -> tracking.*



