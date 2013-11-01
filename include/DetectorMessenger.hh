//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DetectorMessenger.hh
// makes all aspects of the detector: HPGes, NaI, samples, etc
// uses Clover.hh, NaIAnnulus.hh, and DySample.hh classes
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorMessenger_h
#define DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorMessenger: public G4UImessenger
{
  public:
    DetectorMessenger(DetectorConstruction* );
    ~DetectorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    DetectorConstruction* Detector;
    
    G4UIdirectory*             TwoCloverDir;
    G4UIdirectory*             detDir;
    //materials
    G4UIcmdWithADoubleAndUnit* DetectorDistanceCmd;
    //update
    G4UIcmdWithoutParameter*   UpdateCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

