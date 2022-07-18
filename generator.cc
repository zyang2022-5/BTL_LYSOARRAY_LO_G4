#include "generator.hh"

MyPrimaryGenerator :: MyPrimaryGenerator()
{
    fParticleGun = new G4ParticleGun(1); /*Number of particles*/

    // Add to constructor what we want to use in macro files or modify per event or it will be overwritten 
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName ="gamma";
    G4ParticleDefinition *particle = particleTable->FindParticle(particleName);
    

        G4ThreeVector pos(0.*m,0.1*m,0.*m);
        G4ThreeVector mom(0.,-1.,0.);
        fParticleGun->SetParticlePosition (pos);
        fParticleGun->SetParticleMomentumDirection (mom);
        fParticleGun->SetParticleMomentum (511. *keV);
        fParticleGun->SetParticleDefinition(particle);

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
