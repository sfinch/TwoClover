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
    G4UIcmdWithADoubleAndUnit* CrystalHalfLengthCmd[4];
    G4UIcmdWithADoubleAndUnit* CrystalRadCmd[4];
    G4UIcmdWithADoubleAndUnit* CrystalEndRadCmd[4];
    G4UIcmdWithADoubleAndUnit* HoleRadCmd[4];
    G4UIcmdWithADoubleAndUnit* HoleDepthCmd[4];
    G4UIcmdWithADoubleAndUnit* DeadLayerCmd[4];
	//Shell dimensions
    G4UIcmdWithADoubleAndUnit* ShellHalfLengthCmd;
    G4UIcmdWithADoubleAndUnit* EndGapCmd;
    G4UIcmdWithADoubleAndUnit* WindowThickCmd;
    G4UIcmdWithADoubleAndUnit* WallThickCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

