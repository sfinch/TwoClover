//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/PrimaryGeneratorAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include "TF1.h"

class G4ParticleGun;
class G4GeneralParticleSource;
class G4Event;
class DetectorConstruction;
class PrimaryGeneratorMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    PrimaryGeneratorAction(DetectorConstruction*);    
    virtual ~PrimaryGeneratorAction();
  
    // setters
    void SetRndmFlag(G4String val)   { rndmFlag = val;}
    void SetNumGamma(int num);
    void SetPositionR(G4double R)    { positionR = R;}
    void SetEnergy(int num, G4double En);
  
    // function
    void GeneratePrimaries(G4Event*);
    G4ThreeVector randP();
    G4ThreeVector randE2(G4ThreeVector);
    G4ThreeVector randE1(G4ThreeVector);

  private:
    // object properties
    G4ParticleGun*           particleGun;  //pointer a to G4  class
    DetectorConstruction*    Detector;     //pointer to the geometry
      
    PrimaryGeneratorMessenger* gunMessenger;   //messenger for this class
    G4String                   rndmFlag;       //flag for switching between gps and gun

    G4int          numGamma;       // max = 4
    G4double       positionR;
    G4double       energy[4];
  
    TF1 *fPDF020;
    TF1 *fPDF420;
    TF1 *fPDF010;
  
    G4GeneralParticleSource* particleSource;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetEnergy(int num, G4double En)
{ 
  if ((num>=0)&&(num<4)){
    energy[num] = En;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetNumGamma(int num)
{
  if (num<1)
    num=1;
  else if (num>4)
    num = 4;
  
  numGamma = num;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


