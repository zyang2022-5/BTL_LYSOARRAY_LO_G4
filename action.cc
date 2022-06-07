#include "action.hh"

MyActionInitialization:: MyActionInitialization(G4String OutName)
{
// Saving output name
OutputName=OutName;
}


MyActionInitialization:: ~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
    // Particle gun generator
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    // Actions performed at begining and end of a run
    MyRunAction *runAction = new MyRunAction(OutputName);
    SetUserAction(runAction);

    // Actions at the begining and end of each event in a run
    MyEventAction *eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);

    // Actions performed at each step for each particle track
    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);

    // Actions performed at each track begining and end
    MyTrackingAction *trackingAction = new MyTrackingAction(eventAction);
    SetUserAction(trackingAction);
}
