import sys
import os
from os.path import exists
import numpy as np
import time
import subprocess
import ROOT
from os import listdir
from os.path import isfile, join
import os.path
from os import path

def CleanOut(Folder = "/storage/af/user/greales/simG4/outputs/", Name="Out_job_Pop_*"):

    p = subprocess.call(['rm',Folder+Name])
    return 0

def SubWrite(Output = "SubDefaultName.sub",CurrentFolder="/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4/JobFiles",JobName="/JobFiles/JobActionGC1FLResinMuon.sh", Pop=0, IndvN=0 , Children=[]):

    f = open(Output, "a")
    f.write("Universe = vanilla\n")
    f.write("executable = "+CurrentFolder+JobName+"\n")
    f.write('arguments ="-a Pop_'+str(Pop)+'_indv_$(Item)_"\n')
    f.write("Output  = /storage/af/user/greales/simG4/outputs/Out_job$(Cluster).out\n")
    f.write("Error   = /storage/af/user/greales/simG4/errors/error_job$(Cluster).out\n")
    f.write("Log     = /storage/af/user/greales/simG4/logs/log_job$(Cluster).out\n")
    f.write("requirements = Machine =!= LastRemoteHost\n")
    f.write("WHEN_TO_TRANSFER_OUTPUT = ON_EXIT_OR_EVICT\n")
    f.write('+JobQueue = "Normal"\n')
    f.write("+MaxRuntime = 7000\n")
    f.write("+RunAsOwner = True\n")
    f.write("+InteractiveUser = True\n")
    f.write("+SingularityBindCVMFS = True\n")
    f.write('+SingularityImage = "/cvmfs/singularity.opensciencegrid.org/cmssw/cms:rhel7"\n')
    f.write("x509userproxy = $ENV(X509_USER_PROXY)\n")
    f.write("RequestDisk = 2000000\n")
    f.write("RequestMemory = 2000\n")
    f.write("RequestCpus = 1\n")
    f.write("on_exit_remove = ((ExitBySignal == False) && (ExitCode == 0)) || (JobStatus=?=3)\n")
    f.write("on_exit_hold = (ExitBySignal == True) || (ExitCode != 0)\n")
    f.write("+PeriodicRemove = ((JobStatus =?= 2) && ((MemoryUsage =!= UNDEFINED && MemoryUsage > 2.5*RequestMemory)))\n")
    f.write("should_transfer_files = Yes\n")
    f.write("max_retries = 3\n")
    f.write("Queue 1\n")
    f.close()
    return 0

def SubLaunch(Subname = "SubDefaultName.sub",CurrentFolder="/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4/JobFiles"):

    SubExec="condor_submit "+CurrentFolder+Subname
    p = subprocess.run(SubExec)
    return 0
def SubExec(cmd="ls"):

    p = subprocess.run(cmd)
    return 0

def SubMonitor(Subname = "/storage/af/user/greales/simG4/outputs/Out_job_Pop_1.out", wait=2, maxwait=3600, ptime=60):
    tc=0
    if(path.exists(Subname)):
        print("File Found.")
        subprocess.call(["date"])
        return 0
    else:
        print("Waiting")
        subprocess.call(["date"])
        while(tc<maxwait):
            #print(".", end = "")   
            print(tc/60)
            #sys.stdout.write(". ")
            time.sleep(wait)
            tc+=wait
            if(tc>maxwait):
                print("Time Limit") 
                subprocess.call(["date"])
                return 1

def PyROOTLCAvg(PopName="Out_NSGA_POP_",Pop=0,Folder="./Results/",Indv=100):
    arr=[]
    arr = [0 for i in range(Indv)] 
    Popfiles=[f for f in listdir(Folder) if f.startswith(PopName)]
    for f in Popfiles:
        inFile = ROOT . TFile . Open ( Folder+f ," READ ")
        tree = inFile . Get ("EndOfRun")
        tree.GetEntry(0)
                    # get Indv number from file name!!    
        #arr[c] = getattr(tree,"fLCAvg")


def Obj(PopIndv):
    SubWrite()
    SubLaunch()
    SubMonitor()
    PyROOTLCAvg()
          







        
    
    





