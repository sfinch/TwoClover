//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DySample.hh"

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

DySample::DySample(G4String giveName)
{

  name = giveName;

  // default parameter values of the sample
  sampleThick	 = 1.*mm;
  sampleWidth	 = 10.*mm;
  sampleHeight   = 10.*mm;

  ComputeDySampleParameters();
  
  // materials
  DefineMaterials();
  G4NistManager* manager = G4NistManager::Instance();

  sampleMaterial = manager->FindOrBuildMaterial("G4_Dy");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DySample::~DySample(){ 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample::DefineMaterials()
{ 
 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample::BuildDySample(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot){

  logicWorld = logWorld;
  SampleRot = rot;
  SamplePos = G4ThreeVector(pos->x(),pos->y(),pos->z());

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->transform(*SampleRot);

  // complete the Dy Sample parameters definition
  ComputeDySampleParameters();
  
  // Sample 
  sample = new G4Box("sample", //name
  			sampleThick,	// thickness
			sampleHeight, 	// height
			sampleWidth);	// width
  
  logSample = new G4LogicalVolume(sample,
  			sampleMaterial,	//material
			"logSample");

  physSample = new G4PVPlacement(rm,	//rotation
					SamplePos,	//placement
					logSample,	//its logical volume
					"physSample",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number

  PrintDySampleParameters();     

  // Visualization attributes
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.1,0.1,0.1,1));
  atb->SetForceSolid(true);
  logSample->SetVisAttributes(atb);}
  
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample::PrintDySampleParameters(){
	
  
  G4double SampVol = sample->GetCubicVolume();

  G4cout << "\n------------------------------------------------------------"
		 << "---> Sample \n"
         << sampleThick/mm << "mm thickness \n" 
         << sampleWidth/mm << "mm width\n" 
         << sampleHeight/mm << "mm height\n" 
		 << "--->Calculated quantities \n"
		 << SampVol/(cm*cm*cm) << " cm^3 sample 1 volume \n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DySample::SetName(G4String giveName){
	name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
