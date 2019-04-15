#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include <fstream>

class DetectorConstruction;
class EventAction;

class SteppingAction : public G4UserSteppingAction
{
public:
  SteppingAction(DetectorConstruction*, EventAction*);
  virtual ~SteppingAction();

  virtual void UserSteppingAction(const G4Step*);
  static std::ofstream& GetStream(){static std::ofstream fout("./Outputs/Pulses.dat");
    return fout;}
private:
  DetectorConstruction* fDetector;
  EventAction* fEventAction;
  std::ofstream Tout;
} ;
#endif
