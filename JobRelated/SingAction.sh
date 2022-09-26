#!/bin/bash
# Default values for optional arguments
BASEDIR=$(pwd)          # Current folder where the G4 files can be found
#BASEDIR="/storage/af/user/greales/simG4/SingLYSO/"

G4Source="/usr/Geant4/geant4v11.p0/share/Geant4-11.0.0/geant4make/"     # G4 Installation folder in the container (HOME VM)
SingCont="/home/guillermo/localdev/ubuntutest/" 			# location of the container (HOME VM)
ContName="test1"        						# Name of the singularity container (HOME VM)

#G4Source="/usr/Geant4/geant4v11.p0/share/Geant4-11.0.0/geant4make/" 	# G4 Installation folder in the container (Tier II greales)
#SingCont="/storage/af/user/greales/SingDir"				# location of the container (Tier II greales)
#ContName="G4SingCont.sif"						# Name of the singularity container (Tier II greales)

buildfol="buildSing"    # bulding folder name
SimName="LYSObarsim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut="WhateverNameTest"   # Name of the Root output files from the G4 executable 
cd $SingCont


#### sigularity exec examples
#singularity exec test1 sh /usr/TestFolder/test.sh
#singularity exec -B $BASEDIR test1 sh /usr/TestFolder/test.sh
#singularity exec -B $BASEDIR test1 sh $BASEDIR/test.sh
#singularity exec test1 sh $BASEDIR/test.sh
#singularity exec --bind test1 sh $BASEDIR/test.sh
#sh $BASEDIRsh

### Main action
ActionSH=$BASEDIR/G4CompileScript.sh
singularity exec -B $BASEDIR $SingCont/$ContName sh $ActionSH -c 1 -m 1 -s $G4Source -h $BASEDIR -f $buildfol
cd $BASEDIR
ActionSH=$BASEDIR/G4SimScript.sh
singularity exec -B $BASEDIR $SingCont/$ContName sh $ActionSH -a $SimArgs -n $SimName -s $G4Source -m $MacroName  -b $BASEDIR  -f $buildfol -v 0 -o $OutPut
