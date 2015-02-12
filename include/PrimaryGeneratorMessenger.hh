//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/PrimaryGeneratorMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorMessenger_h
#define PrimaryGeneratorMessenger_h 1

#include <vector>
#include "G4UImessenger.hh"
#include "globals.hh"

class PrimaryGeneratorAction;

class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorMessenger: public G4UImessenger
{
  public:
    PrimaryGeneratorMessenger(PrimaryGeneratorAction*);
    virtual ~PrimaryGeneratorMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    PrimaryGeneratorAction* Action;
    G4UIdirectory*          gunDir; 
    G4UIcmdWithAString*     RndmCmd;

    G4UIcmdWithADoubleAndUnit* PositionXCmd;
    G4UIcmdWithADoubleAndUnit* PositionYCmd;
    G4UIcmdWithADoubleAndUnit* PositionZCmd;
    G4UIcmdWithAnInteger*      numGammaCmd;

    G4UIcmdWithAnInteger*      spinCmd[4];
    G4UIcmdWithADoubleAndUnit* energyCmd[4];

    G4UIcmdWithoutParameter*   PrintCmd;
  
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

