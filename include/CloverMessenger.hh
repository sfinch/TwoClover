//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef CloverMessenger_h
#define CloverMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class Clover;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class CloverMessenger: public G4UImessenger
{
  public:
    CloverMessenger(Clover* );
   ~CloverMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    Clover* Detector;
    
    G4UIdirectory*             CloverDir;
	//materials
    G4UIcmdWithAString*        CrystalMaterCmd;
    G4UIcmdWithAString*        WindowMaterCmd;
    G4UIcmdWithAString*        WallMaterCmd;
	//Crystal Dimensions
    G4UIcmdWithADoubleAndUnit* CrystalHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* CrystalRadCmd;
    G4UIcmdWithADoubleAndUnit* CrystalEndRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleRadCmd;
    G4UIcmdWithADoubleAndUnit* HoleDepthCmd;
    G4UIcmdWithADoubleAndUnit* DeadLayerCmd;
	//Shell dimensions
    G4UIcmdWithADoubleAndUnit* ShellHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* EndGapCmd;
    G4UIcmdWithADoubleAndUnit* WindowThickCmd;
    G4UIcmdWithADoubleAndUnit* WallThickCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

