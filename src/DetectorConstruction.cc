//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/DetectorConstruction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4RotationMatrix.hh"

#include "G4Box.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
:defaultMaterial(0),
 solidWorld(0),logicWorld(0),physiWorld(0)
{
  //detectorDistance = .16/2*cm;
  detectorDistance = 0.3175/2*cm;

  // materials
  DefineMaterials();

  //initialize detectors
  CloverDet[0] = new Clover("Clover1");
  CloverDet[1] = new Clover("Clover2");

  NaIDet = new NaIAnnulus("NaI");
  
  DySamp1 = new DySample1("Dy1");
  DySamp2 = new DySample2("Dy2");

  // Calculate the world size
  ComputeTwoCloverParameters();
  
  // create commands for interactive definition 
  detectorMessenger = new DetectorMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction(){ 
  delete detectorMessenger;
  delete CloverDet[0];
  delete CloverDet[1];
  delete NaIDet;
  delete DySamp1;
  delete DySamp2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){
  return ConstructTwoClover();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials(){ 
 
  G4double a, z, density;      //z=mean number of protons;  
                                 //a=mass of a mole;

  // vacuum
  G4Material* Vacuum =
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                               kStateGas, 2.73*kelvin, 3.e-18*pascal);
    
  //default materials of the World
  defaultMaterial  = Vacuum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::ConstructTwoClover(){

  // Clean old geometry, if any
  G4GeometryManager::GetInstance()->OpenGeometry();
  G4PhysicalVolumeStore::GetInstance()->Clean();
  G4LogicalVolumeStore::GetInstance()->Clean();
  G4SolidStore::GetInstance()->Clean();

  // complete the TwoClover parameters definition
  ComputeTwoCloverParameters();
   
  // World
  solidWorld = new G4Box("World",               //its name
                   WorldSizeX/2,WorldSizeYZ/2,WorldSizeYZ/2);   //its size
                         
  logicWorld = new G4LogicalVolume(solidWorld,      //its solid
                                   defaultMaterial, //its material
                                   "World");        //its name
                                   
  physiWorld = new G4PVPlacement(0,         //no rotation
                 G4ThreeVector(),   //at (0,0,0)
                                 logicWorld,        //its logical volume                 
                                 "World",       //its name
                                 0,         //its mother  volume
                                 false,         //no boolean operation
                                 0);            //copy number

  // Clover 
  G4ThreeVector *DetPos[2];
  G4RotationMatrix *DetRot[2];

  DetPos[0] = new G4ThreeVector(-detectorDistance,0,0);
  DetPos[1] = new G4ThreeVector(detectorDistance,0,0);
  DetRot[0] = new G4RotationMatrix();
  DetRot[0]->rotateX(180*deg);
  DetRot[1] = new G4RotationMatrix();

  CloverDet[1]->BuildClover(logicWorld, DetPos[1], DetRot[1]);
  CloverDet[0]->BuildClover(logicWorld, DetPos[0], DetRot[0]);
  /*

  // NaI Annulus
  G4ThreeVector *NaIPos = new G4ThreeVector(0,0,0);
  G4RotationMatrix *NaIRot = new G4RotationMatrix();

  NaIDet->BuildNaIAnnulus(logicWorld, NaIPos, NaIRot);
  */

  //Sample
  G4ThreeVector *sampPos = new G4ThreeVector(0,0,0);
  G4RotationMatrix *sampRot = new G4RotationMatrix();

  //DySamp1->BuildDySample(logicWorld, sampPos, sampRot);
  DySamp2->BuildDySample(logicWorld, sampPos, sampRot);

  PrintTwoCloverParameters();     
  
  // Visualization attributes
  logicWorld->SetVisAttributes (G4VisAttributes::Invisible);

  //always return physical world
  return physiWorld;    //always return the physical World
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::PrintTwoCloverParameters(){
  G4cout << "\n------------------------------------------------------------"
         << "\n---> Detector Distance  " << detectorDistance/cm << "cm\n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetDetectorDistance(G4double val){
  detectorDistance = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RunManager.hh"

void DetectorConstruction::UpdateGeometry(){
  G4RunManager::GetRunManager()->DefineWorldVolume(ConstructTwoClover());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
