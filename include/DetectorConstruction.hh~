
#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"


class G4VPhysicalVolume;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();
  inline const G4VPhysicalVolume* GetScint(){return fScint;}
  inline const G4VPhysicalVolume* GetSiPM(){return fSiPM;}
  inline const G4VPhysicalVolume* GetphysWorld(){return expHall_phys;}
  inline const G4VPhysicalVolume* GetWrap(){return Guide_Wrap_phys;}
  public:
    virtual G4VPhysicalVolume* Construct();
  

  private:
    G4double fExpHall_x;
    G4double fExpHall_y;
    G4double fExpHall_z;

    G4double fGuide_x;
    G4double fGuide_y;
    G4double fGuide_z;

    G4double fScint_x;
    G4double fScint_y;
    G4double fScint_z;
  G4VPhysicalVolume* Guide_Wrap_phys;
  G4VPhysicalVolume* fScint;
  G4VPhysicalVolume* fSiPM;
  G4VPhysicalVolume* expHall_phys;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif /*DetectorConstruction_h*/
