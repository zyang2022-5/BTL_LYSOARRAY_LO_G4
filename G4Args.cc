#include "G4Args.hh"


MyG4Args :: MyG4Args(int mainargc,char** mainargv)
{
    G4cout << " ### Processing Command line Arguments to the sim : " << G4endl;

    for (int j = 1; j < mainargc; j=j+1){
                if(strcmp(mainargv[j],"-o")==0)
                {   
                    OutName = mainargv[j+1];      j=j+1;
                    G4cout<< " ### Changed Ouput name to : " << OutName<<G4endl;            
                    Oin=1;         
                }
                else if(strcmp(mainargv[j],"-Vov")==0)
                {   
                    Detection[0] = atof(mainargv[j+1]);      j=j+1;
                    G4cout<< " ### Changed Overvoltage to : " << Detection[0] <<G4endl;              
                }
                else if(strcmp(mainargv[j],"-mn")==0)
                {   
                    VisTrue=0;
                    MacName = mainargv[j+1];
                    nrep =  atoi(mainargv[j+2]);j=j+2;
                    G4cout<< " ### Set Run Macro name to : " << MacName <<G4endl;         
                    G4cout<< " ### Number of runs : " << nrep <<G4endl;     
                    nEvents=FindEvents(MacName);
                    G4cout<< " ### Number of events : " << nEvents <<G4endl;  
                    nEventTiming = new G4double[nEvents];
                    nEventLO = new G4double[nEvents];

                    nRunTimingAvg = new G4double[nrep];
                    nRuntLOAvg = new G4double[nrep];
                    nRunTimingStd = new G4double[nrep];
                    nRuntLOStd = new G4double[nrep];
                    nEdepEvts = new G4int[nrep];
                    MainTrees[5]=1;

                }
                else if(strcmp(mainargv[j],"-m")==0)
                {   
                    VisTrue=0;
                    MacName = mainargv[j+1];j=j+1;
                    nrep =  1;      
                    G4cout<< " ### Set Run Macro name to : " << MacName <<G4endl;         
                    G4cout<< " ### Number of runs : " << nrep <<G4endl;  
                    nEvents=FindEvents(MacName);
                    G4cout<< " ### Number of events : " << nEvents <<G4endl;  
                    nEventTiming = new G4double[nEvents];
                    nEventLO = new G4double[nEvents];

                    nRunTimingAvg = new G4double[nrep];
                    nRuntLOAvg = new G4double[nrep];
                    nRunTimingStd = new G4double[nrep];
                    nRuntLOStd = new G4double[nrep];
                    nEdepEvts = new G4int[nrep];
                    MainTrees[5]=1;
                }
                else if(strcmp(mainargv[j],"-rnd")==0)
                {   
                    RndGen[0] = atoi(mainargv[j+1]);
                    if (RndGen[0]==0)
                    {
                        RndGen[1] = 0;
                        RndGen[2] = 0;j=j+1;
                        G4cout<< " ### Remove all random generators. " <<G4endl; 
                    }
                    else
                    {
                        RndGen[1] = atoi(mainargv[j+2]);
                        RndGen[2] = atoi(mainargv[j+3]);j=j+3;
                    G4cout<< " ### Random Particle Position set to  : " << RndGen[1] <<G4endl;         
                    G4cout<< " ### Random Geometry Parameters set to  : " << RndGen[2] <<G4endl;                          
                    } 
                }
                else if(strcmp(mainargv[j],"-Arrivals")==0)
                {   
                    MainTrees[0] = 1;
                    G4cout<< " ### Storing Tree Detected" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-Detected")==0)
                {   
                    MainTrees[1] = 1;
                    G4cout<< " ### Storing Tree Detected" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-Stepping")==0)
                {   
                    MainTrees[2] = 1;
                    G4cout<< " ### Storing Tree Tracking" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-Tracking")==0)
                {   
                    MainTrees[3] = 1;
                    G4cout<< " ### Storing Tree Tracking" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-EndOfEvent")==0)
                {   
                    MainTrees[4] = 1;
                    G4cout<< " ### Storing Tree EndOfEvent" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-nArrivals")==0)
                {   
                    MainTrees[0] = 0;
                    G4cout<< " ### Not! Storing Tree Detected" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-nDetected")==0)
                {   
                    MainTrees[1] = 0;
                    G4cout<< " ### Not! Storing Tree Detected" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-nStepping")==0)
                {   
                    MainTrees[2] = 0;
                    G4cout<< " ### Not! Storing Tree Tracking" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-nTracking")==0)
                {   
                    MainTrees[3] = 0;
                    G4cout<< " ### Not! Storing Tree Tracking" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-nEndOfEvent")==0)
                {   
                    MainTrees[4] = 0;
                    G4cout<< " ### Not! Storing Tree EndOfEvent" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-TreeAll")==0)
                {   
                    for (int Tree = 0; Tree < 5; Tree=Tree+1){
                        MainTrees[Tree] = 1;
                    }
                    G4cout<< " ### Storing All Trees" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-TreeN")==0)
                {   
                    MainTrees[atoi(mainargv[j+1])] = 1;j=j+1;
                    G4cout<< " ### Storing All Trees" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-LYSO_L")==0)
                {   
                    Geom_LYSO[2] = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### LYSO_L modified to :"<< Geom_LYSO[2]*2 <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-LYSO_Yield")==0)
                {   
                    LYSOProps[0] = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### LYSO_Yield modified to :"<< LYSOProps[0] <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-LYSO_RiseT")==0)
                {   
                    LYSOProps[2] = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### LYSO_RiseT modified to :"<< LYSOProps[2] <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-LYSO_DecayT")==0)
                {   
                    LYSOProps[3] = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### LYSO_DecayT modified to :"<< LYSOProps[3] <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-GeomConfig")==0)
                {   
                    GeomConfig = atoi(mainargv[j+1]); j=j+1;
                    G4cout<< " ### Geometry Configuration Changed to :"<< GeomConfig <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-NPhotTiming")==0)
                {   
                    NPhotTiming = atoi(mainargv[j+1]); j=j+1;
                    G4cout<< " ### Number of photons used for the timing Changed to :"<< NPhotTiming <<G4endl;         
                }else if(strcmp(mainargv[j],"-noTiming")==0)
                {   
                    TimeTrue = 0;
                    G4cout<< " ### No timing calculation performed" <<G4endl;         
                }else if(strcmp(mainargv[j],"-KillLT")==0)
                {   
                    KillLTTrue = 1;KillLTime = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### Killing photons after "<< KillLTime <<" ps" <<G4endl;         
                }
                else if(strcmp(mainargv[j],"-incr")==0)
                {   
                    incr = atof(mainargv[j+1]);j=j+1;
                    G4cout<< " ### The thickness of the crystal in the middle is changed to "<< incr <<G4endl;         
                }
        }

    if (Oin == 0 ) {  OutName = DefOutName;   }

}
MyG4Args :: ~MyG4Args()
{}

void MyG4Args :: AddPhotTiming(G4double Hitpos, G4double MeasTime ){


    //int i;
    if(Hitpos>0) {
        if(nPhotR<=NPhotTiming){
            TListR[nPhotR-1]=MeasTime;
        } else if (nPhotR==NPhotTiming+1){
           quickSort(TListR, 0, NPhotTiming - 1);
            if(TListR[NPhotTiming-1]>MeasTime){
                TListR[NPhotTiming-1]=MeasTime;
                quickSort(TListR, 0, NPhotTiming - 1);
            }
        } else {
            if(TListR[NPhotTiming-1]>MeasTime){
                TListR[NPhotTiming-1]=MeasTime;
                quickSort(TListR, 0, NPhotTiming - 1);
            }
        }

    }else{
        if(nPhotL<=NPhotTiming){
            TListL[nPhotL-1]=MeasTime;  
        } else if (nPhotL==NPhotTiming+1){
           quickSort(TListL, 0, NPhotTiming - 1);
            if(TListR[NPhotTiming-1]>MeasTime){
                TListR[NPhotTiming-1]=MeasTime;
                quickSort(TListR, 0, NPhotTiming - 1);
            }
        } else {
            if(TListR[NPhotTiming-1]>MeasTime){
                TListR[NPhotTiming-1]=MeasTime;
                quickSort(TListR, 0, NPhotTiming - 1);
            }
        }
    }
}    


G4double MyG4Args :: GetPhotTiming(){
int j;
    for (j = 0; j < NPhotTiming; j+=1){
    //G4cout<< "Photon Gtimes : " << TListL[j] << " "<< TListR[j] << G4endl;
        AvgTiming+=(TListL[j]+TListR[j]);
    }
    AvgTiming=AvgTiming/NPhotTiming/2.;
    return AvgTiming;
}


void MyG4Args :: GeomReinit( ){
G4UImanager *UImanager = G4UImanager::GetUIpointer();
G4String command="/run/reinitializeGeometry";
UImanager->ApplyCommand(command);  
G4cout<< command << G4endl;
}

G4int MyG4Args :: FindEvents(G4String macname ){

    //std::ifstream macfile(macname);
    std::ifstream macfile;
    macfile.open(macname);
    std::string line;
    G4int cont, nEvent;
    if (macfile)
    {

        while(!macfile.eof())
        {
            cont=cont+1;
         
            macfile >> line;
            G4cout << line  << G4endl;
            if (line.compare("/run/beamOn") == 0){
            //G4cout << "Found line: " << line  << G4endl;            
            macfile >> line;
            nEvent=atoi(line.c_str());
            //G4cout << "nEvents found: " << nEvent  << G4endl;
            }


        }
        //std::string line = getLastLine(file);
        //std::cout << line << '\n';
        
    }
    else{std::cout << "Unable to open file.\n";}


    return nEvent;
}

void MyG4Args :: FillAvgTim(G4int runid){
    nRunTimingAvg[runid]=0;
    G4int cnt=0;
    for (int j = 1; j < nEvents; j=j+1){
        if(nEventTiming[j]>0){
            cnt+=1;
            nRunTimingAvg[runid]+=nEventTiming[j];      
        }
    }
    nRunTimingAvg[runid]=nRunTimingAvg[runid]/cnt;
    nEdepEvts[runid]=cnt;
}

void MyG4Args :: FillAvgLO(G4int runid) {

    nRuntLOAvg[runid]=0;
    G4int cnt=0;
    for (int j = 1; j < nEvents; j=j+1){
        if(nEventLO[j]>0){
            cnt+=1;
            nRuntLOAvg[runid]+=nEventLO[j];      
        }
    }
    nRuntLOAvg[runid]=nRuntLOAvg[runid]/cnt;
    nEdepEvts[runid]=cnt;
}

void MyG4Args :: FillStdTim(G4int runid){

    G4int cnt=0;
    for (int j = 1; j < nEvents; j=j+1){
        if(nEventTiming[j]>0){
            cnt+=1;
            nRunTimingStd[runid]+=pow(nEventTiming[j]-nRunTimingAvg[runid],2);      
        }
    }
    nRunTimingStd[runid]=pow(nRunTimingStd[runid]/cnt,0.5);
    nEdepEvts[runid]=cnt;
}
    void MyG4Args :: FillStdLO(G4int runid){
    G4int cnt=0;
    for (int j = 1; j < nEvents; j=j+1){
        if(nEventLO[j]>0){
            cnt+=1;
            nRuntLOStd[runid]+=pow(nEventLO[j]-nRuntLOAvg[runid],2);      
        }
    }
    nRuntLOStd[runid]=pow(nRuntLOStd[runid]/cnt,0.5);
    nEdepEvts[runid]=cnt;
}





