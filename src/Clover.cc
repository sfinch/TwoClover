//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Clover.hh"
#include "CloverMessenger.hh"

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

Clover::Clover(G4String giveName)
{

  name = giveName;

  // default parameter values of the crystal 
  for (int i=0; i<4; i++){
	crystalHalfLength[i] = (49.2/2)*mm;
	crystalRad[i] = (87.5/2)*mm;
	crystalEndRad[i] = (8)*mm;
	holeRad[i] = (9./2)*mm;
	holeDepth[i] = (35.5)*mm;
	deadLayerThick[i] = .7*mm;
  }
  // default parameter values of the clover 
  shellHalfLength = (105./2)*mm;
  endGap = 3.5*mm;
  windowThickness = 2.54*mm;
  wallThickness = 1.5*mm;
  

  ComputeCloverParameters();
  
  // materials
  SetCrystalMaterial("Ge");
  SetWindowMaterial("Mg");
  SetWallMaterial("Al");

  cloverMessenger = new CloverMessenger(this);
  //BuildClover();
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Clover::~Clover(){ 
	delete cloverMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::BuildClover(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot){

  logicWorld = logWorld;
  DetPos = pos;
  DetRot = rot;
  int multx = 1;
  int multy = 1;
  int multz = 1;

  if (DetPos->x()<0){
  	multx = -1*multx;
    DetPos->setX(-1*pos->x());
  }
  if (DetPos->y()<0){
  	multy = -1*multy;
    DetPos->setY(-1*pos->y());
  }
  if (DetPos->x()<0){
    multz = -1*multz;
    DetPos->setZ(-1*pos->z());
  }

  G4RotationMatrix *rm = new G4RotationMatrix();
  rm->rotateY(90.*deg);
  rm->transform(*DetRot);
  G4ThreeVector CryPlacement[4];
  G4ThreeVector deadPlacement[4];
  for (int i=0; i<1; i++){
    CryPlacement[i] = G4ThreeVector(multx*(crystalHalfLength[i]+windowThickness+endGap+DetPos->x()),
   								multy*DetPos->y(),multz*DetPos->z());
  }

   G4RotationMatrix *norot = new G4RotationMatrix();
   norot->rotateY(90.*deg);
   norot->transform(*DetRot);

  // complete the clover parameters definition
  ComputeCloverParameters();
   
  // Crystal
  for (int i=0; i<1; i++){

	//Making the exterior crystal shape
	G4VSolid *cyl1[4];
	cyl1[i] = new G4Tubs("cyl1", //name
  			0.*cm, 			//inner radius
			crystalRad[i], 	//outer radius
			crystalHalfLength[i], //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  
	//making the hole
	G4VSolid *inCyl[4];
	inCyl[i] = new G4Tubs("inCyl", //name
			0.*cm, 			//inner radius
			holeRad[i], 	//outer radius
			0.5*(holeDepth[i]-holeRad[i]), //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
	G4VSolid *inSph[4];
	inSph[i] = new G4Orb("inSph",holeRad[i]);
	hole[i] = new G4UnionSolid("hole",
  			inCyl[i],inSph[i],0,G4ThreeVector(0.,0.,.5*(holeDepth[i]-holeRad[i])));
  
	//final detector shape
	activeCrystal[i] = new G4SubtractionSolid("solidCrystal",
  			cyl1[i],hole[i],0,G4ThreeVector(0.,0.,-crystalHalfLength[i]+0.5*holeDepth[i]-0.5*holeRad[i]+.5*deadLayerThick[i]));

	logCrystal[i] = new G4LogicalVolume(activeCrystal[i],
  			crystalMaterial,	//material
			"logCrystal");

	physiCrystal[i] = new G4PVPlacement(rm,	//rotation
					//G4ThreeVector(crystalHalfLength+windowThickness+endGap+.5*deadLayerThick,0.,0.),//placement
					CryPlacement[i],
					logCrystal[i],	//its logical volume
					"physiCrystal",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  }
 
  //Make the outer shell
  G4VSolid *outerShell = new G4Tubs("outerShell",	//name
  			crystalRad[0]+endGap,	//inner rad
  			crystalRad[0]+endGap+wallThickness,	//outer rad
  			shellHalfLength,	//half length
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logShell = new G4LogicalVolume(outerShell,
  			wallMaterial,	//material
			"logShell");
  physiShell = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(multx*(shellHalfLength+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()),	//placement
					logShell,	//its logical volume
					"physiShell",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number


  //Make the window
  G4VSolid *window = new G4Tubs("window",	//name
  			0,	//inner rad
  			crystalRad[0]+endGap,	//outer rad
  			0.5*windowThickness,	//outer rad
			0.*deg,		//starting phi
			360.*deg);	//ending phi
  logWindow = new G4LogicalVolume(window,
  			windowMaterial,	//material
			"logWindow");
  physiWindow = new G4PVPlacement(rm,	//rotation
					G4ThreeVector(multx*(.5*windowThickness+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()),	//placement
					logWindow,	//its logical volume
					"physiWindow",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number


  			
  PrintCloverParameters();     
  
 // Visualization attributes
 // crysal
 for (int i=0; i<1;i++){
 	{G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.7));
	 atb->SetForceSolid(true);
	 logCrystal[i]->SetVisAttributes(atb);}
  }
  
 // dead layer
 //{G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.2));
 // atb->SetForceSolid(true);
 // logDeadLayer->SetVisAttributes(atb);}
  
 // window
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  atb->SetForceSolid(true);
  logWindow->SetVisAttributes(atb);}

 // shell
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.1));
  atb->SetForceSolid(true);
  logShell->SetVisAttributes(atb);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::PrintCloverParameters(){
	
  /*
  G4double activeVol = activeCrystal->GetCubicVolume();
  G4double activeSA = activeCrystal->GetSurfaceArea();
  G4double deadVol = deadLayer->GetCubicVolume();
  G4double deadSA = deadLayer->GetSurfaceArea();
  G4double holeVol = hole->GetCubicVolume();
  G4double holeSA = hole->GetSurfaceArea();
  */

  G4cout << "\n------------------------------------------------------------"
         << "\n--->  " << name << "  <---\n"
         << "\n---> The crysyal is " << crystalMaterial->GetName() << "\n"
		 /*
         << crystalRad/mm << "mm outer radius \n"
         << 2.*crystalHalfLength/mm << "mm length \n" 
         << crystalEndRad/mm << "mm end radius \n" 
         << holeRad/mm << "mm hole radius \n" 
         << holeDepth/mm << "mm hole depth \n" 
         << deadLayerThick/mm << "mm dead layer \n" 
		 */
		 << "---> Shielding properties \n"
         << 2.*shellHalfLength/mm << "mm shell length \n" 
         << wallThickness/mm << "mm wall of " << wallMaterial->GetName() << "\n" 
         << endGap/mm << "mm end gap \n" 
         << windowThickness/mm << "mm window of " << windowMaterial->GetName() << "\n"
		 << "--->Calculated quantities \n"
		 /*
		 << holeVol/(cm*cm*cm) << " cm^3 hole volume \n"
		 << holeSA/(cm*cm) << " cm^2 hole surface area \n"
		 << activeVol/(cm*cm*cm) << " cm^3 Active volume \n"
		 << activeSA/(cm*cm) << " cm^2 Active surface area \n"
		 << deadVol/(cm*cm*cm) << " cm^3 Dead layer volume \n"
		 << deadSA/(cm*cm) << " cm^2  Dead layer surface area \n"
		 */
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetName(G4String giveName){
	name = giveName;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	crystalMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetWindowMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	windowMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetWallMaterial(G4String materialChoice){
  // search the material by its name   
	G4NistManager* manager = G4NistManager::Instance();
	wallMaterial = manager->FindOrBuildMaterial("G4_"+materialChoice);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalHalfLength(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalHalfLength[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalRad(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalRad[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalEndRad(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalEndRad[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetHoleRad(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	holeRad[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetHoleDepth(int seg, G4double val){
  if (seg>=0 || seg<4){ 
	holeDepth[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetDeadLayerThick(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	deadLayerThick[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetShellHalfLength(G4double val){
   shellHalfLength = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetEndGap(G4double val){
   endGap = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetWindowThickness(G4double val){
   windowThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetWallThickness(G4double val){
   wallThickness = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

