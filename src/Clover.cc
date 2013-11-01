//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/Clover.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "Clover.hh"
#include "CloverMessenger.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4UImanager.hh"

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

    holeRad[i] = (7.5/2)*mm;
    holeDepth[i] = (60.0)*mm;

    deadLayerThick[i] = (1.0)*mm;
  }

  // default parameter values of the clover 
  shellHalfLength = (105./2)*mm;
  shellWidth = 101.*mm;
  endGap = 3.5*mm;
  windowThickness = 2.54*mm;
  wallThickness = 1.5*mm;

  ComputeCloverParameters();
  
  // materials
  SetCrystalMaterial("Ge");
  SetWindowMaterial("Al");
  SetWallMaterial("Al");

  cloverMessenger = new CloverMessenger(this);
  
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

  // finding the positioning the detectors
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
  G4ThreeVector deadPlacement[4];
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
  for (int i=0; i<4; i++){
    deadPlacement[i] = G4ThreeVector(CryPlacement[i].x()+multx*(crystalHalfLength[i]-0.5*holeDepth[i]+0.5*holeRad[i]), 
        CryPlacement[i].y(), 
        CryPlacement[i].z());
  }


  // complete the clover parameters definition
  ComputeCloverParameters();
   
  // Crystal 
  G4VSolid *cyl1[4];
  G4VSolid *cylbox[4];
  G4VSolid *cyl2[4];
  G4VSolid *inCyl[4];
  G4VSolid *inSph[4];
  G4VSolid *dead1[4];
  G4VSolid *dead2[4];
  G4VSolid *dead3[4];

  for (int i=0; i<4; i++){
    G4double width  = crystalInOffset[i]+crystalOutOffset[i];
    G4double offset = (crystalOutOffset[i]-crystalInOffset[i])*0.5;

    //Making the exterior crystal shape
    cyl1[i] = new G4Tubs("cyl1", //name
            0.*cm,          //inner radius
            crystalRad[i],  //outer radius
            crystalHalfLength[i], //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
    cylbox[i] = new G4Box("cylbox1", //name
            width/2, width/2, crystalHalfLength[i]);
    cyl2[i] = new G4IntersectionSolid("cyl2",
            cyl1[i],cylbox[i],0,G4ThreeVector(offset,offset,0));
  
    //making the hole
    inCyl[i] = new G4Tubs("inCyl", //name
            0.*cm,          //inner radius
            holeRad[i],     //outer radius
            0.5*(holeDepth[i]-holeRad[i]), //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
    inSph[i] = new G4Orb("inSph",holeRad[i]);
    hole[i] = new G4UnionSolid("hole",
            inCyl[i],inSph[i],0,G4ThreeVector(0.,0.,.5*(holeDepth[i]-holeRad[i])));

    //make the dead layer
    dead1[i] = new G4Tubs("dead1", //name
            0.*cm,          //inner radius
            holeRad[i]+deadLayerThick[i],  //outer radius
            0.5*(holeDepth[i]-holeRad[i]), //z half length
            0.*deg,         //starting phi
            360.*deg);      //ending phi
    dead2[i] = new G4Orb("dead2",holeRad[i]+deadLayerThick[i]);
    dead3[i]= new G4UnionSolid("dead3",
            dead1[i],dead2[i],0,G4ThreeVector(0.,0.,.5*(holeDepth[i]-holeRad[i])));
    deadLayer[i] = new G4SubtractionSolid("deadLayer",
            dead3[i],hole[i],0,G4ThreeVector(0.,0.,0.));
    
    //dead layer
    logDeadLayer[i] = new G4LogicalVolume(deadLayer[i],
            crystalMaterial,    //material
            "logDeadLayer");

    physiDeadLayer[i] = new G4PVPlacement(rm[i],    //rotation
            deadPlacement[i],
            logDeadLayer[i],    //its logical volume
            "physiDeadLayer",   //its name
            logicWorld, //its mother  volume
            false,      //no boolean operation
            0);     //copy number
  
    //final detector shape
    activeCrystal[i] = new G4SubtractionSolid("solidCrystal",
            cyl2[i],dead3[i],0,G4ThreeVector(0.,0.,-crystalHalfLength[i]+0.5*holeDepth[i]-0.5*holeRad[i]));

    logCrystal[i] = new G4LogicalVolume(activeCrystal[i],
            crystalMaterial,    //material
            "logCrystal");

    physiCrystal[i] = new G4PVPlacement(rm[i],  //rotation
            CryPlacement[i],
            logCrystal[i],  //its logical volume
            "physiCrystal", //its name
            logicWorld, //its mother  volume
            false,      //no boolean operation
            0);     //copy number
  }
 
  //Make the outer shell
  //G4VSolid *shell1 = roundedBox("shell1", shellWidth, shellHalfLength, 1.5*cm);
  //G4VSolid *shell2 = roundedBox("shell2", shellWidth-2*wallThickness, 1.1*shellHalfLength, 1.5*cm);
  G4VSolid *shell1 = new G4Box("shell1", shellWidth/2., shellWidth/2., shellHalfLength);
  G4VSolid *shell2 = new G4Box("shell2", shellWidth/2. - wallThickness, shellWidth/2. - wallThickness, 
                               1.1*shellHalfLength);
  G4VSolid *outerShell = new G4SubtractionSolid("outerShell", shell1, shell2, 0, G4ThreeVector(0,0,0));
  logShell = new G4LogicalVolume(outerShell,
            wallMaterial,   //material
            "logShell");
  physiShell = new G4PVPlacement(rm[0], //rotation
                    G4ThreeVector(multx*(shellHalfLength+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()), //placement
                    logShell,   //its logical volume
                    "physiShell",   //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number


  //Make the window
  //G4VSolid *window = roundedBox("window",shellWidth, windowThickness, 1.5*cm);
  G4VSolid *window = new G4Box("window", shellWidth/2. - wallThickness, 
                               shellWidth/2. - wallThickness, windowThickness/2.);
  logWindow = new G4LogicalVolume(window,
            windowMaterial, //material
            "logWindow");
  physiWindow = new G4PVPlacement(rm[0],    //rotation
                    G4ThreeVector(multx*(.5*windowThickness+DetPos->x()),multy*DetPos->y(),multz*DetPos->z()),  //placement
                    logWindow,  //its logical volume
                    "physiWindow",  //its name
                    logicWorld, //its mother  volume
                    false,      //no boolean operation
                    0);     //copy number
            
  PrintCloverParameters();

  // Set solid angle
  G4double detectorDistance = sqrt(DetPos->x()*DetPos->x() + DetPos->y()*DetPos->y() 
      + DetPos->z()*DetPos->z());
  theta = pi - acos(detectorDistance*1./
  sqrt(shellWidth*shellWidth/4 + detectorDistance*detectorDistance));
  
  G4UImanager* UI = G4UImanager::GetUIpointer();
  std::stringstream strbld;
  
  strbld << "/gps/ang/mintheta " << theta << " rad";
  UI->ApplyCommand(strbld.str());
  strbld.str("");
  strbld.clear();
  
  strbld << "/gps/ang/rot1 0 0 -1";
  UI->ApplyCommand(strbld.str());
  
  
  // Visualization attributes
  // crysal
  for (int i=0; i<4;i++){
    {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,1.0,0.0,0.7));
     atb->SetForceSolid(true);
     logCrystal[i]->SetVisAttributes(atb);}

    {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,1.));
     atb->SetForceSolid(true);
     logDeadLayer[i]->SetVisAttributes(atb);}
   }
  
  // window
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.5));
  //{G4VisAttributes* atb= new G4VisAttributes(G4Colour(1.0,0.0,0.0,0.0));
   atb->SetForceSolid(true);
   logWindow->SetVisAttributes(atb);}

  // shell
  {G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.1));
  //{G4VisAttributes* atb= new G4VisAttributes(G4Colour(0.0,0.0,1.0,0.0));
   atb->SetForceSolid(true);
   logShell->SetVisAttributes(atb);}

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Clover::PrintCloverParameters(){
    
  // find volume and surface aread
  G4double activeVol[4], activeSA[4], holeVol[4], holeSA[4]; 
  G4double  deadLayerVol[4], deadLayerSA[4];

  for (int i=0; i<4; i++){
    activeVol[i] = activeCrystal[i]->GetCubicVolume();
    activeSA[i] = activeCrystal[i]->GetSurfaceArea();
    holeVol[i] = hole[i]->GetCubicVolume();
    holeSA[i] = hole[i]->GetSurfaceArea();
    deadLayerVol[i] = deadLayer[i]->GetCubicVolume();
    deadLayerSA[i]  = deadLayer[i]->GetSurfaceArea();
  }
  G4double solidangle = 2*pi*(1-cos(pi-theta));

  // print
  G4cout << "\n------------------------------------------------------------"
         << "\n--->  " << name << "  <---\n"
         << "\n---> The crysyal is " << crystalMaterial->GetName() << "\n";
         for (int i=0; i<4; i++){
           G4cout << "--- Crystal " << i+1 << "---\n"
           << crystalRad[i]/mm << "mm outer radius \n"
           << 2.*crystalHalfLength[i]/mm << "mm length \n" 
           << holeRad[i]/mm << "mm hole radius \n" 
           << holeDepth[i]/mm << "mm hole depth \n"
           << deadLayerThick[i]/mm << "mm thickness of dead layer\n";
         }
         G4cout << "---> Shielding properties \n"
         << 2.*shellHalfLength/mm << "mm shell length \n" 
         << wallThickness/mm << "mm wall of " << wallMaterial->GetName() << "\n" 
         << endGap/mm << "mm end gap \n" 
         << windowThickness/mm << "mm window of " << windowMaterial->GetName() << "\n"
         << "--->Calculated quantities \n";
         for (int i=0; i<4; i++){
           G4cout << "--- Crystal " << i+1 << "---\n"
           << holeVol[i]/(cm*cm*cm) << " cm^3 hole volume \n"
           << holeSA[i]/(cm*cm) << " cm^2 hole surface area \n"
           << activeVol[i]/(cm*cm*cm) << " cm^3 Active volume \n"
           << activeSA[i]/(cm*cm) << " cm^2 Active surface area \n"
           << deadLayerVol[i]/(cm*cm*cm) << " cm^3 dead layer volume \n"
           << deadLayerSA[i]/(cm*cm) << " cm^2 dead layer surface area \n";
         }
         G4cout << theta << " theta \n"
         << solidangle << " /4*pi solid angle\n"
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// this function should make a box with rounded edges.  I have had problems getting it to work and dropped it
G4VSolid* Clover::roundedBox(G4String name, G4double height, G4double width, G4double curv)
{
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
   
   G4VSolid *box1 = new G4Box("box1",      //name  
                               height/2, y, width/2);  //size
   G4VSolid *box2 = new G4Box("box2",      //name  
                               x, height/2, width/2);  //size
   G4VSolid *box  = new G4UnionSolid("box", box1, box2, 0, G4ThreeVector(0,0,0));

   for (int i=0; i<4; i++){
       rm[i] = new G4RotationMatrix();
       edge[i] = new G4Tubs("edge"+i, //name
           0.*cm,      //inner radius
           curv,   //outer radius
           width/2,    //z half length
           0.*deg,     //starting phi
           90.*deg);   //ending phi
       rm[i]->rotateZ(i*90.*deg);

       if (i==0){
         rounded[i] = new G4UnionSolid("rounded"+i, box, edge[i], rm[i], corner[i]);
       }
       else{
         rounded[i] = new G4UnionSolid("rounded"+i, rounded[i-1], edge[i], rm[i], corner[i]);
       }
   }
   rounded[3]->SetName(name);

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

void Clover::SetDeadLayerThick(int seg, G4double val){
  if (seg>=0 || seg<4){ 
    deadLayerThick[seg] = val;
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

void Clover::SetShellWidth(G4double val){
  shellWidth = val;
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

