#!/bin/bash
# Default values for optional arguments
BASEDIR=$(pwd)
### SOURCING G4
# /bin/sh is usually some other shell trying to mimic The Shell. Many distributions use /bin/bash for sh, it supports source. On Ubuntu, though, /bin/dash is used which does not support source. Most shells use . instead of source. If you cannot edit the script, try to change the shell which runs it.
#https://stackoverflow.com/questions/13702425/source-command-not-found-in-sh-shell

# Default values for optional arguments
Mopt=1
# Read optional arguments
while getopts ":c:m:s:h:cf:" arg; do
    case $arg in
        c) Copt=$OPTARG;;
                #oarg=$OPTARG
                #echo "COPT1";; # make command
        m) Mopt=$OPTARG;; # ccmake command
        h) BASEDIR=$OPTARG
            echo "*** Change of Base Directory"
            echo $BASEDIR;; # Change G4 source directory
    esac
done

echo "*** Sourcing G4:"
cd $(BASEDIR)
. ./G4Source.sh
echo $(pwd)

if [ $Mopt = 1 ]; then
echo "*** MAKE:"
    make -f Makefile.tier2
fi

echo "*** Compilation Finished"

