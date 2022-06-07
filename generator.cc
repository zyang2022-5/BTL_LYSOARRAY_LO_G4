#include "generator.hh"

MyPrimaryGenerator :: MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); /*Number of particles*/

    // Add to constructor what we want to use in macro files or modify per event or it will be overwritten 
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    //G4String particleName ="mu+"; (2-10GeV)                      
    G4String particleName ="gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
    
    /*const MyDetectorConstruction *detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    G4int GeomConfig  = detectorConstruction->GetGC();

    G4double LYSO_L  = detectorConstruction->GetLYSOL();
    LYSO_L=LYSO_L-LYSO_L*0.01;
    G4double LYSO_T  = detectorConstruction->GetLYSOT();
    LYSO_T=LYSO_T-LYSO_T*0.01;*/

    //G4cout<< "Primary generator GeomConfig: " << GeomConfig<< G4endl;

        //G4double GenX=-LYSO_T*mm+LYSO_T*2*mm*G4UniformRand();
        //G4double GenZ=-LYSO_L*mm+LYSO_L*2*mm*G4UniformRand();
        G4ThreeVector pos(0.*m,0.1*m,0.*m);
        G4ThreeVector mom(0.,-1.,0.);
        fParticleGun->SetParticlePosition (pos);
        fParticleGun->SetParticleMomentumDirection (mom);
        fParticleGun->SetParticleMomentum (511. *keV);
        fParticleGun->SetParticleDefinition(particle);

    /*if(GeomConfig == 1){
        G4double GenX=-LYSO_T*mm+LYSO_T*2*mm*G4UniformRand();
        G4double GenZ=-LYSO_L*mm+LYSO_L*2*mm*G4UniformRand();
        G4ThreeVector pos(GenX*m,0.1*m,GenZ*m);
        G4ThreeVector mom(0.,-1.,0.);
        fParticleGun->SetParticlePosition (pos);
        fParticleGun->SetParticleMomentumDirection (mom);
        fParticleGun->SetParticleMomentum (511. *keV);
        fParticleGun->SetParticleDefinition(particle);
    }else if (GeomConfig == 2){
        G4double GenX=-LYSO_T*mm+LYSO_T*2*mm*G4UniformRand();
        G4double GenZ=-LYSO_T*mm+LYSO_T*2*mm*G4UniformRand();
        G4ThreeVector pos(GenX*m,GenZ*m,0.1*m);
        G4ThreeVector mom(0.,0.,-1.);
        fParticleGun->SetParticlePosition (pos);
        fParticleGun->SetParticleMomentumDirection (mom);
        fParticleGun->SetParticleMomentum (511. *keV);
        fParticleGun->SetParticleDefinition(particle);
    }*/
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{

    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition *particle= fParticleGun->GetParticleDefinition();

    if (particle == G4Geantino::Geantino())
    {
        // Cobalt particle
        G4int Z=27;
        G4int A=27;
        G4double charge = 0.*eplus;
        G4double energy = 0.*keV;

        G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);

        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);

    }
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
