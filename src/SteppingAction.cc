#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4RunManager.hh"
#include <fstream>
#include "ActionInitialization.hh"
#include <sstream>

#include "G4Step.hh"

//======================================================
SteppingAction::SteppingAction(DetectorConstruction* det,
			       EventAction* evt)
  :G4UserSteppingAction(),
   fDetector(det), fEventAction(evt)
{
  Tout.open("./Outputs/Labels.dat");
  //this is for making training data, not all that useful
  //if you don't do machine learning, so not fully setup
  //as it makes big files
}
//=====================================================
SteppingAction::~SteppingAction()
{}
//=====================================================
void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  std::stringstream ss;
  //std::ofstream Tout("./Outputs/Types.dat");
  G4Track* track = aStep->GetTrack();
  G4String ParticleName = track->GetDynamicParticle()->
                                 GetParticleDefinition()->GetParticleName();

  //gets the particle name
  if(ParticleName=="neutron"||ParticleName=="gamma"){
    ss<<ParticleName<<std::endl;
    Tout<<ss.str();
  }
  G4VPhysicalVolume* volume
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  //gets the volume
 G4double GlobalTime = track->GetGlobalTime();
 G4double DeltaTime = aStep->GetDeltaTime();
 G4ThreeVector location = track->GetPosition();
 //get time and location

//collect energy ect step by step
 if(volume->GetLogicalVolume() ==fDetector->GetSiPMLog()){
    if(ParticleName == "opticalphoton"){
      fEventAction->AddSiPM(1);
      fEventAction->SiPMTrack(GlobalTime, DeltaTime, location);
      track->SetTrackStatus(fStopAndKill);
    }
    else{
      return;
    }
  }
 //gets the optical photons and sends them to event action to be counted
 //the track info is also sent to be parased and processed 
 //the photon is then killed, effectivlly making a 100% efficient
 //detector, this is due to the fact I havent implimented the
 //absorption for the SiPM since the material change as I am
 //not doing multi-layerd SiPM anymore, this will be remidied in
 //a future build, either by post process or by using the Absorption
 //mechanism, not sure but it's good enough for now
 
  if( ParticleName=="opticalphoton"){return;}
 G4double edep = aStep->GetTotalEnergyDeposit();
 //Get the energy deposited in step
 if(volume == fDetector->GetScint()) fEventAction->AddEdep(edep);
 //if the step is in the scintillator add it
 // this resets every event so don't worry about that
 // a new event causes event action to bin the edep
 // and start a new counter, this just informs eventAction
 //of the information in this step. This allows more flexability
 //in dealing with information


 const std::vector<const G4Track*>* secondaries =
                                            aStep->GetSecondaryInCurrentStep();
 //cheack secondaries, we can then check if scint cherek ext
 if (secondaries->size()>0) {
   for(unsigned int i=0; i<secondaries->size(); ++i) {
        if (secondaries->at(i)->GetParentID()>0) {
           if(secondaries->at(i)->GetDynamicParticle()->GetParticleDefinition()
               == G4OpticalPhoton::OpticalPhotonDefinition()){
              if (secondaries->at(i)->GetCreatorProcess()->GetProcessName()
		  == "Scintillation")fEventAction->AddScint(1);
	      //Can just add more if statements if other mechanisms are
	      //needed, ie. Process name is --whatever-- nad this will work
	      //but you would have to add something to deal with it in
	      //the evnet action and HitoManager
	      //not the use of pre-increment here, since this is run every
	      //step, any speed up is welcome, and more nessasary than
	      //anywhere else as i++ would make a copy then increment
	      //returning the copied value, causing the storage of 2
	      //variables, aslo unsigned speeds up a little
	      //probably beyond the scope of most implementations
	   }
	}
   
 }
 }
 
 }
