//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/NaIAnnulus.hh
// makes the NaI annulus for the detector
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef NaIAnnulus_h
#define NaIAnnulus_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class NaIAnnulusMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class NaIAnnulus
{
  public:
  
    NaIAnnulus(G4String name);
    ~NaIAnnulus();

  public:

     //setters
     void SetName(G4String);
     
     void SetCrystalMaterial (G4String);     
     void SetWallMaterial (G4String);     

     void SetCrystalHalfLength(G4double);
     void SetCrystalInRad(G4double);
     void SetCrystalOutRad(G4double);

     void SetWallInThickness(G4double);
     void SetWallOutThickness(G4double);

     // functions
     void BuildNaIAnnulus(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot);
     void PrintNaIAnnulusParameters();
     
  public:
  
     // getters
     G4String GetName()                 {return name;};
      
     G4Material* GetCrystalMaterial()   {return crystalMaterial;};
     G4Material* GetWallMaterial()      {return wallMaterial;};

     G4double GetCrystalHalfLength()    {return crystalHalfLength;};
     G4double GetCrystalInRad()         {return crystalInRad;}; 
     G4double GetCrystalOutRad()        {return crystalOutRad;};
      
     G4double GetWallInThickness()      {return wallInThickness;};
     G4double GetWallOutThickness()     {return wallOutThickness;};
     
     const G4VPhysicalVolume* GetCrystal()   {return physCrystal;};
                 
  private:

     // object properties
     G4String           name;
     G4LogicalVolume*   logicWorld;
     G4ThreeVector*     DetPos;
     G4RotationMatrix*  DetRot;
     
     G4Material*        crystalMaterial;
     G4Material*        wallMaterial;
     
     G4double           crystalHalfLength;
     G4double           crystalInRad;
     G4double           crystalOutRad;

     G4double           wallInThickness;
     G4double           wallOutThickness;
     
     G4Material*        defaultMaterial;
            

     G4VSolid *crystal;                 //NaI Crystal
     G4LogicalVolume *logCrystal;       //pointer to the logical crystal 
     G4VPhysicalVolume *physCrystal;    //pointer to the physical crystal 

     G4VSolid *inWall;                  //Inner Wall
     G4LogicalVolume *logInWall;        //pointer to the logical wall
     G4VPhysicalVolume *physInWall;     //pointer to the physical wall

     G4VSolid *outWall;                 //Outer wall
     G4LogicalVolume *logOutWall;       //pointer to the logical wall
     G4VPhysicalVolume *physOutWall;    //pointer to the physical wall

     NaIAnnulusMessenger* NaIMessenger;
     
  private:
    
     void ComputeNaIAnnulusParameters();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void NaIAnnulus::ComputeNaIAnnulusParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

