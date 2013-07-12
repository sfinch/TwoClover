//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "Clover.hh"
#include "NaIAnnulus.hh"
#include "DySample.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class DetectorMessenger;

class Clover;
class NaIAnnulus;
class DySample;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
  
    DetectorConstruction();
   ~DetectorConstruction();

  public:
     
     void SetDetectorDistance (G4double);     

     G4VPhysicalVolume* Construct();

     void UpdateGeometry();
     
  public:
  
     void PrintTwoCloverParameters(); 
                    
     G4double GetWorldSizeX()           {return WorldSizeX;}; 
     G4double GetWorldSizeYZ()          {return WorldSizeYZ;};
     
     G4double GetDetectorDistance()     {return detectorDistance;};
     Clover* GetClover(G4int det);
     NaIAnnulus* GetNaIAnnulus()        {return NaIDet;};
     DySample* GetDySample()            {return DySamp;};

     const G4VPhysicalVolume* GetphysiWorld() {return physiWorld;};           
                 
  private:
     
     G4double           detectorDistance;
     Clover*              CloverDet[2];
     NaIAnnulus*        NaIDet;
     DySample*          DySamp;

     G4Material*        defaultMaterial;
     G4double           WorldSizeYZ;
     G4double           WorldSizeX;
            
     G4Box*             solidWorld;    //pointer to the solid World 
     G4LogicalVolume*   logicWorld;    //pointer to the logical World
     G4VPhysicalVolume* physiWorld;    //pointer to the physical World

     DetectorMessenger* detectorMessenger;  //pointer to the Messenger
      
  private:
    
     void DefineMaterials();
     void ComputeTwoCloverParameters();
     G4VPhysicalVolume* ConstructTwoClover();     
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DetectorConstruction::ComputeTwoCloverParameters()
{
  G4double maxRad = NaIDet->GetCrystalOutRad() + NaIDet->GetWallOutThickness();
  G4double maxLength = NaIDet->GetCrystalHalfLength();
  G4double maxLength2 = detectorDistance + 2*CloverDet[1]->GetShellHalfLength();
  if (maxLength2 > maxLength){
    maxLength = maxLength2;
  }

  WorldSizeX  = 2.2*maxLength;
  WorldSizeYZ = 2.2*maxRad;

  //WorldSizeX  = 55.*cm;
  //WorldSizeYZ = 40.*cm;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline Clover* DetectorConstruction::GetClover(G4int det)
{
  if (det==0 || det==1){ 
    return CloverDet[det];
  }
  else
    return NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

