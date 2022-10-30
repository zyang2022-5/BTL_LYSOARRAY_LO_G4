#!/usr/bin/env bash
#Default values for optional arguments
#BASEDIR=$(pwd)          # Current folder where the G4 files can be found
BASEDIR="/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4/"

SimName="sim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut="GC1_FLResinMach_Muon_Incr_"   # Name of the Root output files from the G4 executable 
Vars="{1.0-1.0}"
Nsections="2"

while getopts "a:n:s:v:z:" arg; do
    case $arg in
        a) OutPut=$OutPut$OPTARG
            echo "*** Change of OutputName"
            echo $simargsprev;; # Change G4 source directory
        n) SimName=$OPTARG
            echo "*** Change of Simulation Name"
            echo $SimName;; # Change G4 source directory
        s) Sopt=$OPTARG
            echo "*** Change of Source directory"
            echo $Sopt;; # Change G4 source directory
        v) Vars=$OPTARG
            echo "*** Change of Vars"
            echo $Vars;; # Change G4 source directory
        z) Nsections=$OPTARG
            echo "*** Change of Nsections"
            echo $Nsections;; # Change G4 source directory
    esac
done

echo "*** Sourcing G4:"
. $BASEDIR/G4SourceCVMFS.sh

echo "Running Simulation" 
OutPut=$OutPut$Sopt
echo $Sopt
cd $BASEDIR
./$SimName -o $OutPut -GeomConfig 1 -runevt 1 -Muon -nDetected -Znode $Nsections -Ypos $Vars -Volume



