#include "event.hh"
#include "util.hh"
#include <G4DigiManager.hh>
#include <G4HCofThisEvent.hh>
#include <G4DCofThisEvent.hh>
#include <G4VHitsCollection.hh>
#include <G4VDigiCollection.hh>
#include "hit/G4SipmHit.hh"
#include "digi/G4SipmDigi.hh"
#include "digi/G4SipmVoltageTraceDigi.hh"
#include "model/G4SipmModel.hh"
#include "model/G4SipmVoltageTraceModel.hh"

MyEventAction::MyEventAction(MyRunAction*, MyG4Args* MainArgs)
{
    double wavelength[1000], qe[1000];

    PassArgs = MainArgs;

    PDE = new G4PhysicsOrderedFreeVector();
    G4double Vov = PassArgs->GetVov();
    G4double Eff420 = (0.393 * 1.0228) * (1 - exp(-0.583*Vov));

    int n = read_tsv_file("eff.dat", wavelength, qe, 1, Eff420);

    if (n == -1) {
        fprintf(stderr, "error reading eff.dat! Did you remember to source the env.sh file?\n");
        exit(1);
    }

    for (int i = 0; i < n; i++)
        PDE->InsertValues(wavelength[i], qe[i]);

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


    // Writing to screen when a certain number out of all the total events asked for are done
  if(fmod(eventID,double(nOfEvents*_perCent*0.01))==0)
  {
    time_t my_time = time(NULL);
    tm *ltm = localtime(&my_time);
    G4double status=(100*(eventID/double(nOfEvents))); 
    std::cout << "=> Event " << eventID << " start ("<< status << "%, "<< ltm->tm_hour << ":" <<  ltm->tm_min << ":" << ltm->tm_sec << ")" << std::endl;
  }






}


void MyEventAction::EndOfEventAction(const G4Event *anEvent)
{
    const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4double GLUEL  = detectorConstruction->GetGLUEL();
    G4double RESINL  = detectorConstruction->GetRESINL();
    G4double XPOS  = detectorConstruction->GetXPOS();
    G4double YPOS  = detectorConstruction->GetYPOS();
    G4double XPOS2  = detectorConstruction->GetXPOS2();
    G4double YPOS2  = detectorConstruction->GetYPOS2();
    G4int GeomConfig  = detectorConstruction->GetGC();
 
 		
    G4AnalysisManager *man = G4AnalysisManager::Instance();
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
//digitize the modules
    G4int RealSipm = PassArgs->GetG4SipmState();
    if (RealSipm){
	    G4DigiManager* digiManager = G4DigiManager::GetDMpointer();
	    G4DCtable* dcTable = digiManager->GetDCtable();
	    for (int i = 0; i < dcTable->entries(); i++) {
	       G4String dmName = dcTable->GetDMname(i);
	       G4VDigitizerModule* dm = digiManager->FindDigitizerModule(dmName);
	       if (dm) {
		       dm->Digitize();
	       }
            }

	    G4HCofThisEvent* hCof = anEvent->GetHCofThisEvent();
	    if (hCof != NULL) {
			for (int i = 0; i < hCof->GetCapacity(); ++i) {
				G4VHitsCollection* hc = hCof->GetHC(i);
				if (hc != NULL) {
					if (dynamic_cast<G4SipmHitsCollection*>(hc)) {
						for (size_t j = 0; j < hc->GetSize(); j++) {
							G4SipmHit* hit = (G4SipmHit*) hc->GetHit(j);
							G4int sipmId = hit->getSipmId();
							//G4int pdgId = hit->getPdgId(); //unused
							G4int trackId = hit->getTrackId();
							G4double eKin = hit->getEKin();
							G4double time = hit->getTime();
							//G4double weight = hit->getWeight(); //unused
							G4ThreeVector direction = hit->getMomentum();
							G4ThreeVector posPhoton = hit->getWorldPosition();
							G4double wlen = 1239.841939*eV/eKin;

							if (PassArgs->GetTree_Detected() == 1){
								man->FillNtupleIColumn(1, 0,  evt);
								man->FillNtupleIColumn(1, 1,  trackId);
								man->FillNtupleDColumn(1, 2,  posPhoton[0]/mm);// D==double
								man->FillNtupleDColumn(1, 3,  posPhoton[1]/mm);
								man->FillNtupleDColumn(1, 4,  posPhoton[2]/mm);
								man->FillNtupleDColumn(1, 5,  time/ps);
								man->FillNtupleDColumn(1, 8,  wlen);
								man->FillNtupleDColumn(1, 10,  direction[0]);// D==double
								man->FillNtupleDColumn(1, 11,  direction[1]);
								man->FillNtupleDColumn(1, 12,  direction[2]);    
								man->AddNtupleRow(1);
							}

							if (PassArgs->GetGeomConfig()==3 || PassArgs->GetGeomConfig()==13 && posPhoton[0]/mm>-1*(PassArgs->GetGeom_LYSO_thick())*2+0.1 && posPhoton[0]/mm<-0.01){
								PassArgs->AddLO();
								if(posPhoton[2]/mm>0){
									PassArgs->AddPhotR();
								}else{
									PassArgs->AddPhotL();
								}
							if(PassArgs->GetTimeTrue()==1){PassArgs->AddPhotTiming(posPhoton[2]/mm , time/ps);}

							} else if (PassArgs->GetGeomConfig()==3 || PassArgs->GetGeomConfig()==13 ){PassArgs->AddCT();
							} else if (PassArgs->GetGeomConfig()==1 || PassArgs->GetGeomConfig()==11){
								PassArgs->AddLO();
								if(posPhoton[2]/mm>0){
									PassArgs->AddPhotR();
								}else{
									PassArgs->AddPhotL();
								}
								if(PassArgs->GetTimeTrue()==1){PassArgs->AddPhotTiming(posPhoton[2]/mm , time/ps);}
							}
						}
					}
				}
			}
		}    
    }

    G4int PC = PassArgs->GetLO();
    G4int CT = PassArgs->GetCT();

    if(PassArgs->GetEdep()>0){

            if (GeomConfig == 1 || GeomConfig == 11 ){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Zpos: " << GenZ << G4endl;
    G4cout<< "Energy deposition: " << PassArgs->GetEdep()/MeV << " [MeV] Performed :" << PassArgs->GetNEdep() << " times"<< G4endl;
    G4cout<< "MUON Energy deposition: "  <<PassArgs->GetMuonEdep()/MeV<< " [MeV]"<< G4endl;
    G4cout<< "Photons created end of event: " << PassArgs->GetTP() << G4endl;
	if (!RealSipm) {
    		G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
		G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    		G4cout<< "Estimated Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
	}
    G4cout<< "Real Number of Photons Detected: " << PC << G4endl;
    G4cout<< "Photons detected in the Right SiPM: " << PassArgs->GetNPhotR() << G4endl;
    G4cout<< "Photons detected in the Left SiPM: " << PassArgs->GetNPhotL() << G4endl;
    G4cout<< "Light Output Average (LO/2.) end of event: " << PC/(PassArgs->GetEdep()/MeV)/2. << G4endl;
    G4cout<< "Muon Track Length in LYSO: " << PassArgs->GetMuonLYSOTrackLength() << G4endl;
    G4cout<< "LC / Stopping Power: " << PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength()<< G4endl;

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
	if (!RealSipm){
    	G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
    	G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    	G4cout<< "Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
    	G4cout<< "Estimated Light Output per SiPM " << PassArgs->GetPhHits()*PDE420/(PassArgs->GetEdep()/MeV) << G4endl;
	}
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
        }else if (GeomConfig == 3 || GeomConfig == 13){
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
    //G4cout<< "Event Nº: " << evt << G4endl; Print Run number!!
    G4cout<< "Event Nº: " << evt << G4endl;
    G4cout<< "Primary position command: " << command << G4endl;
    G4cout<< "Energy deposition: " << PassArgs->GetEdep()/MeV << " [MeV] Performed :" << PassArgs->GetNEdep() << " times"<< G4endl;
    G4cout<< "Photons created end of event: " << PassArgs->GetTP() << G4endl;
    if (!RealSipm){
    	G4cout<< "Photon Hits end of event: " << PassArgs->GetPhHits() << G4endl;
    	G4cout<< "Estimated PDE (420nm, 3.5OV): " << PDE420 << G4endl;
    	G4cout<< "Estimated Photon Detected (420nm PDE, 3.5OV) end of event: " << PassArgs->GetPhHits()*PDE420 << G4endl;
    }
    G4cout<< "Real Number of Photons Detected: " << PC << G4endl;
    G4cout<< "Photons detected in the Right SiPM: " << PassArgs->GetNPhotR() << G4endl;
    G4cout<< "Photons detected in the Left SiPM: " << PassArgs->GetNPhotL() << G4endl;
    G4cout<< "Light Output Average (LO/2.) end of event: " << PC/(PassArgs->GetEdep()/MeV)/2. << G4endl;
    G4cout<< "Real Number of Cross-Talk Photons Detected: " << CT << G4endl;
    G4cout<< "Cross-Talk/MeV (nxSiPM) end of event: " << CT/(PassArgs->GetEdep()/MeV) << G4endl;
    G4cout<< "Muon Track Length in LYSO: " << PassArgs->GetMuonLYSOTrackLength() << G4endl;
    G4cout<< "LC / Stopping Power: " << PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength()<< G4endl;

    if(PassArgs->GetTimeTrue()==1){G4cout<< "Global Timing: " << PassArgs->GetPhotTiming() << G4endl;}
    G4cout<< "#####################" << G4endl;
    G4cout<< "#####################" << G4endl;
        }
    }

    // Fill evt values
    if(PassArgs->Getnrep()>0){
    PassArgs->FillEvtLO(evt, PC/(PassArgs->GetEdep()/MeV)/2.);
    PassArgs->FillEvtLD(evt, PC);
    PassArgs->FillEvtLSt(evt, PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength());

    if(PassArgs->GetTimeTrue()==1){PassArgs->FillEvtTim(evt,  PassArgs->GetPhotTiming());}
    }   

if(PassArgs->GetTree_EndOfEvent()==1){
    man->FillNtupleDColumn(4, 0, PassArgs->GetEdep()/MeV);
    man->FillNtupleDColumn(4, 1, PassArgs->GetTP());
    man->FillNtupleDColumn(4, 2, PC);
    //man->FillNtupleDColumn(4, 2, PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength());
    man->FillNtupleDColumn(4, 3, PDE420);
    man->FillNtupleDColumn(4, 4, PC/(PassArgs->GetEdep()/MeV)/2.);
    man->FillNtupleDColumn(4, 5, GenX);
    man->FillNtupleDColumn(4, 6, GenZ);
    man->FillNtupleDColumn(4, 7, GLUEL);//mm
    man->FillNtupleDColumn(4, 8, RESINL);//mm
    man->FillNtupleDColumn(4, 9, XPOS);
    man->FillNtupleDColumn(4, 10, YPOS);
    man->FillNtupleDColumn(4, 11, XPOS2);
    man->FillNtupleDColumn(4, 12, YPOS2);
    //man->FillNtupleDColumn(4, 13, PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength());

    if(RealSipm){    
	// Process digi collections.
		G4DCofThisEvent* dCof = anEvent->GetDCofThisEvent();
		if (dCof != NULL) {
			for (int j = 0; j < dCof->GetCapacity(); ++j) {
				G4VDigiCollection* dc = dCof->GetDC(j);
				if (dc != NULL) {
					G4SipmDigiCollection* sipm_dc = dynamic_cast<G4SipmDigiCollection*>(dc);
					if (sipm_dc) {
						for (size_t i = 0; i < sipm_dc->GetSize(); i++) {
							G4SipmDigi* digi = (G4SipmDigi*) sipm_dc->GetDigi(i);
							if (digi->getTime() >= 0.0 && digi->getTime() < 1000000) {
								man->FillNtupleDColumn(6, 0, digi->getSipmId());
								man->FillNtupleDColumn(6, 1, digi->getCellId());
								man->FillNtupleDColumn(6, 2, digi->getType());
								man->FillNtupleDColumn(6, 3, digi->getTime());
								man->FillNtupleDColumn(6, 4, digi->getWeight());
								man->AddNtupleRow(6);
							}
						}
					}
					G4SipmVoltageTraceDigiCollection* vt = dynamic_cast<G4SipmVoltageTraceDigiCollection*>(dc);
					if (vt) {					  
						for (size_t i = 0; i < vt->GetSize(); i++) {
							G4SipmVoltageTraceDigi* digi = (G4SipmVoltageTraceDigi*) dc->GetDigi(i);
							//TODO: Voltage trace charge integration needs to be implemented.
							//Needs to discuss whether to output charge and time plots or integrated results in numbers.

						}
					}
				}
			}
		}
    }


    if (GeomConfig == 3 || GeomConfig == 13){
    man->FillNtupleDColumn(4, 13, CT);
    man->FillNtupleDColumn(4, 14, CT/(PassArgs->GetEdep()/MeV));}
    
    //else {man->FillNtupleDColumn(4, 11, 0.);}
    man->FillNtupleDColumn(4, 15, PassArgs->GetPhotTiming());
    man->FillNtupleDColumn(4, 16, PassArgs->GetNPhotL());
    man->FillNtupleDColumn(4, 17, PassArgs->GetNPhotR());
    man->FillNtupleDColumn(4, 19, evt);
    man->FillNtupleDColumn(4, 20, PC/(PassArgs->GetEdep()/MeV)/2.*PassArgs->GetMuonLYSOTrackLength());
    /*get ev number from detector!!!*/
    /*Write down particle gun position and angle (x,z,alpha_yz)*/
    man->AddNtupleRow(4);
}

   ///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
 // Run status
    G4UImanager *UImanager = G4UImanager::GetUIpointer();
  G4int eventID=anEvent->GetEventID();
  G4Run* run = static_cast<G4Run*>( G4RunManager::GetRunManager()->GetNonConstCurrentRun() );

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

            G4cout<< "Event GeomConfig: "<< GeomConfig <<" ,PartRnd. "<< PassArgs->GetRnd_Part() << G4endl;
        if (GeomConfig == 1 || GeomConfig == 11){
			
						if (PassArgs->Getrndangle()==1){
			G4double theta = 39.9+G4UniformRand()*50;
            G4cout<< "Angle: " << theta-40 << G4endl;
			theta=theta/180*M_PI;
			G4double X0 = 0;
			G4double Y0 = 50;
			G4double r = G4UniformRand()*0.7;
            G4cout<< "radius: " << r << G4endl;

			G4double ux = -cos(theta);
			G4double uy = -sin(theta);
			G4double x1 = -r*cos(theta);
			G4double y1 = r*sin(theta);
			G4double x10 = r*cos(theta)+X0;
			G4double y10 = r*sin(theta);
			G4double x2 =(Y0-y1)/tan(theta);
			
			GenX=(x2-x10)/1000.;
			GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(Y0/1000)+" "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);  
            command = "/gun/direction "+std::to_string(ux)+" "+std::to_string(uy)+" 0."; 
            G4cout<< command << G4endl; 
            UImanager->ApplyCommand(command);  

			}
			else{
            GenX=(-LYSO_T+LYSO_T*2*G4UniformRand()+PassArgs->GetPartXDispl())/1000.;
            GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command); 
			}    
            //command = "/gun/direction 0. -1. 0."; 
            //G4cout<< command << G4endl;
            //UImanager->ApplyCommand(command); 
        }else if (GeomConfig == 2){
            GenX=(-LYSO_T2+LYSO_T2*2*G4UniformRand())/1000.;
            GenZ=(-LYSO_T+LYSO_T2*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(GenZ)+" -0.05 "+"m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);     
            //command = "/gun/direction 0. 0. 1."; 
            //G4cout<< command << G4endl;
            //UImanager->ApplyCommand(command); 
        }else if (GeomConfig == 3 ){
			if (PassArgs->Getrndangle()==1){
			G4double theta = 39.9+G4UniformRand()*50;
            G4cout<< "Angle: " << theta-40 << G4endl;
			theta=theta/180*M_PI;
			G4double X0 = +1.5+0.1;
			G4double Y0 = 50;
			G4double r = G4UniformRand()*1;
            G4cout<< "radius: " << r << G4endl;

			G4double ux = -cos(theta);
			G4double uy = -sin(theta);
			G4double x1 = -r*cos(theta);
			G4double y1 = r*sin(theta);
			G4double x10 = r*cos(theta)+X0;
			G4double y10 = r*sin(theta);
			G4double x2 =(Y0-y1)/tan(theta);
			
			GenX=(x2-x10)/1000.;
			GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(Y0/1000)+" "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);  
            command = "/gun/direction "+std::to_string(ux)+" "+std::to_string(uy)+" 0."; 
            G4cout<< command << G4endl; 
            UImanager->ApplyCommand(command);  
			}
			else{
            GenX=(-LYSO_T*2.*mm-0.194/2*mm+LYSO_T*mm*2*G4UniformRand()+PassArgs->GetPartXDispl())/1000.;
            GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);     
			}
            //command = "/gun/direction 0. -1. 0."; 
            //G4cout<< command << G4endl;
            //UImanager->ApplyCommand(command); 
        }else if (GeomConfig == 13){
			
			if (PassArgs->Getrndangle()==1){
			G4double theta = 39.9+G4UniformRand()*50;
            G4cout<< "Angle: " << theta-40 << G4endl;
			theta=theta/180*M_PI;
			G4double X0 = +1.5+0.1;
			G4double Y0 = 50;
			G4double r = G4UniformRand()*1;
            G4cout<< "radius: " << r << G4endl;

			G4double ux = -cos(theta);
			G4double uy = -sin(theta);
			G4double x1 = -r*cos(theta);
			G4double y1 = r*sin(theta);
			G4double x10 = r*cos(theta)+X0;
			G4double y10 = r*sin(theta);
			G4double x2 =(Y0-y1)/tan(theta);
			
			GenX=(x2-x10)/1000.;
			GenZ=(-LYSO_L+LYSO_L*2*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" "+std::to_string(Y0/1000)+" "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);  
            command = "/gun/direction "+std::to_string(ux)+" "+std::to_string(uy)+" 0."; 
            G4cout<< command << G4endl; 
            UImanager->ApplyCommand(command);  
			}
			else{			
			LYSO_T=PassArgs->GetGeom_LYSO_thick();
            GenX=(-LYSO_T*1.99*mm-0.10*mm+LYSO_T*mm*1.98*G4UniformRand()+PassArgs->GetPartXDispl())/1000.;
            GenZ=(-LYSO_L*0.99+LYSO_L*1.98*G4UniformRand())/1000.;
            command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);   }  
            //command = "/gun/direction 0. -1. 0."; 
            //G4cout<< command << G4endl;
            //UImanager->ApplyCommand(command); 
        }
    }else if (PassArgs->GetRnd_Part()==2){
        const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
        G4int GeomConfig  = PassArgs->GetGeomConfig();

            G4cout<< "Event GeomConfig: "<< GeomConfig <<" ,PartRnd. "<< PassArgs->GetRnd_Part() << G4endl;
        if (GeomConfig == 1 || GeomConfig == 11){
            G4cout<<"Evt" <<eventID << G4endl; 
            GenX=PassArgs->GetGunX(eventID)/1000;
            GenZ=PassArgs->GetGunY(eventID)/1000;
            command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);    
            command = "/gun/direction 0. -1. 0."; 
            UImanager->ApplyCommand(command);     
            G4cout<< command << G4endl; 
        }else if (GeomConfig == 2){
            GenX=PassArgs->GetGunX(eventID)/1000;
            GenZ=PassArgs->GetGunY(eventID)/1000;
            command = "/gun/position "+std::to_string(GenX)+" 0.05 "+std::to_string(GenZ)+" m"; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command);    
            command = "/gun/direction 0. 0. 1."; 
            G4cout<< command << G4endl;
            UImanager->ApplyCommand(command); 
        }
}


}
