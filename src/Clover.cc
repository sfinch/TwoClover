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
  crystalHalfLength = (49.2/2)*mm;
  crystalRad = (87.5/2)*mm;
  crystalEndRad = (8)*mm;
  holeRad = (9./2)*mm;
  holeDepth = (35.5)*mm;
  deadLayerThick = .7*mm;
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
   G4ThreeVector CryPlacement = G4ThreeVector(multx*(crystalHalfLength+windowThickness+endGap+DetPos->x()),
   								multy*DetPos->y(),multz*DetPos->z());
   G4ThreeVector deadPlacement = G4ThreeVector(multx*(crystalHalfLength+windowThickness+endGap+DetPos->x()),
   								multy*DetPos->y(),multz*DetPos->z());

   G4RotationMatrix *norot = new G4RotationMatrix();
   norot->rotateY(90.*deg);
   norot->transform(*DetRot);

  // complete the clover parameters definition
  ComputeCloverParameters();
   
  // Crystal

  //Making the exterior crystal shape
  G4VSolid *cyl1 = new G4Tubs("cyl1", //name
  			0.*cm, 			//inner radius
			crystalRad-crystalEndRad, 	//outer radius
			crystalHalfLength, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *cyl2 = new G4Tubs("cyl2", //name
  			0.*cm, 			//innter radius
			crystalRad, 	//outer radius
			crystalHalfLength-.5*crystalEndRad, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *tor = new G4Torus("tor",	//name
  			0.*cm, 			//inner r
			crystalEndRad,	//outer r
 			crystalRad-crystalEndRad,//torus rad
			0.*deg, 		//start phi
			360.*deg);		//end phi
  G4VSolid *crystal1 = new G4UnionSolid("crystal1", //name
  			cyl1,cyl2,0,G4ThreeVector(0.,0.,-.5*crystalEndRad));
  G4VSolid *crystal2 = new G4UnionSolid("crystal2", //name
  			crystal1, tor,0,G4ThreeVector(0.,0.,crystalHalfLength-crystalEndRad));
  
  // making the active part of the crystal
  G4VSolid *activeCyl1 = new G4Tubs("activeCyl1", //name
  			0.*cm, 			//inner radius
			crystalRad-crystalEndRad, 	//outer radius
			crystalHalfLength-.5*deadLayerThick, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *activeCyl2 = new G4Tubs("activeCyl2", //name
  			0.*cm, 			//innter radius
			crystalRad-deadLayerThick, 	//outer radius
			crystalHalfLength-.5*crystalEndRad, //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *activeTor = new G4Torus("activeTor",	//name
  			0.*cm, 			//inner r
			crystalEndRad-deadLayerThick,	//outer r
 			crystalRad-crystalEndRad,//torus rad
			0.*deg, 		//start phi
			360.*deg);		//end phi
  G4VSolid *activeCrystal1 = new G4UnionSolid("activeCrystal1", //name
  			activeCyl1,activeCyl2,0,G4ThreeVector(0.,0.,0.5*deadLayerThick-.5*crystalEndRad));
  G4VSolid *activeCrystal2 = new G4UnionSolid("activeCrystal2", //name
  			activeCrystal1, activeTor,0,G4ThreeVector(0.,0.,crystalHalfLength-crystalEndRad+.5*deadLayerThick));
  
  //making the hole
  G4VSolid *inCyl = new G4Tubs("inCyl", //name
			0.*cm, 			//inner radius
			holeRad, 	//outer radius
			0.5*(holeDepth-holeRad), //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
  G4VSolid *inSph = new G4Orb("inSph",holeRad);
  hole = new G4UnionSolid("hole",
  			inCyl,inSph,0,G4ThreeVector(0.,0.,.5*(holeDepth-holeRad)));
  
  //final detector shape
  activeCrystal = new G4SubtractionSolid("solidCrystal",
  			activeCrystal2,hole,0,G4ThreeVector(0.,0.,-crystalHalfLength+0.5*holeDepth-0.5*holeRad+.5*deadLayerThick));
  deadLayer = new G4SubtractionSolid("deadLayer",
  			crystal2,activeCrystal2,0,G4ThreeVector(0.,0.,-deadLayerThick));

  logCrystal = new G4LogicalVolume(activeCrystal,
  			crystalMaterial,	//material
			"logCrystal");
  logDeadLayer = new G4LogicalVolume(deadLayer,
  			crystalMaterial,	//material
			"logDeadLayer");

  physiCrystal = new G4PVPlacement(rm,	//rotation
					//G4ThreeVector(crystalHalfLength+windowThickness+endGap+.5*deadLayerThick,0.,0.),//placement
					CryPlacement,
					logCrystal,	//its logical volume
					"physiCrystal",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  physiDeadLayer = new G4PVPlacement(rm,	//rotation
					//G4ThreeVector(crystalHalfLength+windowThickness+endGap,0.,0.),	//placement
					deadPlacement,
					logDeadLayer,	//its logical volume
					"physiDeadLayer",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
 
  //Make the outer shell
  G4VSolid *outerShell = new G4Tubs("outerShell",	//name
  			crystalRad+endGap,	//inner rad
  			crystalRad+endGap+wallThickness,	//outer rad
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
  			crystalRad+endGap,	//outer rad
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
 {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.7));
  atb->SetForceSolid(true);
  logCrystal->SetVisAttributes(atb);}
  
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
	
  G4double activeVol = activeCrystal->GetCubicVolume();
  G4double activeSA = activeCrystal->GetSurfaceArea();
  G4double deadVol = deadLayer->GetCubicVolume();
  G4double deadSA = deadLayer->GetSurfaceArea();
  G4double holeVol = hole->GetCubicVolume();
  G4double holeSA = hole->GetSurfaceArea();

  G4cout << "\n------------------------------------------------------------"
         << "\n--->  " << name << "  <---\n"
         << "\n---> The crysyal is " << crystalMaterial->GetName() << "\n"
         << crystalRad/mm << "mm outer radius \n"
         << 2.*crystalHalfLength/mm << "mm length \n" 
         << crystalEndRad/mm << "mm end radius \n" 
         << holeRad/mm << "mm hole radius \n" 
         << holeDepth/mm << "mm hole depth \n" 
         << deadLayerThick/mm << "mm dead layer \n" 
		 << "---> Shielding properties \n"
         << 2.*shellHalfLength/mm << "mm shell length \n" 
         << wallThickness/mm << "mm wall of " << wallMaterial->GetName() << "\n" 
         << endGap/mm << "mm end gap \n" 
         << windowThickness/mm << "mm window of " << windowMaterial->GetName() << "\n"
		 << "--->Calculated quantities \n"
		 << holeVol/(cm*cm*cm) << " cm^3 hole volume \n"
		 << holeSA/(cm*cm) << " cm^2 hole surface area \n"
		 << activeVol/(cm*cm*cm) << " cm^3 Active volume \n"
		 << activeSA/(cm*cm) << " cm^2 Active surface area \n"
		 << deadVol/(cm*cm*cm) << " cm^3 Dead layer volume \n"
		 << deadSA/(cm*cm) << " cm^2  Dead layer surface area \n"
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

void Clover::SetCrystalHalfLength(G4double val){
  crystalHalfLength = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalRad(G4double val){
  crystalRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalEndRad(G4double val){
  crystalEndRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetHoleRad(G4double val){
  holeRad = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetHoleDepth(G4double val){
  holeDepth = val;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetDeadLayerThick(G4double val){
  deadLayerThick = val;
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

