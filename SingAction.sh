#!/bin/bash
# Default values for optional arguments
BASEDIR=$(pwd)
G4Source="/usr/Geant4/geant4v11.p0/share/Geant4-11.0.0/geant4make/"
SingCont="/home/guillermo/localdev/ubuntutest/"
buildfol="buildSing"
ContName="test1"
SimName="LYSObarsim"
MacroName="run.mac"
SimArgs="-m"
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
singularity exec -B $BASEDIR $SingCont/$ContName sh $ActionSH -a $SimArgs -n $SimName -s $G4Source -m $MacroName  -b $BASEDIR  -f $buildfol -v 0
