#include "action.hh"

MyActionInitialization:: MyActionInitialization(G4String OutName)
{
OutputName=OutName;
}


MyActionInitialization:: ~MyActionInitialization()
{}

void MyActionInitialization::Build() const
{

    MyPrimaryGenerator *generator = new MyPrimaryGenerator();
    SetUserAction(generator);

    MyRunAction *runAction = new MyRunAction(OutputName);
    SetUserAction(runAction);

    MyEventAction *eventAction = new MyEventAction(runAction);
    SetUserAction(eventAction);

    MySteppingAction *steppingAction = new MySteppingAction(eventAction);
    SetUserAction(steppingAction);

    MyTrackingAction *trackingAction = new MyTrackingAction(eventAction);
    SetUserAction(trackingAction);
}
