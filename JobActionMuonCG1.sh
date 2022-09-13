#!/usr/bin/env bash
#Default values for optional arguments
BASEDIR=$(pwd)          # Current folder where the G4 files can be found#
#BASEDIR="/storage/af/user/greales/simG4/Repov2/"

SimName="sim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut="MuonSingleSiPM3mmResin_0922_1045"   # Name of the Root output files from the G4 executable 

### Main action
ActionSH=$BASEDIR/SimScriptMuonCG1.sh
. $ActionSH -a $SimArgs -n $SimName -m $MacroName -b $BASEDIR -v 0 -o $OutPut
