#include "generator.hh"

MyPrimaryGenerator :: MyPrimaryGenerator(MyG4Args* MainArgs)
{
    PassArgs=MainArgs;
    fParticleGun = new G4ParticleGun(1); /*Number of particles*/

    // Add to constructor what we want to use in macro files or modify per event or it will be overwritten 
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();

    if(PassArgs->GetMuonFlag()==1){
            G4String particleName ="mu+";
            G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
            
                G4ThreeVector mom(PassArgs->GetPartDir(0),PassArgs->GetPartDir(1),PassArgs->GetPartDir(2));

                fParticleGun->SetParticleMomentumDirection (mom);
                fParticleGun->SetParticleMomentum (2. *GeV);
                fParticleGun->SetParticleDefinition(particle); 

        }else{
            G4String particleName ="gamma";
            G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
            
                G4ThreeVector mom(PassArgs->GetPartDir(0),PassArgs->GetPartDir(1),PassArgs->GetPartDir(2));
                fParticleGun->SetParticleMomentumDirection (mom);
                fParticleGun->SetParticleMomentum (511. *keV);
                fParticleGun->SetParticleDefinition(particle); 
        }

            G4double LT=PassArgs->GetGeom_LYSO_thick();
                if(PassArgs->GetGeomConfig()==3 || PassArgs->GetGeomConfig()==13 ){
                    G4ThreeVector pos(-1*LT*mm,0.05*m,0.*m);
                    fParticleGun->SetParticlePosition (pos);
                }else{
                    G4ThreeVector pos(0.*m,0.05*m,0.*m);
                    fParticleGun->SetParticlePosition (pos);
                }

}

MyPrimaryGenerator::~MyPrimaryGenerator()
{

    delete fParticleGun;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
    G4ParticleDefinition *particle= fParticleGun->GetParticleDefinition();

    // Example for particle definition (not used in this executable)
    /*if (particle == G4Geantino::Geantino())
    {
        // Cobalt particle
        G4int Z=27;
        G4int A=27;
        G4double charge = 0.*eplus;
        G4double energy = 0.*keV;

        G4ParticleDefinition *ion = G4IonTable::GetIonTable()->GetIon(Z,A,energy);

        fParticleGun->SetParticleDefinition(ion);
        fParticleGun->SetParticleCharge(charge);

    }*/
    fParticleGun->GeneratePrimaryVertex(anEvent);
}
