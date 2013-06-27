//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "CloverMessenger.hh"

#include "Clover.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverMessenger::CloverMessenger(Clover* Det)
{ 
  Detector = Det;

  G4String dir = "/TwoClover/det/"+Detector->GetName()+"/";
  
  CloverDir = new G4UIdirectory(dir);
  G4String guid = Detector->GetName()+" detector commands";
  CloverDir->SetGuidance(guid.c_str());

  G4String crap;
  //materials
  crap = dir+"setCrystalMat";
  CrystalMaterCmd = new G4UIcmdWithAString(crap.c_str(),this);
  CrystalMaterCmd->SetGuidance("Select Material of the Crystal.");
  CrystalMaterCmd->SetParameterName("choice",false);
  CrystalMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setWindowMat";
  WindowMaterCmd = new G4UIcmdWithAString(crap.c_str(),this);
  WindowMaterCmd->SetGuidance("Select Material of the Window.");
  WindowMaterCmd->SetParameterName("choice",false);
  WindowMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setWallMat";
  WallMaterCmd = new G4UIcmdWithAString(crap.c_str(),this);
  WallMaterCmd->SetGuidance("Select Material of the side wall.");
  WallMaterCmd->SetParameterName("choice",false);
  WallMaterCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  //crystal dimensions 
  crap = dir+"setCrystalHalfLength";
  CrystalHalfLengthCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalHalfLengthCmd->SetGuidance("Set the half length of the crystal.");
  CrystalHalfLengthCmd->SetParameterName("Size",false);
  CrystalHalfLengthCmd->SetRange("Size>0.");
  CrystalHalfLengthCmd->SetUnitCategory("Length");    
  CrystalHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setCrystalRad";
  CrystalRadCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalRadCmd->SetGuidance("Set the radius of the crystal.");
  CrystalRadCmd->SetParameterName("Size",false);
  CrystalRadCmd->SetRange("Size>0.");
  CrystalRadCmd->SetUnitCategory("Length");    
  CrystalRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setCrystalEndRad";
  CrystalEndRadCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  CrystalEndRadCmd->SetGuidance("Set the radius of rounding on the crystal's edge.");
  CrystalEndRadCmd->SetParameterName("Size",false);
  CrystalEndRadCmd->SetRange("Size>0.");
  CrystalEndRadCmd->SetUnitCategory("Length");    
  CrystalEndRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  crap = dir+"setHoleRad";
  HoleRadCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  HoleRadCmd->SetGuidance("Set the end radius of the hole in the crystal.");
  HoleRadCmd->SetParameterName("Size",false);
  HoleRadCmd->SetRange("Size>0.");
  HoleRadCmd->SetUnitCategory("Length");    
  HoleRadCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setHoleDepth";
  HoleDepthCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  HoleDepthCmd->SetGuidance("Set the depth of the hole in the crystal.");
  HoleDepthCmd->SetParameterName("Size",false);
  HoleDepthCmd->SetRange("Size>0.");
  HoleDepthCmd->SetUnitCategory("Length");    
  HoleDepthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setDeadLayer";
  DeadLayerCmd= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  DeadLayerCmd->SetGuidance("Set the thickness of the crystal's  dead layer.");
  DeadLayerCmd->SetParameterName("Size",false);
  DeadLayerCmd->SetRange("Size>0.");
  DeadLayerCmd->SetUnitCategory("Length");    
  DeadLayerCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  //Shell and window dimensions
  crap = dir+"setShellHalfLength";
  ShellHalfLengthCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  ShellHalfLengthCmd->SetGuidance("Set the half length of the ourter shell.");
  ShellHalfLengthCmd->SetParameterName("Size",false);
  ShellHalfLengthCmd->SetRange("Size>0.");
  ShellHalfLengthCmd->SetUnitCategory("Length");    
  ShellHalfLengthCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  crap = dir+"setEndGap";
  EndGapCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  EndGapCmd->SetGuidance("Set the end gap size between the crystal and window.");
  EndGapCmd->SetParameterName("Size",false);
  EndGapCmd->SetRange("Size>0.");
  EndGapCmd->SetUnitCategory("Length");    
  EndGapCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
    
  crap = dir+"setWindowThick";
  WindowThickCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  WindowThickCmd->SetGuidance("Set Thickness of the window");
  WindowThickCmd->SetParameterName("Size",false);
  WindowThickCmd->SetRange("Size>=0.");
  WindowThickCmd->SetUnitCategory("Length");
  WindowThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  crap = dir+"setWallThick";
  WallThickCmd = new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
  WallThickCmd->SetGuidance("Set Thickness of the side wall");
  WallThickCmd->SetParameterName("Size",false);
  WallThickCmd->SetRange("Size>=0.");
  WallThickCmd->SetUnitCategory("Length");  
  WallThickCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CloverMessenger::~CloverMessenger()
{
  delete CrystalMaterCmd; delete WindowMaterCmd; delete WallMaterCmd;
  delete CrystalHalfLengthCmd; delete CrystalRadCmd; delete CrystalEndRadCmd;
  delete HoleRadCmd; delete HoleDepthCmd;
  delete DeadLayerCmd;
  delete ShellHalfLengthCmd; delete EndGapCmd;
  delete WindowThickCmd; delete WallThickCmd;

  delete CloverDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //materials
  if( command == CrystalMaterCmd )
   { Detector->SetCrystalMaterial(newValue);}
   
  if( command == WindowMaterCmd )
   { Detector->SetWindowMaterial(newValue);}
  
  if( command == WallMaterCmd )
   { Detector->SetWallMaterial(newValue);}
  
  //Crystal dimensions
  if( command == CrystalHalfLengthCmd )
   { Detector->SetCrystalHalfLength(CrystalHalfLengthCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalRadCmd )
   { Detector->SetCrystalRad(CrystalRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == CrystalEndRadCmd )
   { Detector->SetCrystalEndRad(CrystalEndRadCmd->GetNewDoubleValue(newValue));}

  if( command == HoleRadCmd )
   { Detector->SetHoleRad(HoleRadCmd->GetNewDoubleValue(newValue));}
   
  if( command == HoleDepthCmd )
   { Detector->SetHoleDepth(HoleDepthCmd->GetNewDoubleValue(newValue));}
   
  if( command == DeadLayerCmd )
   { Detector->SetDeadLayerThick(DeadLayerCmd->GetNewDoubleValue(newValue));}
   
   //Shell Dimensions
  if( command == ShellHalfLengthCmd )
   { Detector->SetShellHalfLength(ShellHalfLengthCmd->GetNewDoubleValue(newValue));}
   
  if( command == EndGapCmd )
   { Detector->SetEndGap(EndGapCmd->GetNewDoubleValue(newValue));}
   
  if( command == WindowThickCmd )
   { Detector->SetWindowThickness(WindowThickCmd->GetNewDoubleValue(newValue));}

  if( command == WallThickCmd )
   { Detector->SetWallThickness(WallThickCmd->GetNewDoubleValue(newValue));}
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
