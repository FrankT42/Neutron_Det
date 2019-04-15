#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class DetectorConstruction;
class G4GeneralParticleSource;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  PrimaryGeneratorAction(DetectorConstruction*);
  virtual ~PrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  void SetOptPhotonPolar();
  void SetOptPhotonPolar(G4double);
  

private:
  G4ParticleGun*  fParticleGun;
  DetectorConstruction* fDetector;
  G4GeneralParticleSource* fGPS;
};
#endif
