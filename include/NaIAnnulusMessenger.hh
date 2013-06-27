//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef NaIAnnulusMessenger_h
#define NaIAnnulusMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class NaIAnnulus;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NaIAnnulusMessenger: public G4UImessenger
{
  public:
    NaIAnnulusMessenger(NaIAnnulus* );
   ~NaIAnnulusMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    NaIAnnulus* Detector;
    
    G4UIdirectory*             NaIAnnulusDir;
	//materials
    G4UIcmdWithAString*        CrystalMaterCmd;
    G4UIcmdWithAString*        WallMaterCmd;
	//Crystal Dimensions
    G4UIcmdWithADoubleAndUnit* CrystalHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* CrystalInRadCmd;
    G4UIcmdWithADoubleAndUnit* CrystalOutRadCmd;
	//Shell dimensions
    G4UIcmdWithADoubleAndUnit* WallInThickCmd;
    G4UIcmdWithADoubleAndUnit* WallOutThickCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

