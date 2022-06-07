#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicsOrderedFreeVector.hh"
#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "event.hh"
//#include "event.hh"
class MySensitiveDetector : public G4VSensitiveDetector
{

public:
    MySensitiveDetector(G4String,G4double);
    ~MySensitiveDetector();
    G4int GetDetCount() const {return countdet;}

private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    G4PhysicsOrderedFreeVector *PDE;    
    G4int countdet;
    G4double Vov;
    
    //MyEventAction *fEventAction;

};

#endif
