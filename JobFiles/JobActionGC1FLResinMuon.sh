#!/usr/bin/env bash
#Default values for optional arguments
#BASEDIR=$(pwd)          # Current folder where the G4 files can be found
BASEDIR="/storage/af/user/greales/simG4/Repov2/"

SimName="sim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut="GC1_FLResin_Muon_"   # Name of the Root output files from the G4 executable 

while getopts "a:n:s:" arg; do
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
    esac
done

echo "*** Sourcing G4:"
. $BASEDIR/G4SourceCVMFS.sh

echo "Running Simulation" 
cd $BASEDIR
./$SimName -o $OutPut -GeomConfig 1 -m run.mac -Muon
