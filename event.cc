#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*,MyG4Args* MainArgs)
{
    PassArgs=MainArgs;


    PDE = new G4PhysicsOrderedFreeVector();
    G4double Vov=PassArgs->GetVov();
    std::ifstream datafile;
    datafile.open("eff.dat");
    G4double wlendat, queffdat;
    G4int cont=0;
    G4double Eff420=(0.393 * 1.0228) * ( 1 - exp(-0.583*Vov) );
    while(!datafile.eof())
    {
        G4cout << cont << std::endl;
        cont=cont+1;
     
        datafile >> wlendat;
        datafile >> queffdat;
        G4cout << wlendat << " " << queffdat << std::endl;

        PDE->InsertValues(wlendat, queffdat*Eff420/1.);
    }

    datafile.close();
    PDE420 = PDE->Value(420.);
}

MyEventAction::~MyEventAction()
{

}

void MyEventAction::BeginOfEventAction(const G4Event *anEvent)
{
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    PassArgs->InitAllCount();

 // Run status
  G4int eventID=anEvent->GetEventID();
  G4Run* run = static_cast<G4Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
  G4int nOfEvents = run->GetNumberOfEventToBeProcessed();
  G4double _perCent = 10.; // status increment in percent

   
// Randomizing the impact point of the initial particle gun
if(PassArgs->GetRnd_Part()==1)
{
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4int GeomConfig  = PassArgs->GetGeomConfig();

// Setting limits to the randomizer for the particle gun
    G4double LYSO_L  = detectorConstruction->GetLYSOL();
    LYSO_L=LYSO_L-LYSO_L*0.01;
    G4double LYSO_T  = detectorConstruction->GetLYSOT();
    LYSO_T=LYSO_T-LYSO_T*0.01;
    G4double LYSO_T2  = detectorConstruction->GetLYSOT();
    LYSO_T2=LYSO_T2-LYSO_T2*0.01;

    //G4cout<< "Data from construction: "<< LYSO_L << " " << LYSO_T << " " << GeomConfig << G4endl;
    if (GeomConfig == 1){
        G4double GenX=(-LYSO_T+LYSO_T*2*G4UniformRand())/1000.;
        G4double GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
        command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command);     
        command = "/gun/direction 0. -1. 0."; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command); 
    }else if (GeomConfig == 2){
        G4double GenX=(-LYSO_T2+LYSO_T2*2*G4UniformRand())/1000.;
        G4double GenZ=(-LYSO_T+LYSO_T2*2*G4UniformRand())/1000.;
        command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(GenZ)+" -0.05 "+"m"; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command);     
        command = "/gun/direction 0. 0. 1."; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command); 
    }else if (GeomConfig == 3){
        G4double GenX=(-LYSO_T*2.*mm-0.194/2*mm+LYSO_T*mm*2*G4UniformRand())/1000.;
        G4double GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
        command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command);     
        command = "/gun/direction 0. -1. 0."; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command); 
}
}


    // Writing to screen when a certain number out of all the total events asked for are done
  if(fmod(eventID,double(nOfEvents*_perCent*0.01))==0)
  {
    time_t my_time = time(NULL);
    tm *ltm = localtime(&my_time);
    G4double status=(100*(eventID/double(nOfEvents))); 
    std::cout << "=> Event " << eventID << " start ("<< status << "%, "<< ltm->tm_hour << ":" <<  ltm->tm_min << ":" << ltm->tm_sec << ")" << std::endl;
  }






}


void MyEventAction::EndOfEventAction(const G4Event*)
{
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4double GLUEL  = detectorConstruction->GetGLUEL();
    G4double RESINL  = detectorConstruction->GetRESINL();
    G4double XPOS  = detectorConstruction->GetXPOS();
    G4double YPOS  = detectorConstruction->GetYPOS();
    G4double XPOS2  = detectorConstruction->GetXPOS2();
    G4double YPOS2  = detectorConstruction->GetYPOS2();
    G4int GeomConfig  = detectorConstruction->GetGC();
    G4int PC = PassArgs->GetLO();
    G4int CT = PassArgs->GetCT();
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    if(PassArgs->GetEdep()>0){

            if (GeomConfig == 1){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Energy deposition: " << PassArgs->GetEdep()/MeV << " [MeV] Performed :" << PassArgs->GetNEdep() << " times"<< G4endl;
    G4cout<< "Photons created end of event: " << PassArgs->GetTP() << G4endl;
    G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
    G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    G4cout<< "Estimated Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
    G4cout<< "Real Number of Photons Detected: " << PC << G4endl;
    G4cout<< "Photons detected in the Right SiPM: " << PassArgs->GetNPhotR() << G4endl;
    G4cout<< "Photons detected in the Left SiPM: " << PassArgs->GetNPhotL() << G4endl;
    G4cout<< "Light Output Average (LO/2.) end of event: " << PC/(PassArgs->GetEdep()/MeV)/2. << G4endl;
    if(PassArgs->GetTimeTrue()==1){G4cout<< "Global Timing: " << PassArgs->GetPhotTiming() << G4endl;}
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;


        }else if (GeomConfig == 2){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Energy deposition: " << PassArgs->GetEdep()/MeV << " [MeV] " << G4endl;
    G4cout<< "Photons created end of event: " << PassArgs->GetTP() << G4endl;
    G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
    G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    G4cout<< "Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
    G4cout<< "Estimated Light Output per SiPM " << PassArgs->GetPhHits()*PDE420/(PassArgs->GetEdep()/MeV) << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
        }else if (GeomConfig == 3){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    //G4cout<< "Event Nº: " << evt << G4endl; Print Run number!!
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Energy deposition: " << PassArgs->GetEdep()/MeV << " [MeV] Performed :" << PassArgs->GetNEdep() << " times"<< G4endl;
    G4cout<< "Photons created end of event: " << PassArgs->GetTP() << G4endl;
    G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
    G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    G4cout<< "Estimated Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
    G4cout<< "Real Number of Photons Detected: " << PC << G4endl;
    G4cout<< "Photons detected in the Right SiPM: " << PassArgs->GetNPhotR() << G4endl;
    G4cout<< "Photons detected in the Left SiPM: " << PassArgs->GetNPhotL() << G4endl;
    G4cout<< "Light Output Average (LO/2.) end of event: " << PC/(PassArgs->GetEdep()/MeV)/2. << G4endl;
    G4cout<< "Real Number of Cross-Talk Photons Detected: " << CT << G4endl;
    G4cout<< "Cross-Talk/MeV (nxSiPM) end of event: " << CT/(PassArgs->GetEdep()/MeV) << G4endl;
    if(PassArgs->GetTimeTrue()==1){G4cout<< "Global Timing: " << PassArgs->GetPhotTiming() << G4endl;}
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
        }
    }

    // Fill evt values
    if(PassArgs->Getnrep()>0){
    PassArgs->FillEvtLO(evt, PC/(PassArgs->GetEdep()/MeV)/2.);
    if(PassArgs->GetTimeTrue()==1){PassArgs->FillEvtTim(evt,  PassArgs->GetPhotTiming());}
    }   

if(PassArgs->GetTree_EndOfEvent()==1){
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(4, 0, PassArgs->GetEdep()/MeV);
    man->FillNtupleDColumn(4, 1, PassArgs->GetTP());
    man->FillNtupleDColumn(4, 2, PC);
    man->FillNtupleDColumn(4, 3, PDE420);
    man->FillNtupleDColumn(4, 4, PC/(PassArgs->GetEdep()/MeV)/2.);
    man->FillNtupleDColumn(4, 5, PXd*1000);
    man->FillNtupleDColumn(4, 6, PZd*1000);
    man->FillNtupleDColumn(4, 7, GLUEL/mm);
    man->FillNtupleDColumn(4, 8, RESINL/mm);
    man->FillNtupleDColumn(4, 9, XPOS/mm);
    man->FillNtupleDColumn(4, 10, YPOS/mm);
    man->FillNtupleDColumn(4, 11, XPOS2/mm);
    man->FillNtupleDColumn(4, 12, YPOS2/mm);
    if (GeomConfig == 3){
    man->FillNtupleDColumn(4, 13, CT);
    man->FillNtupleDColumn(4, 14, CT/(PassArgs->GetEdep()/MeV));}
    //else {man->FillNtupleDColumn(4, 11, 0.);}
    man->FillNtupleDColumn(4, 15, PassArgs->GetPhotTiming());
    man->FillNtupleDColumn(4, 16, evt);
    /*get ev number from detector!!!*/
    /*Write down particle gun position and angle (x,z,alpha_yz)*/
    man->AddNtupleRow(4);
}


}
