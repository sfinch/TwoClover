//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/RunActionMessenger.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunActionMessenger.hh"

#include "RunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::RunActionMessenger(RunAction* RunAct)
:runAction(RunAct)
{
  // directory
  runDir = new G4UIdirectory("/TwoClover/run/");
  runDir->SetGuidance("run control");
   
  // command to change the file name
  fileNameCmd= new G4UIcmdWithAString("/TwoClover/run/fileName",this);
  fileNameCmd->SetGuidance("Give a filename to save to.");
  fileNameCmd->SetParameterName("choice",true);
  fileNameCmd->SetDefaultValue("simulated");
  fileNameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunActionMessenger::~RunActionMessenger()
{
  delete fileNameCmd;
  delete runDir; 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  if(command == fileNameCmd){
    runAction->SetFileName(newValue);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
