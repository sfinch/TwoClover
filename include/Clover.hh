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

     void SetCrystalHalfLength(G4double);
     void SetCrystalRad(G4double);
     void SetCrystalEndRad(G4double);
     void SetHoleRad(G4double);
     void SetHoleDepth(G4double);
     void SetDeadLayerThick(G4double);

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

     G4double GetCrystalHalfLength()	{return crystalHalfLength;};
     G4double GetCrystalRad()			{return crystalRad;}; 
     G4double GetCrystalEndRad()		{return crystalEndRad;};
     G4double GetHoleRad()				{return holeRad;};
     G4double GetHoleDepthRad()			{return holeDepth;};
     G4double GetDeadLayerThick()		{return deadLayerThick;};
      
     G4double GetShellHalfLength()		{return shellHalfLength;}; 
     G4double GetEndGap()				{return endGap;};
     G4double GetWindowThickness()		{return windowThickness;};
     G4double GetWallThickness()		{return wallThickness;};
      
     G4Material* GetCrystalMaterial()   {return crystalMaterial;};
     G4Material* GetWindowMaterial()    {return windowMaterial;};
     G4Material* GetWallMaterial()      {return wallMaterial;};
     
     const G4VPhysicalVolume* GetCrystal()   {return physiCrystal;};
                 
  private:

     G4String	 	    name;
     G4LogicalVolume*   logicWorld;
   	 G4ThreeVector*		DetPos;
   	 G4RotationMatrix*	DetRot;
     
     G4Material*        crystalMaterial;
     G4Material*        windowMaterial;
     G4Material*        wallMaterial;
     
     G4double           crystalHalfLength;
     G4double           crystalRad;
     G4double           crystalEndRad;
     G4double           holeRad;
     G4double           holeDepth;
     G4double           deadLayerThick;

     G4double           shellHalfLength;
     G4double           endGap;
     G4double           windowThickness;
     G4double           wallThickness;
     
     G4Material*        defaultMaterial;
            
	 G4VSolid *activeCrystal;
	 G4VSolid *deadLayer;
	 G4VSolid *hole;

     G4LogicalVolume *logCrystal;    //pointer to the logical crystal 
     G4LogicalVolume *logDeadLayer;    //pointer to the logical dead layer
     G4VPhysicalVolume *physiCrystal;    //pointer to the physical crystal 
     G4VPhysicalVolume *physiDeadLayer;    //pointer to the physical dead layer

     G4LogicalVolume *logShell;    //pointer to the logical shell 
     G4VPhysicalVolume *physiShell;    //pointer to the physical shell 
     
     G4LogicalVolume *logWindow;    //pointer to the logical shell 
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

#endif

