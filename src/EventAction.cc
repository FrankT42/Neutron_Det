#include "EventAction.hh"

#include "RunAction.hh"
#include "HistoManager.hh"
#include <vector>
#include "G4ThreeVector.hh"
#include "G4Event.hh"

//This is effectivly a control mechanism between stepping action
//and the HistoManager, it also controls what is printed
//to the screen in batch mode ect.
//each thread will print what is here
//ie G4WT5 > 
//---->Start of Event: 100
//ect for thread 5(6th thread, they start at 0)


EventAction::EventAction(RunAction* run, HistoManager* histo)
  :G4UserEventAction(),
   fRunAction(run), fHistoManager(histo),
   fPrintModulo(0), fSiPM(0)
{
  fPrintModulo = 100;//this is the number of events between prints to screen 100
                     //seems reasonable to me
}
//=============================================

EventAction::~EventAction()
{
}
//===============================================
void EventAction::SiPMTrack(G4double global_time,G4double delta_time, G4ThreeVector location)
{//time and track info from stepping action
  t_g.push_back(global_time);
  t_d.push_back(delta_time);
  x.push_back(location.x());
  y.push_back(location.y());
}

//============================================
void EventAction::BeginOfEventAction(const G4Event* evt)
{
  //print when a new event which is also a multiple of the
  //PrintModulo
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
  //this is where the fill by event stuff
  //is cleared to reset, so if anything is
  //added clear it here or face the conciquences
  //which is buggered action
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
