//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorMessenger.hh"

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::PrimaryGeneratorMessenger(PrimaryGeneratorAction* Gun)
:Action(Gun)
{
  gunDir = new G4UIdirectory("/TwoClover/gun/");
  gunDir->SetGuidance("PrimaryGenerator control");
   
  RndmCmd = new G4UIcmdWithAString("/TwoClover/gun/rndm",this);
  RndmCmd->SetGuidance("Shoot randomly the incident particle.");
  RndmCmd->SetGuidance(" Choice : on(default), off");
  RndmCmd->SetParameterName("choice",true);
  RndmCmd->SetDefaultValue("on");
  RndmCmd->SetCandidates("on off");
  RndmCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  numGammaCmd = new G4UIcmdWithAnInteger("/TwoClover/gun/numGamma",this);
  numGammaCmd->SetGuidance("Set the number of Gammas shot");
  numGammaCmd->SetParameterName("Number",false);
  numGammaCmd->SetRange("Number>0&&Number<5");
  
  PositionRCmd = new G4UIcmdWithADoubleAndUnit("/TwoClover/gun/positionR",this);
  PositionRCmd->SetGuidance("Set the position of the source along R");
  PositionRCmd->SetParameterName("Size",false);
  PositionRCmd->SetUnitCategory("Length");
  PositionRCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  for (int i=0; i<4;i++){
    G4String dir = "/TwoClover/gun/energy"+G4UIcommand::ConvertToString(i+1);
    G4String guid = "Set energy of gamma "+G4UIcommand::ConvertToString(i+1);

    energyCmd[i] = new G4UIcmdWithADoubleAndUnit(dir.c_str(),this);
    energyCmd[i]->SetGuidance(guid.c_str());
    energyCmd[i]->SetParameterName("Size",false);
    energyCmd[i]->SetRange("Size>0.");
    energyCmd[i]->SetUnitCategory("Energy");
    energyCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorMessenger::~PrimaryGeneratorMessenger()
{
  delete RndmCmd;
  delete gunDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == RndmCmd )
    { Action->SetRndmFlag(newValue);}
  if( command == numGammaCmd)
    { Action->SetNumGamma(numGammaCmd->GetNewIntValue(newValue));}
  if( command == PositionRCmd)
    { Action->SetPositionR(PositionRCmd->GetNewDoubleValue(newValue));}
  for (int i=0; i<4;i++){
    if( command == energyCmd[i])
      { Action->SetEnergy(i, energyCmd[i]->GetNewDoubleValue(newValue));}
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


