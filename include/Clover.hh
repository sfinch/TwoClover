//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef Clover_h
#define Clover_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class CloverMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class Clover
{
  public:
  
    Clover(G4String name);
   ~Clover();

  public:

   	 void SetName(G4String);
     
     void SetCrystalMaterial (G4String);     
     void SetWindowMaterial (G4String);     
     void SetWallMaterial (G4String);     

     void SetCrystalHalfLength(int, G4double);
     void SetCrystalRad(int, G4double);
     void SetCrystalEndRad(int, G4double);
     void SetHoleRad(int, G4double);
     void SetHoleDepth(int, G4double);
     void SetDeadLayerThick(int, G4double);

     void SetShellHalfLength(G4double);
     void SetEndGap(G4double);
     void SetWindowThickness(G4double);
     void SetWallThickness(G4double);

     void BuildClover(G4LogicalVolume *logWorld,
					 G4ThreeVector *pos,
					 G4RotationMatrix *rot);
     void PrintCloverParameters();
     
  public:
  
     G4String GetName()					{return name;};

     G4double GetCrystalHalfLength(int seg);
     G4double GetCrystalRad(int seg);
     G4double GetCrystalEndRad(int seg);
     G4double GetHoleRad(int seg);
     G4double GetHoleDepthRad(int seg);
     G4double GetDeadLayerThick(int seg);
      
     G4double GetShellHalfLength()		{return shellHalfLength;}; 
     G4double GetEndGap()				{return endGap;};
     G4double GetWindowThickness()		{return windowThickness;};
     G4double GetWallThickness()		{return wallThickness;};
      
     G4Material* GetCrystalMaterial()   {return crystalMaterial;};
     G4Material* GetWindowMaterial()    {return windowMaterial;};
     G4Material* GetWallMaterial()      {return wallMaterial;};
     
     const G4VPhysicalVolume* GetCrystal()   {return physiCrystal[0];};
                 
  private:

     G4String	 	    name;
     G4LogicalVolume*   logicWorld;
   	 G4ThreeVector*		DetPos;
   	 G4RotationMatrix*	DetRot;
     
     G4Material*        crystalMaterial;
     G4Material*        windowMaterial;
     G4Material*        wallMaterial;
     
     G4double           crystalHalfLength[4];
     G4double           crystalRad[4];
     G4double           crystalEndRad[4];
     G4double           holeRad[4];
     G4double           holeDepth[4];
     G4double           deadLayerThick[4];

     G4double           shellHalfLength;
     G4double           endGap;
     G4double           windowThickness;
     G4double           wallThickness;
     
     G4Material*        defaultMaterial;
            
	 G4VSolid *activeCrystal[4];
	 G4VSolid *deadLayer[4];
	 G4VSolid *hole[4];

     G4LogicalVolume *logCrystal[4];    //pointer to the logical crystal 
     G4LogicalVolume *logDeadLayer[4];    //pointer to the logical dead layer
     G4VPhysicalVolume *physiCrystal[4];    //pointer to the physical crystal 
     G4VPhysicalVolume *physiDeadLayer[4];    //pointer to the physical dead layer

     G4LogicalVolume *logShell;    //pointer to the logical shell 
     G4VPhysicalVolume *physiShell;    //pointer to the physical shell 
     
     G4LogicalVolume *logWindow;        //pointer to the logical shell 
     G4VPhysicalVolume *physiWindow;    //pointer to the physical shell 

	 CloverMessenger* cloverMessenger;
     
  private:
    
     void ComputeCloverParameters();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void Clover::ComputeCloverParameters()
{
	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetCrystalHalfLength(int seg)		
{
  if (seg>=0 || seg<4){ 
	return crystalHalfLength[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetCrystalRad(int seg)
{
  if (seg>=0 || seg<4){ 
	return crystalRad[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetCrystalEndRad(int seg)
{
  if (seg>=0 || seg<4){ 
	return crystalEndRad[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetHoleRad(int seg)
{
  if (seg>=0 || seg<4){ 
	return holeRad[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetHoleDepthRad(int seg)
{
  if (seg>=0 || seg<4){ 
	return holeDepth[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetDeadLayerThick(int seg)
{
  if (seg>=0 || seg<4){ 
	return deadLayerThick[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

