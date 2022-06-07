#include "physics.hh"

MyPhysicsList :: MyPhysicsList()
{
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());

    G4StepLimiterPhysics* stepLimitPhys = new G4StepLimiterPhysics();
    stepLimitPhys->SetApplyToAll(true); // activates step limit for ALL particles
    RegisterPhysics(stepLimitPhys);
}

MyPhysicsList::~MyPhysicsList()
{}
