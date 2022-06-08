#include "event.hh"

MyEventAction::MyEventAction(MyRunAction*)
{
    fEdep=0.;   // Count initialization of energy deposited by gamma particles in the LYSO
    fPhCount=0; // Count initialization of photon impacts in the detector
    fLO=0;      // Initialization of LO

    PDE = new G4PhysicsOrderedFreeVector();
    G4double Vov=3.5; // Not set as input!
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
    PDE420 = PDE->Value(420.); // PDE estimated for 420nm wavelength
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
    //UImanager->ApplyCommand("/run/reinitializeGeometry"); only works between runs
}

MyEventAction::~MyEventAction()
{

}

void MyEventAction::BeginOfEventAction(const G4Event *anEvent)
{
    // Need to reinitialize vis options if the geometry is reinitialized. To set at the end of the run to be more efficient. Not required for the use in the cluster.
    //G4UImanager *UImanager = G4UImanager::GetUIpointer();
    //command="/vis/initialize ";
    //UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    //command="/vis/drawVolume";
    //UImanager->ApplyCommand(command);  G4cout<< command << G4endl;
    //command="/vis/scene/add/trajectories smooth";
    //UImanager->ApplyCommand(command);  G4cout<< command << G4endl;

     // Run status
      G4int eventID=anEvent->GetEventID();
      G4Run* run = static_cast<G4Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );
      G4int nOfEvents = run->GetNumberOfEventToBeProcessed();
      G4double perCent = 10.; // status increment in percent

    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4int GeomConfig  = detectorConstruction->GetGC();

    // Minimum distance to the edge of the LYSO to generate the particle gun
    G4double LYSO_L  = detectorConstruction->GetLYSOL();
    LYSO_L=LYSO_L-LYSO_L*0.01;
    G4double LYSO_T  = detectorConstruction->GetLYSOT();
    LYSO_T=LYSO_T-LYSO_T*0.01;// This should go at the end of the run or with the rest of geometry changes

    G4cout<< "Data from construction: "<< LYSO_L << " " << LYSO_T << " " << GeomConfig << G4endl;
    

    // Change of gun position between events depending on geometry (LYSO bar/tile)
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
        G4double GenX=(-LYSO_T+LYSO_T*2*G4UniformRand())/1000.;
        G4double GenZ=(-LYSO_T+LYSO_T*2*G4UniformRand())/1000.;
        command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(GenZ)+" -0.05 "+"m"; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command);     
        command = "/gun/direction 0. 0. 1."; 
        G4cout<< command << G4endl;
        UImanager->ApplyCommand(command); 
    }


// Write down data to screen if we are at a certain percentage of the total number of events
  if(fmod(eventID,double(nOfEvents*perCent*0.01))==0)
  {
    time_t my_time = time(NULL);
    tm *ltm = localtime(&my_time);
    G4double status=(100*(eventID/double(nOfEvents))); 
    std::cout << "=> Event " << eventID << " start ("<< status << "%, "<< ltm->tm_hour << ":" <<  ltm->tm_min << ":" << ltm->tm_sec << ")" << std::endl;
  }


// Reinitialize counters -> Is it needed? or these are deleted in between events?
    fEdep=0.;
    fPhCount=0;
    fLO=0;
}


void MyEventAction::EndOfEventAction(const G4Event*)
{
        G4UImanager *UImanager = G4UImanager::GetUIpointer();
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4double LYSO_L  = detectorConstruction->GetLYSOL();
    G4double GLUEL  = detectorConstruction->GetGLUEL();
    G4double RESINL  = detectorConstruction->GetRESINL();
    G4double XPOS  = detectorConstruction->GetXPOS();
    G4double YPOS  = detectorConstruction->GetYPOS();
    
    // Store and write (only is there was photon creation in the LYSO) data and estimated data for the event
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    if(fEdep>0){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Energy deposition: " << fEdep/MeV << " [MeV] " << G4endl;
    G4cout<< "Photons created end of event: " << fPhCount << G4endl;
    G4cout<< "Photon Hits end of event: " << fLO << G4endl;
    G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    G4cout<< "Photon Detected (420nm PDE, 3.5OV) end of event: " << fLO*PDE420 << G4endl;
    G4cout<< "Estimated Light Output per SiPM " << fLO*PDE420/(fEdep/MeV)/2. << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    }
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    man->FillNtupleDColumn(2, 0, fEdep/MeV);
    man->FillNtupleDColumn(2, 1, fPhCount);
    man->FillNtupleDColumn(2, 2, fLO);
    man->FillNtupleDColumn(2, 3, PDE420);
    man->FillNtupleDColumn(2, 4, fLO*PDE420/(fEdep/MeV)/2.);
    man->FillNtupleDColumn(2, 5, PXd*1000);
    man->FillNtupleDColumn(2, 6, PZd*1000);
    man->FillNtupleDColumn(2, 7, GLUEL/mm);
    man->FillNtupleDColumn(2, 8, RESINL/mm);
    man->FillNtupleDColumn(2, 9, XPOS/mm);
    man->FillNtupleDColumn(2, 10, YPOS/mm);
    man->FillNtupleDColumn(2, 11, evt);
    man->AddNtupleRow(2);

 
}
