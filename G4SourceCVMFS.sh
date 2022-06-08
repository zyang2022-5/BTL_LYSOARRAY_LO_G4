#!/bin/bash
export LCGENV_PATH=/cvmfs/sft.cern.ch/lcg/releases/
# geant4-config --version -> 11.0.1
eval "` $LCGENV_PATH/lcgenv/latest/lcgenv -p LCG_102rc1 x86_64-centos7-gcc11-opt Geant4 `"


