//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/NaIAnnulus.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "NaIAnnulus.hh"
#include "NaIAnnulusMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4Orb.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4RotationMatrix.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4Transform3D.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NaIAnnulus::NaIAnnulus(G4String giveName)
{
  name = giveName;

  // default parameter values of the wall
  wallInThickness = 0.787*mm;
  wallOutThickness = 3.175*mm;
  // default parameter values of the crystal 
  crystalHalfLength = (.5*(435.97-(2*12.7)))*mm;
  crystalInRad = ((.5*152.4) +0.878)*mm;
  crystalOutRad = ((.5*368.3)-3.175)*mm;
  
  ComputeNaIAnnulusParameters();
  
  // materials
  SetCrystalMaterial("SODIUM_IODIDE");
  SetWallMaterial("Al");

  NaIMessenger = new NaIAnnulusMessenger(this);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

NaIAnnulus::~NaIAnnulus(){ 
    delete NaIMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::BuildNaIAnnulus(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot){

  logicWorld = logWorld;

  // placement of NaI
  DetPos = pos;
  DetRot = rot;

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*DetRot);
  G4ThreeVector CryPlacement = G4ThreeVector(DetPos->x(),DetPos->y(),DetPos->z());

  // complete the NaIAnnulus parameters definition
  ComputeNaIAnnulusParameters();
  
  // Crystal
  crystal = new G4Tubs("crystal", //name
            crystalInRad,   //inner radius
            crystalOutRad,  //outer radius
            crystalHalfLength, //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logCrystal = new G4LogicalVolume(crystal,
            crystalMaterial,    //material
            "logCrystal");

  physCrystal = new G4PVPlacement(rm,   //rotation
                    CryPlacement,
                    logCrystal, //its logical volume
                    "physCrystal",  //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  //Inner wall
  inWall = new G4Tubs("inWall", //name
            crystalInRad-wallInThickness,   //inner radius
            crystalInRad,   //outer radius
            crystalHalfLength, //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logInWall = new G4LogicalVolume(inWall,
            wallMaterial,   //material
            "logInWall");

  physInWall = new G4PVPlacement(rm,    //rotation
                    CryPlacement,
                    logInWall,  //its logical volume
                    "physInWall",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number
 
  //Outer wall
  outWall = new G4Tubs("outWall", //name
            crystalOutRad,  //inner radius
            crystalOutRad+wallOutThickness,     //outer radius
            crystalHalfLength, //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
  
  logOutWall = new G4LogicalVolume(inWall,
            wallMaterial,   //material
            "logOutWall");

  physOutWall = new G4PVPlacement(rm,   //rotation
                    CryPlacement,
                    logOutWall, //its logical volume
                    "physOutWall",  //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number 
  
  PrintNaIAnnulusParameters();     

  // Visualization attributes
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.2));
  atb->SetForceSolid(true);
  logCrystal->SetVisAttributes(atb);}
  
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.1));
  atb->SetForceWireframe(true);
  logOutWall->SetVisAttributes(atb);}
  
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.1));
  atb->SetForceWireframe(true);
  logInWall->SetVisAttributes(atb);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::PrintNaIAnnulusParameters(){
    
  // calculate volume/surface area
  G4double activeVol = crystal->GetCubicVolume();
  G4double activeSA = crystal->GetSurfaceArea();

  // print
  G4cout << "\n------------------------------------------------------------"
         << "\n---> The NaI crysyal is " << crystalMaterial->GetName() << "\n"
         << crystalInRad/mm << "mm inner radius \n"
         << crystalOutRad/mm << "mm outer radius \n"
         << 2.*crystalHalfLength/mm << "mm length \n" 
         << "---> Wall properties " << wallMaterial->GetName() << "\n" 
         << wallInThickness/mm << "mm inner wall thickness \n" 
         << wallOutThickness/mm << "mm outer wall thickness \n" 
         << "--->Calculated quantities \n"
         << activeVol/(cm*cm*cm) << " cm^3 Active volume \n"
         << activeSA/(cm*cm) << " cm^2 Active surface area \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetName(G4String giveName){
  name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetCrystalMaterial(G4String materialChoice){
  // search the material by its name   
  G4NistManager* manager = G4NistManager::Instance();
  crystalMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetWallMaterial(G4String materialChoice){
  // search the material by its name   
  G4NistManager* manager = G4NistManager::Instance();
  wallMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetCrystalHalfLength(G4double val){
  crystalHalfLength = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetCrystalInRad(G4double val){
  crystalInRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetCrystalOutRad(G4double val){
  crystalOutRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetWallInThickness(G4double val){
  wallInThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void NaIAnnulus::SetWallOutThickness(G4double val){
  wallOutThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

