#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"
#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"
#include "tracking.hh"
#include "G4Args.hh"

class MyActionInitialization : public G4VUserActionInitialization
{

public:
    MyActionInitialization(MyG4Args*);
    ~MyActionInitialization();
    
    virtual void Build() const; /*Run particle gun and computes stepping etc...*/

private:
    G4String OutputName;
    MyG4Args* PassArgs;
};

#endif
