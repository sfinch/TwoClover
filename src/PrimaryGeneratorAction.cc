//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/PrimaryGeneratorAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorMessenger.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "TF1.h"
#include "G4GeneralParticleSource.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
//:Detector(DC),rndmFlag("off")
:Detector(DC),rndmFlag("on")
{

  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);

  // Particle gun
  numGamma = 2;
  positionR = 0*cm;
  // 102Ru
  energy[0] = 468.58*keV;
  energy[1] = 475.06*keV;
  // 96Zr
  //energy[0] = 369.8*keV;
  //energy[1] = 778.23*keV;
  // 150Nd 
  //energy[0] = 406.51*keV;
  //energy[1] = 333.96*keV;
  // 96Nb
  //energy[0] = 568.9*keV;  //5 -> 4
  //energy[1] = 241.4*keV;  //4 -> 4
  //energy[2] = 850*keV;    //4 -> 2
  //energy[3] = 778.2*keV;  //2 -> 0
  // 96Nb
  //energy[0] = 568.9*keV;  //5 -> 4
  //energy[1] = 371.7*keV;  //4 -> 2
  //energy[2] = 1497.9*keV; //2 -> 0

  energy[2] = 1*MeV;
  energy[3] = 1*MeV;

  double pi = 3.141592;
  fPDF020 = new TF1("fPDF020","1.-3.*cos(x)**2+4.*cos(x)**4",0.,pi);             //0 -> 2 -> 0
  fPDF420 = new TF1("fPDF420","1.-(1./8.)*cos(x)**2+(1./24.)*cos(x)**4",0.,pi);  //4 -> 2 -> 0
  fPDF010 = new TF1("fPDF010","1.+cos(x)**2",0.,pi);                             //0 -> 1 -> 0
  fPDF120 = new TF1("fPDF120","1.-(1./3.)*cos(x)**2",0.,pi);                       //1 -> 2 -> 0
  fPDF544 = new TF1("fPDF544","1.+0.1019*cos(x)**2",0.,pi);                      //5 -> 4 -> 4
  fPDF542 = new TF1("fPDF542","1.+0.10319*cos(x)**2",0.,pi);                     //5 -> 4 -> 2
  fPDF442 = new TF1("fPDF542","1.+(1./3.)*cos(x)**2",0.,pi);                     //5 -> 4 -> 2


  particleGun  = new G4ParticleGun(1);
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(energy[0]);
  particleGun->SetParticlePosition(G4ThreeVector(0*cm,positionR,0.*cm));

  // Particle source
  particleSource = new G4GeneralParticleSource();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete particleGun;
  delete gunMessenger;
  delete particleSource;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event

  if (rndmFlag == "off"){
    G4ThreeVector p[5];
    p[0] = randP();
    //p[0] = G4ThreeVector(1,0,0);  // straight gamma
    particleGun->SetParticlePosition(G4ThreeVector(0*cm,positionR,0.*cm));
    //particleGun->SetParticlePosition(G4ThreeVector(0*cm,2.5*cm,-2.5*cm)); // change position

    if (numGamma==1){

    }
    else if (numGamma==2){
      p[1] = randMultipole(p[0],020);
      //p[1] = randMultipole(p[0],010);
    }
    else if (numGamma==3){
      p[1] = randMultipole(p[0],542);
      p[2] = randMultipole(p[1],420);
    }
    else if (numGamma==4){
      p[1] = randMultipole(p[0],544);
      p[2] = randMultipole(p[1],442);
      p[3] = randMultipole(p[2],420);
    }

    for (int i=0; i<numGamma; i++){
      particleGun->SetParticleMomentumDirection(p[i]);
      particleGun->SetParticleEnergy(energy[i]);
      particleGun->GeneratePrimaryVertex(anEvent);
    }
  }

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

G4ThreeVector PrimaryGeneratorAction::randMultipole(G4ThreeVector P, int multipole){

  G4double theta = P.getTheta();
  G4double phi = P.getPhi();

  double theta2 = 0;
  double phi2 = 2*3.141592*G4UniformRand();
  if (multipole == 020){
    theta2 = fPDF020->GetRandom();
  }
  else if (multipole == 420){
    theta2 = fPDF420->GetRandom();
  }
  else if (multipole == 010){
    theta2 = fPDF010->GetRandom();
  }
  else if (multipole == 120){
    theta2 = fPDF120->GetRandom();
  }
  else if (multipole == 544){
    theta2 = fPDF544->GetRandom();
  }
  else if (multipole == 542){
    theta2 = fPDF542->GetRandom();
  }
  else if (multipole == 442){
    theta2 = fPDF442->GetRandom();
  }
  else{
    G4cout << "ERROR: Type of multipole radiation not defined!" << G4endl;
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
