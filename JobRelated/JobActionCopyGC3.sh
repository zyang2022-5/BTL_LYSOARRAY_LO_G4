#!/usr/bin/env bash
#Default values for optional arguments
#BASEDIR=$(pwd)          # Current folder where the G4 files can be found
BASEDIR="/storage/af/user/greales/simG4/Repov2/"

buildfol="buildSing"    # bulding folder nae
SimName="sim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut="JobTestSecond"   # Name of the Root output files from the G4 executable 

### Main action
ActionSH=$BASEDIR/G4SimScriptCVMFSCopy.sh
. $ActionSH -a $SimArgs -n $SimName -m $MacroName -b $BASEDIR -f $buildfol -v 0 -o $OutPut
