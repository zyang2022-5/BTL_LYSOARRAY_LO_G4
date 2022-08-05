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
#include <stdio.h>
#include <vector>

class MySensitiveDetector : public G4VSensitiveDetector
{
	
public:
    MySensitiveDetector(G4String,G4double);
    ~MySensitiveDetector();
    G4int GetDetCount() const {return countdet;}
    G4double Get_shortest_timeG() const {return shortest_timeG;}
    G4int Get_shortest_track() const {printf("returning shortest_track_id = %i\n", shortest_track_id); return shortest_track_id;}
    std::vector<G4int> Get_id_vec() const {return id_vec;}
private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    G4PhysicsOrderedFreeVector *PDE;    
    G4int countdet, shortest_track_id;
    G4double Vov, shortest_timeG;
    std::vector<G4int> id_vec;   
    //MyEventAction *fEventAction;

};

#endif
