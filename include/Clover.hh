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
     void SetCrystalInOffset(int, G4double);
     void SetCrystalOutOffset(int, G4double);
     void SetCrystalSeparation(int, G4double);
     void SetHoleRad(int, G4double);
     void SetHoleDepth(int, G4double);

     void SetShellHalfLength(G4double);
     void SetShellWidth(G4double);
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
     G4double GetCrystalInOffset(int seg);
     G4double GetCrystalOutOffset(int seg);
     G4double GetCrystalSeparation(int seg);
     G4double GetHoleRad(int seg);
     G4double GetHoleDepthRad(int seg);
      
     G4double GetShellHalfLength()		{return shellHalfLength;}; 
     G4double GetShellWidth()			{return shellWidth;}; 
     G4double GetEndGap()				{return endGap;};
     G4double GetWindowThickness()		{return windowThickness;};
     G4double GetWallThickness()		{return wallThickness;};
      
     G4Material* GetCrystalMaterial()   {return crystalMaterial;};
     G4Material* GetWindowMaterial()    {return windowMaterial;};
     G4Material* GetWallMaterial()      {return wallMaterial;};
     
     const G4VPhysicalVolume* GetCrystal(int seg);

	 G4VSolid* roundedBox(G4String, G4double, G4double, G4double);
                 
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

     G4double           crystalInOffset[4];
     G4double           crystalOutOffset[4];
     G4double           crystalSeparation[4];
     G4double           holeRad[4];
     G4double           holeDepth[4];

     G4double           shellHalfLength;
     G4double           shellWidth;
     G4double           endGap;
     G4double           windowThickness;
     G4double           wallThickness;

	 G4double 			theta;
     
     G4Material*        defaultMaterial;
            
	 G4VSolid *activeCrystal[4];
	 G4VSolid *hole[4];

     G4LogicalVolume *logCrystal[4];    //pointer to the logical crystal 
     G4VPhysicalVolume *physiCrystal[4];    //pointer to the physical crystal 

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

inline G4double Clover::GetCrystalInOffset(int seg)
{
  if (seg>=0 || seg<4){ 
	return crystalInOffset[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline G4double Clover::GetCrystalOutOffset(int seg)
{
  if (seg>=0 || seg<4){ 
	return crystalOutOffset[seg];
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

inline G4double Clover::GetCrystalSeparation(int seg)
{
  if (seg>=0 || seg<4){ 
	return crystalSeparation[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline const G4VPhysicalVolume* Clover::GetCrystal(int seg)
{
  if (seg>=0 || seg<4){ 
    return physiCrystal[seg];
  }
  else
  	return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

