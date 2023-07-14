#Default values for optional arguments
#BASEDIR=$(pwd)          # Current folder where the G4 files can be found
BASEDIR="/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4/"

SimName="sim"    # Name of the G4 executable
MacroName="run.mac"     # Name of the macro file to run with the G4 executable
SimArgs="-m"            # Arguments for the G4 executable (any)
OutPut=""   # Name of the Root output files from the G4 executable 
Vars="{1.0-1.0}"
Nsections="1"
LYSOLen="28.5"

while getopts "a:n:s:v:z:l:" arg; do
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
        v) Vars=$OPTARG
            echo "*** Change of Vars"
            echo $Vars;; # Change G4 source directory
        z) Nsections=$OPTARG
            echo "*** Change of Nsections"
            echo $Nsections;; # Change G4 source directory
        l) LYSOLen=$OPTARG
            echo "*** Change of LYSO Length"
            echo $LYSOLen;; # Change G4 source directory
    esac
done

#echo "*** Sourcing G4:"
#. $BASEDIR/G4SourceCVMFS.sh

echo "Running Simulation" 
OutPut=$OutPut$Sopt
echo $Sopt
cd $BASEDIR


./$SimName -o $OutPut -GeomConfig 13 -rnd 1 1 0 -Znode 1 -Zelem 1 -Ypos {1-1} -ESRbackpainted -runevt 10 -Muon

#./$SimName -o $OutPut -GeomConfig 3 -m run.mac -Muon
