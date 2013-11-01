//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/RunActionMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunActionMessenger_h
#define RunActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class RunAction;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunActionMessenger: public G4UImessenger
{
  public:
    RunActionMessenger(RunAction*);
    virtual ~RunActionMessenger();
    
    void SetNewValue(G4UIcommand*, G4String);
    
  private:
    RunAction*          runAction;
    G4UIdirectory*        runDir;   
    G4UIcmdWithAString*   fileNameCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
