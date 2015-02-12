//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/PrimaryGeneratorAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include <map>
#include <stdexcept> 

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4GeneralParticleSource.hh"

#include "TF1.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
:Detector(DC),rndmFlag("on")
{

  pi = 3.141592;
  sampleThick1 = 0;
  sampleThick2 = 0;
  sampleStart1 = 0;
  sampleStart2 = 0;
  sampNum = DC->GetSampNum();
  if (sampNum > 0){
    sampleWidth1 = DC->GetDySample2()->GetSample1Width();
    sampleHeight1= DC->GetDySample2()->GetSample1Height();
    sampleThick1 = DC->GetDySample2()->GetSample1Thick();
    sampleWidth2 = DC->GetDySample2()->GetSample2Width();
    sampleHeight2= DC->GetDySample2()->GetSample2Height();
    sampleThick2 = DC->GetDySample2()->GetSample2Thick();

    sampleStart1 = DC->GetDySample2()->GetSample1Pos().x();
    sampleStart2 = DC->GetDySample2()->GetSample2Pos().x();
  }
  
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);
  for (int i=0; i<4; i++){
    energy[i] = 500*keV;
  }
  for (int i=0; i<5; i++){
    spin[i] = 0;
  } 
  // Particle gun
  numGamma = 2;
  positionX = 0*cm;
  positionY = 0*cm;
  positionZ = 0*cm;
  if (numGamma == 2){
    // 102Ru
    /*spin[0] = 0;
    spin[1] = 2;
    energy[0] = 468.58*keV;
    energy[1] = 475.06*keV;*/
    // 156Dy 1946
    spin[0] = 1;
    spin[1] = 2;
    energy[0] = 1857.4*keV;
    energy[1] = 88.97*keV;
    // 156Dy 1952
    /*spin[0] = 0;
    spin[1] = 1;
    energy[0] = 709.9*keV;
    energy[1] = 1242.*keV;*/
    // 156Dy 2003
    /*spin[0] = 0;
    spin[1] = 2;
    energy[0] = 1899.5*keV;
    energy[1] = 88.9*keV;*/
  }
  else if (numGamma == 3){
  }

  fPDF[20] = new TF1("fPDF020","1.-3.*cos(x)**2+4.*cos(x)**4",0.,pi);
  fPDF[10] = new TF1("fPDF010","1.+cos(x)**2",0.,pi);

  fPDF[120] = new TF1("fPDF120","1.-(1./3.)*cos(x)**2",0.,pi);

  fPDF[220] = new TF1("fPDF220","1.+0.42856*cos(x)**2",0.,pi);

  fPDF[342] = new TF1("fPDF342","1.-0.19696*cos(x)**2",0.,pi);
  fPDF[322] = new TF1("fPDF320","1.-0.07314*cos(x)**2",0.,pi);
  fPDF[320] = new TF1("fPDF320","1.-0.10318*cos(x)**2",0.,pi);

  fPDF[444] = new TF1("fPDF444","1.+0.31965*cos(x)**2",0.,pi);
  fPDF[442] = new TF1("fPDF542","1.+(1./3.)*cos(x)**2",0.,pi);
  fPDF[434] = new TF1("fPDF434","1.+0.02906*cos(x)**2+0.00261*cos(x)**4",0.,pi);
  fPDF[432] = new TF1("fPDF432","1.+0.0769*cos(x)**2",0.,pi);
  fPDF[422] = new TF1("fPDF422","1.+0.1107*cos(x)**2",0.,pi);
  fPDF[420] = new TF1("fPDF420","1.-(1./8.)*cos(x)**2+(1./24.)*cos(x)**4",0.,pi);

  fPDF[544] = new TF1("fPDF544","1.-0.1019*cos(x)**2",0.,pi);
  fPDF[543] = new TF1("fPDF543","1.+0.07683*cos(x)**2",0.,pi);
  fPDF[542] = new TF1("fPDF542","1.-0.10319*cos(x)**2",0.,pi);
  fPDF[534] = new TF1("fPDF534","1.-0.04454*cos(x)**2",0.,pi);
  fPDF[532] = new TF1("fPDF532","1.-0.10318*cos(x)**2",0.,pi);

  particleGun  = new G4ParticleGun(1);
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(energy[0]);
  particleGun->SetParticlePosition(G4ThreeVector(positionX,positionY,positionZ));

  // Particle source
  particleSource = new G4GeneralParticleSource();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { 
  delete particleGun;
  delete gunMessenger;
  delete particleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  
  // custom event generator
  if (rndmFlag == "off"){
    G4ThreeVector p[5];
    p[0] = randP();

    //randomize position in sample (if wanted)
    if (sampNum == 1){
        G4double posX = sampleStart1 + sampleThick1*(G4UniformRand()-0.5);
        G4double posY = sampleHeight1*(G4UniformRand()-0.5);
        G4double posZ = sampleWidth1*(G4UniformRand()-0.5);
        particleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ));
    }
    else if (sampNum == 2){
        double wichSamp = G4UniformRand();
        G4double posX, posY, posZ;
        if (wichSamp<.7068){
            posX = sampleStart1 + sampleThick1*(G4UniformRand()-.5);
            posY = sampleHeight1*(G4UniformRand()-0.5);
            posZ = sampleWidth1*(G4UniformRand()-0.5);
        }
        else{
            posX = sampleStart2 + sampleThick2*(G4UniformRand()-.5);
            posY = sampleHeight2*(G4UniformRand()-0.5);
            posZ = sampleWidth2*(G4UniformRand()-0.5);
        }
        particleGun->SetParticlePosition(G4ThreeVector(posX,posY,posZ));
    }
    else{
        particleGun->SetParticlePosition(G4ThreeVector(positionX,positionY,positionZ));
    }

    //calculate multipole radiation angular distribution
    for (int i=1; i<numGamma; i++){
      p[i] = randMultipole(p[i-1],spin[i-1], spin[i], spin[i+1]);
    }

    //start the gun
    for (int i=0; i<numGamma; i++){
      particleGun->SetParticleMomentumDirection(p[i]);
      particleGun->SetParticleEnergy(energy[i]);
      particleGun->GeneratePrimaryVertex(anEvent);
    }
  }

  // GPS
  if (rndmFlag == "on"){
    particleSource->GeneratePrimaryVertex(anEvent);
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::randP(){

  //isotropic source 
  double P = 0.;
  double px, py, pz;
  do{
    px = (2*G4UniformRand())-1;
    py = (2*G4UniformRand())-1;
    pz = (2*G4UniformRand())-1;
    P = px*px+py*py+pz*pz;
  }while(P>1);
  P = sqrt(P);
  px = px/P;
  py = py/P;
  pz = pz/P;

  return G4ThreeVector(px,py,pz);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreeVector PrimaryGeneratorAction::randMultipole(G4ThreeVector P, int m1, int m2, int m3)
{

  G4double theta = P.getTheta();
  G4double phi = P.getPhi();

  double theta2 = 0;
  double phi2 = 2*3.141592*G4UniformRand();
  int decay = m1*100 + m2*10 + m3;
  try{
    theta2 = fPDF.at(decay)->GetRandom();
  }
  catch(const std::out_of_range& oor) {
    G4cout << "ERROR: W(" << decay << ") is not included! Using isotropic distribution." << G4endl;
    return randP();
  }

  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateY(theta);
  rot->rotateZ(phi);

  G4ThreeVector p2 (1,0,0);
  p2.setTheta(theta2);
  p2.setPhi(phi2);
  p2.setMag(1.);

  p2.transform(*rot);
  delete rot;

  //G4cout << "Theta should be " << std::cos(theta2) << ": " << e2.dot(P) << G4endl;

  return p2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::PrintGunParameters(){
  G4cout << "\n------------------------------------------------------------"
         << "\n---> Rndm flag (on is GPS, off is custom): " <<  rndmFlag  
         << "\n---> X Position:       " << positionX/cm << " cm"
         << "\n---> Y Position:       " << positionY/cm << " cm"
         << "\n---> Z Position:       " << positionZ/cm << " cm"
         << "\n---> Sample 1 Thick:     " << sampleWidth1/cm << " cm"
         << "\n---> Sample 2 Thick:     " << sampleThick2/cm << " cm"
         << "\n---> Number of gammas: " << numGamma << ""
         << "\n---> Level scheme: " << "\n";
  for (int i=0; i<numGamma; i++){
    G4cout << "       ----- " << spin[i] << "\n"
           << "        | " << energy[i]/keV << " keV\n";
  }
  G4cout << "       ----- " << spin[numGamma]
         << "\n------------------------------------------------------------\n";
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
