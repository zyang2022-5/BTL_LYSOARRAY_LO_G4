#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"
#include "tracking.hh"

class MyActionInitialization : public G4VUserActionInitialization
{

public:
    MyActionInitialization(G4String OutName);
    ~MyActionInitialization();
    
    virtual void Build() const; /*Run particle gun and computes stepping etc...*/

private:
    G4String OutputName;
};

#endif
