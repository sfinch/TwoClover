//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NaIAnnulusMessenger.hh"

#include "NaIAnnulus.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NaIAnnulusMessenger::NaIAnnulusMessenger(NaIAnnulus* Det)
{ 
  Detector = Det;

  G4String dir = "/TwoClover/det/"+Detector->GetName()+"/";
  
  NaIAnnulusDir = new G4UIdirectory(dir);
  G4String guid = Detector->GetName()+" detector commands";
  NaIAnnulusDir->SetGuidance(guid.c_str());

  G4String crap;
  //Materials
  crap = dir+"setCrystalMat";
  CrystalMaterCmd = new G4UIcmdWithAString(crap.c_str(),this);
  CrystalMaterCmd->SetGuidance("Select Material of the Crystal.");
  CrystalMaterCmd->SetParameterName("choice",false);
  CrystalMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setWallMat";
  WallMaterCmd = new G4UIcmdWithAString(crap.c_str(),this);
  WallMaterCmd->SetGuidance("Select Material of the wall.");
  WallMaterCmd->SetParameterName("choice",false);
  WallMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //Crystal dimensions 
  crap = dir+"setCrystalHalfLength";
  CrystalHalfLengthCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalHalfLengthCmd->SetGuidance("Set the half length of the crystal.");
  CrystalHalfLengthCmd->SetParameterName("Size",false);
  CrystalHalfLengthCmd->SetRange("Size>0.");
  CrystalHalfLengthCmd->SetUnitCategory("Length");    
  CrystalHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setCrystalInRad";
  CrystalInRadCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalInRadCmd->SetGuidance("Set the inner radius of the crystal.");
  CrystalInRadCmd->SetParameterName("Size",false);
  CrystalInRadCmd->SetRange("Size>0.");
  CrystalInRadCmd->SetUnitCategory("Length");    
  CrystalInRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setCrystalOutRad";
  CrystalOutRadCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalOutRadCmd->SetGuidance("Set the outer radius of the crystal.");
  CrystalOutRadCmd->SetParameterName("Size",false);
  CrystalOutRadCmd->SetRange("Size>0.");
  CrystalOutRadCmd->SetUnitCategory("Length");    
  CrystalOutRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //Wall dimensions
  crap = dir+"setWallInThick";
  WallInThickCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  WallInThickCmd->SetGuidance("Set thickness of the inner side wall");
  WallInThickCmd->SetParameterName("Size",false);
  WallInThickCmd->SetRange("Size>=0.");
  WallInThickCmd->SetUnitCategory("Length");  
  WallInThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setWallOutThick";
  WallOutThickCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  WallOutThickCmd->SetGuidance("Set thickness of the outer side wall");
  WallOutThickCmd->SetParameterName("Size",false);
  WallOutThickCmd->SetRange("Size>=0.");
  WallOutThickCmd->SetUnitCategory("Length");  
  WallOutThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NaIAnnulusMessenger::~NaIAnnulusMessenger()
{
  delete CrystalMaterCmd; delete WallMaterCmd;
  delete CrystalHalfLengthCmd; delete CrystalInRadCmd; delete CrystalOutRadCmd;
  delete WallInThickCmd; delete WallOutThickCmd;

  delete NaIAnnulusDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulusMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //materials
  if( command == CrystalMaterCmd )
   { Detector->SetCrystalMaterial(newValue);}
   
  if( command == WallMaterCmd )
   { Detector->SetWallMaterial(newValue);}
  
  //Crystal dimensions
  if( command == CrystalHalfLengthCmd )
   { Detector->SetCrystalHalfLength(CrystalHalfLengthCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalInRadCmd )
   { Detector->SetCrystalInRad(CrystalInRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalOutRadCmd )
   { Detector->SetCrystalOutRad(CrystalOutRadCmd->GetNewDoubleValue(newValue));}
   
   //Shell Dimensions
  if( command == WallInThickCmd )
   { Detector->SetWallInThickness(WallInThickCmd->GetNewDoubleValue(newValue));}
   
  if( command == WallOutThickCmd )
   { Detector->SetWallOutThickness(WallOutThickCmd->GetNewDoubleValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
