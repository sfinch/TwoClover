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
#include <map>

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
    void SetRndmFlag(G4String val)   {rndmFlag = val;}
    void SetPositionX(G4double X)    {positionX = X;}
    void SetPositionY(G4double Y)    {positionY = Y;}
    void SetPositionZ(G4double Z)    {positionZ = Z;}
    void SetNumGamma(int num);
    void SetEnergy(int num, G4double En);
    void SetSpin(int num, int j);
    void SetEnSpin(int num, G4double En, int j);
  
    // function
    void GeneratePrimaries(G4Event*);

    void PrintGunParameters();

    G4ThreeVector randP();
    G4ThreeVector randMultipole(G4ThreeVector, int, int, int);

  private:
    double pi;

    // object properties
    G4ParticleGun*           particleGun;  //pointer a to G4  class
    DetectorConstruction*    Detector;     //pointer to the geometry

    G4double sampleWidth1, sampleWidth2;
    G4double sampleHeight1, sampleHeight2;
    G4double sampleThick1, sampleThick2;

    G4double sampleStart1, sampleStart2;
      
    PrimaryGeneratorMessenger* gunMessenger;   //messenger for this class
    G4String                   rndmFlag;       //flag for switching between gps and gun

    G4int          numGamma;       // max = 4
    G4double       positionX;
    G4double       positionY;
    G4double       positionZ;
    G4double       energy[4];
    G4double       spin[5];
    
    int            sampNum;
  
    std::map<int,TF1*> fPDF;

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

inline void PrimaryGeneratorAction::SetSpin(int num, int j)
{ 
  if ((num>=0)&&(num<5)){
    spin[num] = j;
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void PrimaryGeneratorAction::SetEnSpin(int num, G4double En, int j)
{ 
  if ((num>=0)&&(num<4)){
    energy[num] = En;
    spin[num] = j;
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


