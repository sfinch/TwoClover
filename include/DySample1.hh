//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DySample1.hh
// makes the enriched 156Dy sample
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DySample1_h
#define DySample1_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DySample1
{
  public:

    DySample1(G4String name);
    ~DySample1();

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
      
     const G4VPhysicalVolume* GetSample1()   {return physSample1;};

  private:

     // object properties
     G4String           name;
     G4LogicalVolume*   logicWorld;
     G4ThreeVector      SamplePos;
     G4RotationMatrix*  SampleRot;
     
     G4Material*        defaultMaterial;
     
     G4double           sample1Thick;
     G4double           sample1Width;
     G4double           sample1Height;
     G4double           sample2Thick;
     G4double           sample2Width;
     G4double           sample2Height;

     G4double           bagThickL;
     G4double           bagThickR;

     G4Material*        sampleMaterial1;
     G4Material*        sampleMaterial2;
     G4Material*        bagMaterial;

     G4VSolid *sample1;                  //Dy sample1
     G4LogicalVolume *logSample1;        //pointer to the logical Dy sample 1
     G4VPhysicalVolume *physSample1;     //pointer to the physical Dy sample 1

     G4VSolid *bagL;                     //bag
     G4LogicalVolume *logBagL;           //pointer to the logical bag
     G4VPhysicalVolume *physBagL;        //pointer to the physical bag

     G4VSolid *bagR;                     //bag
     G4LogicalVolume *logBagR;           //pointer to the logical bag
     G4VPhysicalVolume *physBagR;        //pointer to the physical bag

  private:
    
     void ComputeDySampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DySample1::ComputeDySampleParameters()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

