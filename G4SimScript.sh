#!/bin/bash
# Default values for optional arguments
BASEDIR=$(pwd)
### SOURCING G4
# /bin/sh is usually some other shell trying to mimic The Shell. Many distributions use /bin/bash for sh, it supports source. On Ubuntu, though, /bin/dash is used which does not support source. Most shells use . instead of source. If you cannot edit the script, try to change the shell which runs it.
#https://stackoverflow.com/questions/13702425/source-command-not-found-in-sh-shell

# Default values for optional arguments
SimName="sim"
fbuild="build"
macname="run.mac"
simargsprev="-m"
Sopt="~/software/geant4/geant4-v11.0.0-install/share/Geant4-11.0.0/geant4make/"
Valg=0
# Read optional arguments
while getopts ":s:m:f:b:a:n:v:" arg; do
    case $arg in
        a) simargs=$OPTARG
            echo "*** Change of Simulation Arguments"
            echo $simargsprev;; # Change G4 source directory
        n) SimName=$OPTARG
            echo "*** Change of Simulation Name"
            echo $SimName;; # Change G4 source directory
        s) Sopt=$OPTARG
            echo "*** Change of Source directory"
            echo $Sopt;; # Change G4 source directory
        m) macname=$OPTARG
            echo "*** Change of Macro File name"
            echo $macname;; # Change G4 source directory
        b) BASEDIR=$OPTARG
            echo "*** Change of Base Directory"
            echo $BASEDIR;; # Change G4 source directory
        f) fbuild=$OPTARG
            echo "*** Changed build folderd"
            echo $fbuild;; # Change G4 source directory
        v) Valg=$OPTARG
            echo "*** Segmentation errors check change default"
            echo $Valg;; # Change G4 source directory
    esac
done

echo "*** Sourcing G4:"
cd $Sopt
. ./geant4make.sh
echo $(pwd)

### Running sim
if [ $Valg = 0 ]; then
    cd $BASEDIR
    cd ./$fbuild
    echo $(pwd)
    ./$SimName $simargsprev $macname
    cd $BASEDIR
fi

if [ $Valg = 1 ]; then
echo "*** Valgrind Check:"
ValArgs="--leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         "
Valexec="valgrind"
    cd $BASEDIR
    cd ./$fbuild
    echo $(pwd)
    #valgrind --leak-check=yes /usr/TestFolder/source.sh
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$SimName $simargsprev $macname
    cd $BASEDIR
fi


echo "*** Sim Run Finished."

