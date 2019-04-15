#include "EventAction.hh"

#include "RunAction.hh"
#include "HistoManager.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4Event.hh"

EventAction::EventAction(RunAction* run, HistoManager* histo)
  :G4UserEventAction(),
   fRunAction(run), fHistoManager(histo),
   fPrintModulo(0), fSiPM(0)
{
  fPrintModulo = 100;
}
//=============================================

EventAction::~EventAction()
{
}
//===============================================
void EventAction::SiPMTrack(G4double global_time,G4double delta_time, G4ThreeVector location)
{
  t_g.push_back(global_time);
  t_d.push_back(delta_time);
  x.push_back(location.x());
  y.push_back(location.y());
}

//============================================
void EventAction::BeginOfEventAction(const G4Event* evt)
{
  G4int evtNb = evt->GetEventID();
  if(evtNb%fPrintModulo == 0)
    {
      G4cout<<"\n---->Start of Event: "<<evtNb<<G4endl;
    }
  fEdep = 0.;//reset at start of event
  fScint = 0;
  fSiPM = 0;
  t_g.clear();
  t_d.clear();
  x.clear();
  y.clear();
}
//=============================================

void EventAction::EndOfEventAction(const G4Event*)
{
  //get the stats
  fRunAction->FillPerEvent(fEdep);
  //Gotta do the histograms or who will
  fHistoManager->FillHisto(0,fEdep);
  fHistoManager->FillHisto(1,fScint);
  fHistoManager->FillHisto(2,fSiPM);

  //Do the Ntuples
  //
  fHistoManager->FillNtuple(fEdep,fScint,fSiPM);
  fHistoManager->FillTimeAndLoc(x,y,t_g,t_d);

  
}
//==========================================
