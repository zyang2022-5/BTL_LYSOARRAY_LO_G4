#!/bin/bash
# Default values for optional arguments
BASEDIR=$(pwd)
### SOURCING G4
# /bin/sh is usually some other shell trying to mimic The Shell. Many distributions use /bin/bash for sh, it supports source. On Ubuntu, though, /bin/dash is used which does not support source. Most shells use . instead of source. If you cannot edit the script, try to change the shell which runs it.
#https://stackoverflow.com/questions/13702425/source-command-not-found-in-sh-shell

# Default values for optional arguments
Mopt=1
Copt=0
Sopt="~/software/geant4/geant4-v11.0.0-install/share/Geant4-11.0.0/geant4make/"
CMAKElist="CMakeLists.txt"
fbuild="build"
# Read optional arguments
while getopts ":c:m:s:h:l:f:" arg; do
    case $arg in
        c) Copt=$OPTARG;;
                #oarg=$OPTARG
                #echo "COPT1";; # make command
        m) Mopt=$OPTARG;; # ccmake command
        s) Sopt=$OPTARG
            echo "*** Change of Source directory"
            echo $Sopt;; # Change G4 source directory
        h) BASEDIR=$OPTARG
            echo "*** Change of Base Directory"
            echo $BASEDIR;; # Change G4 source directory
        l) CMAKElist=$OPTARG
            echo "*** Changed CMakeLists file"
            echo $CMAKElist;; # Change G4 source directory
        f) fbuild=$OPTARG
            echo "*** Changed build folderd"
            echo $fbuild;; # Change G4 source directory
    esac
done

echo "*** Sourcing G4:"
cd $Sopt
. ./geant4make.sh
echo $(pwd)

### Compiling sim
if [ $Copt = 1 ]; then
echo "*** CMAKE:"
    cd $BASEDIR
    cd ./$fbuild
    cmake .. #/$CMAKElist
    cd $BASEDIR
fi
if [ $Mopt = 1 ]; then
echo "*** MAKE:"
    cd $BASEDIR
    cd ./$fbuild
    make 
    cd $BASEDIR
fi

echo "*** Test Finished"

