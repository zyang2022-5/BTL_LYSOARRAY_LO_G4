#!/usr/bin/env bash
# Default values for optional arguments
Mopt=1
Copt=0
Sopt="/storage/af/user/greales/simG4/SingLYSO"
CMAKElist="CMakeLists.txt"
fbuild="build"

SimName="sim"
fbuild="build"
macname="run.mac"
simargsprev="-m"
##Sopt="~/software/geant4/geant4-v11.0.0-install/share/Geant4-11.0.0/g$Sopt/eant4make/"
Valg=0

# Read optional arguments
while getopts ":s:m:f:b:a:n:v:o:" arg; do
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
        o) OutName=$OPTARG
            echo "*** Segmentation errors check change default"
            echo $OutName;; # Change G4 source directory
    esac
done
echo "*** Sourcing G4:"
#cd $Sopt
#. ./geant4make.sh
. $Sopt/G4SourceCVMFS.sh
#echo $(pwd)

### Running sim
if [ $Valg = 0 ]; then
    cd $BASEDIR
    echo $(pwd)
    ./$SimName -o Muon_Module_0922_1355 -GeomConfig 3 -mn runMuon.mac 10
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
    echo $(pwd)
    #valgrind --leak-check=yes /usr/TestFolder/source.sh
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./$SimName $simargsprev $macname
    cd $BASEDIR
fi
