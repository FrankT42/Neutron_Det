#include "RunAction.hh"
#include "HistoManager.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"

bool saved = false;

//this deals with the general running and closing of the
//simulation, if you need anything loaded at runtime
//here is where it goes

//==================================

RunAction::RunAction(HistoManager* histo)
  :G4UserRunAction(),
   fHistoManager(histo),fSumEdep(0.),
   Edep(0.)
{}
//===================================

RunAction::~RunAction()
{ if(!saved){
  fHistoManager->Save();
  }
}
//===================================
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
  G4cout<<"### Run "<<aRun->GetRunID()<<" start"<<G4endl;
  fSumEdep = 0.;

  fHistoManager->Book();
}
//====================================
void RunAction::FillPerEvent(G4double Edep)
{
  fSumEdep += Edep;
}

//=====================================
void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if(NbOfEvents == 0) {return;}
  fSumEdep/=NbOfEvents;
  G4cout<<"\n-------------------End of Run-----------------------------\n"
	<<"\n mean Energy Deposited: "<<G4BestUnit(fSumEdep,"Energy")
	<<"\n----------------------------------------------------------\n"
	<<G4endl;

  fHistoManager->Save();
  saved = true;
  //thought it would be nice to tell the mean energies per thread per run
  //might be useless but I like it, other things can go here but who cares
}
