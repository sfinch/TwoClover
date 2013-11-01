//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DySample.hh
// makes the enriched 156Dy sample
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DySample_h
#define DySample_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DySample 
{
  public:

    DySample(G4String name);
    ~DySample();

  public:

     // setters
     void SetName(G4String);
     
     // functions
     void BuildDySample(G4LogicalVolume *logWorld,
                     G4ThreeVector *pos,
                     G4RotationMatrix *rot);
     void PrintDySampleParameters();
     
  public:
  
     // getters
     G4String GetName()                 {return name;};
      
     const G4VPhysicalVolume* GetSample()   {return physSample;};

  private:

     // object properties
     G4String           name;
     G4LogicalVolume*   logicWorld;
     G4ThreeVector      SamplePos;
     G4RotationMatrix*  SampleRot;
     
     G4Material*        sampleMaterial;
     G4Material*        defaultMaterial;
     
     G4double           sampleThick;
     G4double           sampleWidth;
     G4double           sampleHeight;

     G4VSolid *sample;                  //Inner Dy disk
     G4LogicalVolume *logSample;        //pointer to the logical inner disk Dy
     G4VPhysicalVolume *physSample;     //pointer to the physical inner disk Dy 

  private:
    
     void ComputeDySampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DySample::ComputeDySampleParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

