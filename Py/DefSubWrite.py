import sys
import os
from os.path import exists
import subprocess
import numpy as np
import time
import ROOT
from os import listdir
from os.path import isfile, join
import os.path
from os import path

class G4Job:
        def __init__(self,CurrentFolder="/storage/af/user/greales/simG4/BTL_LYSOARRAY_LO_G4/",OutFolder="/storage/af/user/greales/simG4/outputs/", SubName="SubDefaultName", OutName="Out_NSGA_POP_", JobName="JobActionGC1FLResinMuon.sh", Pop=1, IndvN=100):
                self.CurrentFolder = CurrentFolder
                self.OutFolder = OutFolder
                self.SubName = SubName
                self.OutName = OutName
                self.JobName = JobName
                self.Pop = Pop
                self.IndvN = IndvN

def CleanOut(G4Job):

    p = subprocess.call(['rm',G4Job.OutFolder+G4Job.OutName+"*"])
    return 0

def SubWrite(G4Job , Children=[]):

    f = open("SubFiles/"+G4Job.SubName+".sub", "a")
    f.write("Universe = vanilla\n")
    f.write("executable = "+G4Job.CurrentFolder+"JofFiles/"+G4Job.JobName+"\n")
    f.write('arguments ="-a Pop_'+str(G4Job.Pop)+'_indv_$(Item)_"\n')
    f.write("Output  ="+G4Job.OutFolder+G4Job.OutName+str(G4Job.Pop)+".out"+"\n")
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


def SubLaunch(G4Job):
    p = subprocess.call(["condor_submit",G4Job.CurrentFolder+"SubFiles/"+G4Job.SubName+".sub"])
    return 0

def SubMonitor(G4Job, wait=2, maxwait=3600, ptime=60):
    tc=0
    Subname=G4Job.OutFolder+G4Job.OutName+str(G4Job.Pop)+".out"
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
    c=0
    for f in Popfiles:
        inFile = ROOT . TFile . Open ( Folder+f ," READ ")
        tree = inFile . Get ("EndOfRun")
        tree.GetEntry(0)
                    # get Indv number from file name!!    
        arr[c] = getattr(tree,"fLCAvg")
        print("LCAvg: ",c,arr[c])
        c+=1

def Obj(PopIndv):
    SubWrite()
    SubLaunch()
    SubMonitor()
    PyROOTLCAvg()

def AllAtOnce():
    test = G4Job()
    SubWrite(test)
    SubLaunch(test)
    
          







        
    
    





