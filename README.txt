########################################
### LYSO bar/tile Simulation for BTL ###
########################################

Author: Guillermo Reales
Date: 08-Jun-2022

Summary:

This code generates different geometries of LYSO scintillators to estimate their Light Output (LO) with respect to the SiPM detectors.

The code is expected to work with the Singularity container that can be downloaded through the command: singularity pull library://grealesguti/test/test

An example of how to run within a cluster with HTcondor can be found in the folder "HTexample".

How to run:
* Download Container stated above
* Modify SingAction.sh for your Container folder
* Create buildSing and buildSing/Results folder within the folder that contains this repository
* cd into the folder that contain this repository
* run the command: sh SingAction.sh

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

