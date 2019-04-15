#include "ActionInitialization.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "DetectorConstruction.hh"

//======================================================

ActionInitialization::ActionInitialization(DetectorConstruction* detector)
  :G4VUserActionInitialization(),
fDetector(detector)
{}

//=======================================================
ActionInitialization::~ActionInitialization()
{}
//=======================================================
void ActionInitialization::BuildForMaster() const
{
  HistoManager* histo = new HistoManager();
  SetUserAction(new RunAction(histo));
}

//======================================================
void ActionInitialization::Build() const
{
  HistoManager* histo = new HistoManager();

  SetUserAction(new PrimaryGeneratorAction(fDetector));
  RunAction* runAction = new RunAction(histo);
  SetUserAction(runAction);

  EventAction* eventAction = new EventAction(runAction, histo);
  SetUserAction(eventAction);

  SteppingAction* steppingAction = new SteppingAction(fDetector, eventAction);

  SetUserAction(steppingAction);
}
//======================================================
//              End of ActionInitialization
//======================================================
