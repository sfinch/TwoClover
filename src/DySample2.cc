//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/DySample2.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DySample2.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
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

DySample2::DySample2(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  bagThickL      = ((4*.01905)+.00889)*cm;
  bagThickR      = 2*.00889*cm;
  bagThickC      = 4*.02794*cm;

  sample1Width    = 3.3*cm;
  sample1Height   = 3.8*cm;
  sample1Thick    = .1397*cm - 6*.00889*cm;

  sample2Width    = 2.8*cm;
  sample2Height   = 3.5*cm;
  sample2Thick    = .1651*cm - 8*.01905*cm;

  ComputeDySampleParameters();

  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();

  bagMaterial = manager->FindOrBuildMaterial("G4_POLYETHYLENE");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DySample2::~DySample2(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample2::DefineMaterials()
{ 
    
  G4String symbol;             //a=mass of a mole;
  G4double a, z, density;      //z=mean number of protons;  
  G4int iz, n;                 //iz=number of protons  in an isotope; 
                               // n=number of nucleons in an isotope;

  G4int ncomponents, natoms;
  G4double abundance;

  // define Elements
  G4Element* O  = new G4Element("Oxygen"  ,symbol="O" , z= 8., a= 16.00*g/mole);

  // define isotopes by relative abundance 
  G4Isotope* Dy156 = new G4Isotope("Dy156", iz=66, n=156, a=156.*g/mole);
  G4Isotope* Dy158 = new G4Isotope("Dy158", iz=66, n=158, a=158.*g/mole);
  G4Isotope* Dy160 = new G4Isotope("Dy160", iz=66, n=160, a=160.*g/mole);
  G4Isotope* Dy161 = new G4Isotope("Dy161", iz=66, n=161, a=161.*g/mole);
  G4Isotope* Dy162 = new G4Isotope("Dy162", iz=66, n=162, a=162.*g/mole);
  G4Isotope* Dy163 = new G4Isotope("Dy163", iz=66, n=163, a=163.*g/mole);
  G4Isotope* Dy164 = new G4Isotope("Dy164", iz=66, n=164, a=164.*g/mole);

  G4Element* sample1  = new G4Element("Dy Sample 1",symbol="Dy1",ncomponents=7);
  sample1->AddIsotope(Dy156, abundance=21.59*perCent);
  sample1->AddIsotope(Dy158, abundance=00.36*perCent);
  sample1->AddIsotope(Dy160, abundance=03.10*perCent);
  sample1->AddIsotope(Dy161, abundance=19.29*perCent);
  sample1->AddIsotope(Dy162, abundance=20.69*perCent);
  sample1->AddIsotope(Dy163, abundance=17.70*perCent);
  sample1->AddIsotope(Dy164, abundance=17.26*perCent);

  G4Element* sample2  = new G4Element("Dy Sample 2",symbol="Dy2",ncomponents=7);
  sample2->AddIsotope(Dy156, abundance=20.90*perCent);
  sample2->AddIsotope(Dy158, abundance=00.58*perCent);
  sample2->AddIsotope(Dy160, abundance=03.43*perCent);
  sample2->AddIsotope(Dy161, abundance=19.70*perCent);
  sample2->AddIsotope(Dy162, abundance=22.20*perCent);
  sample2->AddIsotope(Dy163, abundance=16.90*perCent);
  sample2->AddIsotope(Dy164, abundance=16.30*perCent);

  // define chemical molecule
  // must update volume of density to reflect smaple dim
  density = (0.8034/1.08295)*g/cm3;
  sampleMaterial1 = new G4Material("Dy2O3_sample1", density, ncomponents=2);
  sampleMaterial1->AddElement(sample1, natoms=2);
  sampleMaterial1->AddElement(O, natoms=3);

  density = (0.3443/0.12446)*g/cm3;
  sampleMaterial2 = new G4Material("Dy2O3_sample2", density, ncomponents=2);
  sampleMaterial2->AddElement(sample2, natoms=2);
  sampleMaterial2->AddElement(O, natoms=3);
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample2::BuildDySample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  //SamplePos = pos;

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->transform(*SampleRot);

  // complete the Dy Sample parameters definition
  ComputeDySampleParameters();
  
  // Sample 1
  sample1 = new G4Box("sample1", //name
            sample1Thick/2,    // thickness
            sample1Height/2,   // height
            sample1Width/2);   // width
  
  logSample1 = new G4LogicalVolume(sample1,
            sampleMaterial1, //material
            "logSample1");

  physSample1 = new G4PVPlacement(rm,    //rotation
                    SamplePos1,  //placement
                    logSample1,  //its logical volume
                    "physSample",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  // Sample 2
  sample2 = new G4Box("sample2", //name
            sample2Thick/2,    // thickness
            sample2Height/2,   // height
            sample2Width/2);   // width
  
  logSample2 = new G4LogicalVolume(sample2,
            sampleMaterial2, //material
            "logSample2");

  physSample2 = new G4PVPlacement(rm,    //rotation
                    SamplePos2,  //placement
                    logSample2,  //its logical volume
                    "physSample",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  //bag
  bagL = new G4Box("bagL", //name
            bagThickL/2,    // thickness
            sample1Height/2,   // height
            sample1Width/2);   // width
  
  logBagL = new G4LogicalVolume(bagL,
            bagMaterial, //material
            "logBagL");

  physBagL = new G4PVPlacement(rm,    //rotation
                    bagPosL,  //placement
                    logBagL,  //its logical volume
                    "physBagL",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  bagR = new G4Box("bagR", //name
            bagThickR/2,    // thickness
            sample1Height/2,   // height
            sample1Width/2);   // width
  
  logBagR = new G4LogicalVolume(bagR,
            bagMaterial, //material
            "logBagR");

  physBagR = new G4PVPlacement(rm,    //rotation
                    bagPosR,  //placement
                    logBagR,  //its logical volume
                    "physBagR",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  bagC = new G4Box("bagC", //name
            bagThickC/2,    // thickness
            sample1Height/2,   // height
            sample1Width/2);   // width
  
  logBagC = new G4LogicalVolume(bagC,
            bagMaterial, //material
            "logBagC");

  physBagC = new G4PVPlacement(rm,    //rotation
                    bagPosC,  //placement
                    logBagC,  //its logical volume
                    "physBagC",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number

  PrintDySampleParameters();     

  // Visualization attributes
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,1));
  atb->SetForceSolid(true);
  logSample1->SetVisAttributes(atb);
  logSample2->SetVisAttributes(atb);}

  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,.3));
  atb->SetForceSolid(true);
  logBagL->SetVisAttributes(atb);
  logBagR->SetVisAttributes(atb);
  logBagC->SetVisAttributes(atb);}
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample2::PrintDySampleParameters(){
  
  G4double SampVol1 = sample1->GetCubicVolume();
  G4double SampVol2 = sample2->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
         << "---> Sample \n"
         << sample1Thick/mm << "mm thickness \n" 
         << sample1Width/mm << "mm width\n" 
         << sample1Height/mm << "mm height\n" 
         << "--->Calculated quantities \n"
         << SampVol1/(cm*cm*cm) << " cm^3 sample 1 volume \n"
         << SampVol2/(cm*cm*cm) << " cm^3 sample 2 volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample2::SetName(G4String giveName){
    name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
