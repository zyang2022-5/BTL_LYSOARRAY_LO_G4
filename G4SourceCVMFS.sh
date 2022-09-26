#!/usr/bin/env bash
export LCGENV_PATH=/cvmfs/sft.cern.ch/lcg/releases/
# geant4-config --version -> 11.0.1.
#eval "` /cvmfs/sft.cern.ch/lcg/releases/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt Geant4 `"
#. /cvmfs/sft.cern.ch/lcg/releases/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt Geant4 
eval "` $LCGENV_PATH/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt Geant4 `"
echo `which geant4-config` 
echo "***G4 is added to path"
eval "` $LCGENV_PATH/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt ROOT `"
echo `root --version` 
echo "***root is added to path"
eval "` $LCGENV_PATH/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt CMake `"
echo `cmake --version` 
echo "***CMAKE is added to path"

