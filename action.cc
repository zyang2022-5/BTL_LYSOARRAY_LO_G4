#include "action.hh"

MyActionInitialization:: MyActionInitialization(MyG4Args *MainArgs)
{
// Saving output name
PassArgs=MainArgs;
OutputName=MainArgs->GetOutName();
}


MyActionInitialization:: ~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{
    // Particle gun generator
    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    // Actions performed at begining and end of a run
    MyRunAction *runAction = new MyRunAction(OutputName,PassArgs);
    SetUserAction(runAction);

    // Actions at the begining and end of each event in a runs
    MyEventAction *eventAction = new MyEventAction(runAction,PassArgs);
    SetUserAction(eventAction);

    // Actions performed at each step for each particle track
    MySteppingAction *steppingAction = new MySteppingAction(eventAction,PassArgs);
    SetUserAction(steppingAction);

    // Actions performed at each track begining and end
    MyTrackingAction *trackingAction = new MyTrackingAction(eventAction,PassArgs);
    SetUserAction(trackingAction);
}
