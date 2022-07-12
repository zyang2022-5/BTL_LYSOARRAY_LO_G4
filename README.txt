LYSO bar/tile Simulation for BTL
================================

Author: Guillermo Reales

Date: 08-Jun-2022

Summary
-------

This code generates different geometries of LYSO scintillators to estimate their Light Output (LO) with respect to the SiPM detectors.

To compile the software, there are three different methods you can use:

* singularity container
* compile from source
* use a pre-existing geant4/root installation on the Caltech Tier2 cluster

The instructions for each of these methods is described below.

Singularity Container
---------------------

The code is expected to work with the Singularity container that can be downloaded through the command:

    $ singularity pull library://grealesguti/test/test

If this provides an error try the UNIQE ID as:

    $ singularity pull library:sha256.4fa8d22a3afd9f8ba6f20494e629965887344f672f5a3f726ad9b7160396ae4b

The G4 version used is G4 v11.0.0

* Follow the steps in the Yvideo -> https://www.youtube.com/watch?v=Lxb4WZyKeCE&list=PLLybgCU6QCGWgzNYOV0SKen9vqg4KXeVL
	- Use G4 version 11.0.0

How to run on your machine:
* Download the Singularity Container stated above
* Modify SingAction.sh for your Container folder
* Create buildSing and buildSing/Results folder within the folder that contains this repository
* cd into the folder that contain this repository
* modify the run.mac file (These are the commands that will be executed by G4)
* run the command: sh SingAction.sh

Building From Source for Fedora/CentOS/RHEL
-------------------------------------------

First, install some required dependencies to build Geant4 and ROOT:

    $ sudo dnf groupinstall 'Development Tools'
    $ sudo dnf install expat expat-devel
    $ sudo dnf install git make cmake gcc-c++ gcc binutils libX11-devel libXpm-devel libXft-devel libXext-devel python openssl-devel

Note that depending on your exact version, some of the names of the above programs may change slightly.

Next, we build geant4 and ROOT. You can build these yourself by following the instructions at [here](https://geant4-userdoc.web.cern.ch/UsersGuides/InstallationGuide/html/installguide.html) and [here](https://root.cern/install/) (remember to source the shell files after!), or you can simply type:

    $ make install-deps

Now, go grab a cup of coffee because it takes a long time to build geant4 and root!

Afterwards, run:

    $ source ./geant4.10.07.p03/geant4-install/bin/geant4.sh
    $ source ./root-v6.24.04/root-install/bin/thisroot.sh

Finally, add these lines to your .bashrc script:

    $ echo "source ./geant4.10.07.p03/geant4-install/bin/geant4.sh" >> ~/.bashrc
    $ echo "source ./root-v6.24.04/root-install/bin/thisroot.sh" >> ~/.bashrc

Finally, compile the software:

    $ make

How to run on TierII
--------------------

* download the repository in a new folder in your node
* create a Results folder
* run Geant4SourceCVMFS.sh
* run the Makefile (`make -f Makefile.tier2`)
* modify the run.mac file (These are the commands that will be executed by G4)
* modify the G4SimScriptCVMFS.sh if required
* run G4SimScriptCVMFS.sh

An example of how to run within a cluster with HTcondor can be found in the folder "HTexample".

Running the code
----------------

### Arguments

    -m Macroname.mac    : run one time a macro file
    -mn i Macroname.mac : repeat a macro file i times
    -o OutputName       : Desired Output name for the root files without file extension type

### Outputs

Root files per event number containing:

* Information on the photons that reach the detector
* Information on the photons that are detected
* General information on the LO per event
* Information on all photons killed
    - Position where they where killed


### Schematics of the code

```
* sim.cc (main)
    ->  construction.*
        ->  detector.*
    ->  physics.*
    ->  action.*
        -> generator.*
        -> run.*
        -> event.*
        -> stepping.*
        -> tracking.*
```
