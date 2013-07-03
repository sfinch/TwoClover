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
#include "G4IntersectionSolid.hh"
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
	crystalHalfLength[i] = (80./2)*mm;
	crystalRad[i] = (50./2)*mm;

	crystalInOffset[i] = (22)*mm;
	crystalOutOffset[i] = (23)*mm;
	crystalSeparation[i] = (0.3)*mm;

	holeRad[i] = (9./2)*mm;
	holeDepth[i] = (35.5)*mm;
  }
  // default parameter values of the clover 
  shellHalfLength = (105./2)*mm;
  endGap = 3.5*mm;
  windowThickness = 2.54*mm;
  wallThickness = 1.5*mm;
  

  ComputeCloverParameters();
  
  // materials
  SetCrystalMaterial("Ge");
  SetWindowMaterial("Al");
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

  G4ThreeVector CryPlacement[4];
  G4RotationMatrix *rm[4];
  for (int i=0; i<4; i++){
  	rm[i] = new G4RotationMatrix(); 
	rm[i]->rotateY(90.*deg);
    rm[i]->transform(*DetRot);
  }
  rm[0]->rotateZ(90.*deg);
  rm[1]->rotateZ(180.*deg);
  rm[2]->rotateZ(270.*deg);
  if (multx==1){
  	CryPlacement[0] = G4ThreeVector(multx*(crystalHalfLength[3]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() - crystalInOffset[3] - crystalSeparation[3],
						multz*DetPos->z() + crystalInOffset[3] + crystalSeparation[3]);
  	CryPlacement[1] = G4ThreeVector(multx*(crystalHalfLength[2]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() - crystalInOffset[2] - crystalSeparation[2],
						multz*DetPos->z() - crystalInOffset[2] - crystalSeparation[2]);
  	CryPlacement[2] = G4ThreeVector(multx*(crystalHalfLength[1]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() + crystalInOffset[1] + crystalSeparation[1],
						multz*DetPos->z() - crystalInOffset[1] - crystalSeparation[1]);
  	CryPlacement[3] = G4ThreeVector(multx*(crystalHalfLength[0]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() + crystalInOffset[0] + crystalSeparation[0],
						multz*DetPos->z() + crystalInOffset[0] + crystalSeparation[0]);
  }
  else if (multx==-1){
  	CryPlacement[0] = G4ThreeVector(multx*(crystalHalfLength[0]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() + crystalInOffset[0] + crystalSeparation[0],
						multz*DetPos->z() + crystalInOffset[0] + crystalSeparation[0]);
  	CryPlacement[1] = G4ThreeVector(multx*(crystalHalfLength[1]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() + crystalInOffset[1] + crystalSeparation[1],
						multz*DetPos->z() - crystalInOffset[1] - crystalSeparation[1]);
  	CryPlacement[2] = G4ThreeVector(multx*(crystalHalfLength[2]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() - crystalInOffset[2] - crystalSeparation[2],
						multz*DetPos->z() - crystalInOffset[2] - crystalSeparation[2]);
  	CryPlacement[3] = G4ThreeVector(multx*(crystalHalfLength[3]+windowThickness+endGap+DetPos->x()),
   						multy*DetPos->y() - crystalInOffset[3] - crystalSeparation[3],
						multz*DetPos->z() + crystalInOffset[3] + crystalSeparation[3]);
  }

  // complete the clover parameters definition
  ComputeCloverParameters();
   
  // Crystal
  G4VSolid *cyl1[4];
  G4VSolid *cylbox[4];
  G4VSolid *cyl2[4];
  G4VSolid *inCyl[4];
  G4VSolid *inSph[4];
  for (int i=0; i<4; i++){
	G4double width  = crystalInOffset[i]+crystalOutOffset[i];
	G4double offset = (crystalOutOffset[i]-crystalInOffset[i])*0.5;

	//Making the exterior crystal shape
	cyl1[i] = new G4Tubs("cyl1", //name
  			0.*cm, 			//inner radius
			crystalRad[i], 	//outer radius
			crystalHalfLength[i], //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
	cylbox[i] = new G4Box("cylbox1", //name
			width/2, width/2, crystalHalfLength[i]);
	cyl2[i] = new G4IntersectionSolid("cyl2",
			cyl1[i],cylbox[i],0,G4ThreeVector(offset,offset,0));
  
	//making the hole
	inCyl[i] = new G4Tubs("inCyl", //name
			0.*cm, 			//inner radius
			holeRad[i], 	//outer radius
			0.5*(holeDepth[i]-holeRad[i]), //z half length
			0.*deg,			//starting phi
			360.*deg);		//ending phi
	inSph[i] = new G4Orb("inSph",holeRad[i]);
	hole[i] = new G4UnionSolid("hole",
  			inCyl[i],inSph[i],0,G4ThreeVector(0.,0.,.5*(holeDepth[i]-holeRad[i])));
  
	//final detector shape
	activeCrystal[i] = new G4SubtractionSolid("solidCrystal",
  			cyl2[i],hole[i],0,G4ThreeVector(0.,0.,-crystalHalfLength[i]+0.5*holeDepth[i]-0.5*holeRad[i]));

	logCrystal[i] = new G4LogicalVolume(activeCrystal[i],
  			crystalMaterial,	//material
			"logCrystal");

	physiCrystal[i] = new G4PVPlacement(rm[i],	//rotation
					CryPlacement[i],
					logCrystal[i],	//its logical volume
					"physiCrystal",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  }
 
  //Make the outer shell
  G4VSolid *shell1 = roundedBox(8*cm, 2*shellHalfLength, 1.5*cm);
  G4VSolid *shell2 = roundedBox(6*cm, 2*shellHalfLength, 1.5*cm);
  //G4VSolid *outerShell = new G4SubtractionSolid("outerShell", shell1, shell2, 0, G4ThreeVector(0,0,0));
  G4VSolid *asdf = new G4Box("asdf", 3*cm, 3*cm, 2*shellHalfLength);	//size
  G4VSolid *outerShell = new G4SubtractionSolid("outerShell", shell1, asdf, 0, G4ThreeVector(0,0,0));
  logShell = new G4LogicalVolume(outerShell,
  //logShell = new G4LogicalVolume(shell1,
  			wallMaterial,	//material
			"logShell");
  /*
  physiShell = new G4PVPlacement(rm[0],	//rotation
					G4ThreeVector(multx*(shellHalfLength+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()),	//placement
					logShell,	//its logical volume
					"physiShell",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
					*/


  //Make the window
  G4VSolid *window = roundedBox(8*cm, windowThickness, 1.5*cm);
  logWindow = new G4LogicalVolume(window,
  			windowMaterial,	//material
			"logWindow");
  physiWindow = new G4PVPlacement(rm[0],	//rotation
					G4ThreeVector(multx*(.5*windowThickness+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()),	//placement
					logWindow,	//its logical volume
					"physiWindow",	//its name
					logicWorld,	//its mother  volume
					false,		//no boolean operation
					0);		//copy number
  			
  PrintCloverParameters();
  
 // Visualization attributes
 // crysal
 for (int i=0; i<4;i++){
 	{G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.7));
	 atb->SetForceSolid(true);
	 logCrystal[i]->SetVisAttributes(atb);}
  }
  
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
  G4double holeVol = hole->GetCubicVolume();
  G4double holeSA = hole->GetSurfaceArea();
  */

  G4cout << "\n------------------------------------------------------------"
         << "\n--->  " << name << "  <---\n"
         << "\n---> The crysyal is " << crystalMaterial->GetName() << "\n"
		 /*
         << crystalRad/mm << "mm outer radius \n"
         << 2.*crystalHalfLength/mm << "mm length \n" 
         << holeRad/mm << "mm hole radius \n" 
         << holeDepth/mm << "mm hole depth \n" 
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
		 */
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VSolid* Clover::roundedBox(G4double height, G4double width, G4double curv){
	G4ThreeVector corner[4];
  	G4RotationMatrix *rm[4]; 
	G4double x = height*0.5 - curv;
	G4double y = height*0.5 - curv;
	G4VSolid *edge[4];
	G4VSolid *rounded[4];

	corner[0] = G4ThreeVector(x,y,0);
	corner[1] = G4ThreeVector(x,-y,0);
	corner[2] = G4ThreeVector(-x,-y,0);
	corner[3] = G4ThreeVector(-x,y,0);
	
	G4VSolid *box1 = new G4Box("box1",		//name	
								height/2, y, width/2);	//size
	G4VSolid *box2 = new G4Box("box2",		//name	
								x, height/2, width/2);	//size
	G4VSolid *box  = new G4UnionSolid("box", box1, box2, 0, G4ThreeVector(0,0,0));

	for (int i=0; i<4; i++){
		rm[i] = new G4RotationMatrix();
		edge[i] = new G4Tubs("edge"+i, //name
  			0.*cm, 		//inner radius
			curv, 	//outer radius
			width/2, 	//z half length
			0.*deg,		//starting phi
			90.*deg);	//ending phi
  		rm[i]->rotateZ(i*90.*deg);

		if (i==0){
		  rounded[i] = new G4UnionSolid("rounded"+i, box, edge[i], rm[i], corner[i]);
		}
		else{
		  rounded[i] = new G4UnionSolid("rounded"+i, rounded[i-1], edge[i], rm[i], corner[i]);
		}
	}
	
	return rounded[3];
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

void Clover::SetCrystalInOffset(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalInOffset[seg] = val;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::SetCrystalOutOffset(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalOutOffset[seg] = val;
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

void Clover::SetCrystalSeparation(int seg, G4double val){
  if (seg>=0 || seg<4){ 
  	crystalSeparation[seg] = val;
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

