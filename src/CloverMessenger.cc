//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/CloverMessenger.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include <stdio.h>
#include <stdlib.h>

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
  
  // directory
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
  for (int i=0; i<4; i++){
    G4String seg = G4UIcommand::ConvertToString(i+1);

    crap = dir+"setCrystal"+seg+"HalfLength";
    CrystalHalfLengthCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    CrystalHalfLengthCmd[i]->SetGuidance("Set the half length of the crystal.");
    CrystalHalfLengthCmd[i]->SetParameterName("Size",false);
    CrystalHalfLengthCmd[i]->SetRange("Size>0.");
    CrystalHalfLengthCmd[i]->SetUnitCategory("Length");    
    CrystalHalfLengthCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    crap = dir+"setCrystal"+seg+"Rad";
    CrystalRadCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    CrystalRadCmd[i]->SetGuidance("Set the radius of the crystal.");
    CrystalRadCmd[i]->SetParameterName("Size",false);
    CrystalRadCmd[i]->SetRange("Size>0.");
    CrystalRadCmd[i]->SetUnitCategory("Length");    
    CrystalRadCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
    
    crap = dir+"setCrystal"+seg+"InOffset";
    CrystalInOffsetCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    CrystalInOffsetCmd[i]->SetGuidance("Set the inner offset of the crystal.");
    CrystalInOffsetCmd[i]->SetParameterName("Size",false);
    CrystalInOffsetCmd[i]->SetRange("Size>0.");
    CrystalInOffsetCmd[i]->SetUnitCategory("Length");    
    CrystalInOffsetCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);

    crap = dir+"setCrystal"+seg+"OutOffset";
    CrystalOutOffsetCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    CrystalOutOffsetCmd[i]->SetGuidance("Set the outer offset of the crystal.");
    CrystalOutOffsetCmd[i]->SetParameterName("Size",false);
    CrystalOutOffsetCmd[i]->SetRange("Size>0.");
    CrystalOutOffsetCmd[i]->SetUnitCategory("Length");    
    CrystalOutOffsetCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);

    crap = dir+"setHole"+seg+"Rad";
    HoleRadCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    HoleRadCmd[i]->SetGuidance("Set the end radius of the hole in the crystal.");
    HoleRadCmd[i]->SetParameterName("Size",false);
    HoleRadCmd[i]->SetRange("Size>0.");
    HoleRadCmd[i]->SetUnitCategory("Length");    
    HoleRadCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
      
    crap = dir+"setHole"+seg+"Depth";
    HoleDepthCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    HoleDepthCmd[i]->SetGuidance("Set the depth of the hole in the crystal.");
    HoleDepthCmd[i]->SetParameterName("Size",false);
    HoleDepthCmd[i]->SetRange("Size>0.");
    HoleDepthCmd[i]->SetUnitCategory("Length");    
    HoleDepthCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);
      
    crap = dir+"setDeadLayer"+seg+"Thick";
    DeadLayerCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    DeadLayerCmd[i]->SetGuidance("Set the thickness of the dead layer.");
    DeadLayerCmd[i]->SetParameterName("Size",false);
    DeadLayerCmd[i]->SetRange("Size>0.");
    DeadLayerCmd[i]->SetUnitCategory("Length");    
    DeadLayerCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);

    crap = dir+"setCrystal"+seg+"Separation";
    CrystalSeparationCmd[i]= new G4UIcmdWithADoubleAndUnit(crap.c_str(),this);
    CrystalSeparationCmd[i]->SetGuidance("Set the crystal's  separation.");
    CrystalSeparationCmd[i]->SetParameterName("Size",false);
    CrystalSeparationCmd[i]->SetRange("Size>0.");
    CrystalSeparationCmd[i]->SetUnitCategory("Length");    
    CrystalSeparationCmd[i]->AvailableForStates(G4State_PreInit,G4State_Idle);

  }
  
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

  for (int i=0; i<4; i++){
    delete CrystalHalfLengthCmd[i]; delete CrystalRadCmd[i]; 
    delete CrystalInOffsetCmd[i]; delete CrystalOutOffsetCmd[i];
    delete HoleRadCmd[i]; delete HoleDepthCmd[i];
    delete DeadLayerCmd[i];
    delete CrystalSeparationCmd[i];
  }

  delete ShellHalfLengthCmd; delete EndGapCmd;
  delete WindowThickCmd; delete WallThickCmd;

  delete CloverDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CloverMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{ 
  //materials
  if (command == CrystalMaterCmd){
    Detector->SetCrystalMaterial(newValue);
  }
   
  if (command == WindowMaterCmd){
    Detector->SetWindowMaterial(newValue);
  }
  
  if (command == WallMaterCmd){
    Detector->SetWallMaterial(newValue);
  }
  
  //Crystal dimensions
  for (int i=0; i<4; i++){
     if (command == CrystalHalfLengthCmd[i]){
       Detector->SetCrystalHalfLength(i, CrystalHalfLengthCmd[i]->GetNewDoubleValue(newValue));
     }
   
     if (command == CrystalRadCmd[i]){
       Detector->SetCrystalRad(i, CrystalRadCmd[i]->GetNewDoubleValue(newValue));
     }
   
     if (command == CrystalInOffsetCmd[i]){
       Detector->SetCrystalInOffset(i, CrystalInOffsetCmd[i]->GetNewDoubleValue(newValue));
     }

     if (command == CrystalOutOffsetCmd[i]){
       Detector->SetCrystalOutOffset(i, CrystalOutOffsetCmd[i]->GetNewDoubleValue(newValue));
     }

     if (command == HoleRadCmd[i]){
       Detector->SetHoleRad(i, HoleRadCmd[i]->GetNewDoubleValue(newValue));
     }
   
     if (command == HoleDepthCmd[i]){
       Detector->SetHoleDepth(i, HoleDepthCmd[i]->GetNewDoubleValue(newValue));
     }
   
     if (command == DeadLayerCmd[i]){
       Detector->SetDeadLayerThick(i, DeadLayerCmd[i]->GetNewDoubleValue(newValue));
     }
   
     if (command == CrystalSeparationCmd[i]){
       Detector->SetCrystalSeparation(i, CrystalSeparationCmd[i]->GetNewDoubleValue(newValue));
     }

  }
   
   //Shell Dimensions
  if (command == ShellHalfLengthCmd){
    Detector->SetShellHalfLength(ShellHalfLengthCmd->GetNewDoubleValue(newValue));
  }
   
  if (command == EndGapCmd){
    Detector->SetEndGap(EndGapCmd->GetNewDoubleValue(newValue));
  }
   
  if (command == WindowThickCmd){
    Detector->SetWindowThickness(WindowThickCmd->GetNewDoubleValue(newValue));
  }

  if (command == WallThickCmd){
    Detector->SetWallThickness(WallThickCmd->GetNewDoubleValue(newValue));
  }
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
