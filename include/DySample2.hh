//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/DySample2.hh
// makes the enriched 156Dy sample
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef DySample2_h
#define DySample2_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4Box;
class G4VSolid;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class DySample2
{
  public:

    DySample2(G4String name);
    ~DySample2();

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
     const G4VPhysicalVolume* GetSample2()   {return physSample2;};
     G4double GetSample1Thick()       {return sample1Thick;};
     G4double GetSample1Width()       {return sample1Width;};
     G4double GetSample1Height()      {return sample1Height;};
     G4double GetSample2Thick()       {return sample2Thick;};
     G4double GetSample2Width()       {return sample2Width;};
     G4double GetSample2Height()      {return sample2Height;};
     G4ThreeVector GetSample1Pos()    {return SamplePos1;};
     G4ThreeVector GetSample2Pos()    {return SamplePos2;};

  private:

     // object properties
     G4String           name;
     G4LogicalVolume*   logicWorld;
     G4ThreeVector      SamplePos1;
     G4ThreeVector      SamplePos2;
     G4ThreeVector      bagPosL;
     G4ThreeVector      bagPosC;
     G4ThreeVector      bagPosR;
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
     G4double           bagThickC;

     G4double           mid;

     G4Material*        sampleMaterial1;
     G4Material*        sampleMaterial2;
     G4Material*        bagMaterial;

     G4VSolid *sample1;                  //Dy sample1
     G4LogicalVolume *logSample1;        //pointer to the logical Dy sample 1
     G4VPhysicalVolume *physSample1;     //pointer to the physical Dy sample 1

     G4VSolid *sample2;                  //Dy sample1
     G4LogicalVolume *logSample2;        //pointer to the logical Dy sample 1
     G4VPhysicalVolume *physSample2;     //pointer to the physical Dy sample 1

     G4VSolid *bagL;                     //bag
     G4LogicalVolume *logBagL;           //pointer to the logical bag
     G4VPhysicalVolume *physBagL;        //pointer to the physical bag

     G4VSolid *bagR;                     //bag
     G4LogicalVolume *logBagR;           //pointer to the logical bag
     G4VPhysicalVolume *physBagR;        //pointer to the physical bag

     G4VSolid *bagC;                     //bag
     G4LogicalVolume *logBagC;           //pointer to the logical bag
     G4VPhysicalVolume *physBagC;        //pointer to the physical bag

  private:
    
     void ComputeDySampleParameters();
     void DefineMaterials();
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void DySample2::ComputeDySampleParameters()
{
  G4double mid = (bagThickL + bagThickR + bagThickC + sample1Thick + sample2Thick)/2;
  bagPosL = G4ThreeVector(bagThickL/2-mid,0,0);
  SamplePos2 = G4ThreeVector(sample2Thick/2-mid+bagThickL,0,0);
  bagPosC = G4ThreeVector(bagThickC/2-mid+bagThickL+sample2Thick,0,0);
  SamplePos1 = G4ThreeVector(sample1Thick/2-mid+bagThickL+sample2Thick+bagThickC,0,0);
  bagPosR = G4ThreeVector(mid-bagThickR/2,0,0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

