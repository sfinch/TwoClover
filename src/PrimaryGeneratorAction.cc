//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* DC)
:Detector(DC),rndmFlag("off")
{
  numGamma = 1;
  positionR = 0*cm;
  energy[0] = 1*MeV;
  energy[1] = 1*MeV;
  energy[2] = 1*MeV;
  energy[3] = 1*MeV;

  particleGun  = new G4ParticleGun(1);
  
  //create a messenger for this class
  gunMessenger = new PrimaryGeneratorMessenger(this);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("gamma");

  particleGun->SetParticleDefinition(particle);
  particleGun->SetParticleEnergy(energy[0]);
  particleGun->SetParticlePosition(G4ThreeVector(0*cm,positionR,0.*cm));

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
  delete gunMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  //this function is called at the begining of event
  G4ThreeVector p = randP();
  particleGun->SetParticleMomentumDirection(p);
  particleGun->SetParticleEnergy(energy[0]);
  particleGun->SetParticlePosition(G4ThreeVector(0*cm,positionR,0.*cm));
  
  if (numGamma==1){
  	particleGun->GeneratePrimaryVertex(anEvent);
  }

  else if (numGamma==2){
    particleGun->GeneratePrimaryVertex(anEvent);

    G4ThreeVector p2 = randE2(p);
    particleGun->SetParticleMomentumDirection(p2);
    particleGun->SetParticleEnergy(energy[1]);
    particleGun->GeneratePrimaryVertex(anEvent);
  }
  else if (numGamma==3){
    particleGun->GeneratePrimaryVertex(anEvent);

    p = randP();
    particleGun->SetParticleMomentumDirection(p);
    particleGun->SetParticleEnergy(energy[1]);
    particleGun->GeneratePrimaryVertex(anEvent);

    p = randP();
    particleGun->SetParticleMomentumDirection(p);
    particleGun->SetParticleEnergy(energy[2]);
    particleGun->GeneratePrimaryVertex(anEvent);
  }
  else if (numGamma==4){
    particleGun->GeneratePrimaryVertex(anEvent);

    p = randP();
    particleGun->SetParticleMomentumDirection(p);
    particleGun->SetParticleEnergy(energy[1]);
    particleGun->GeneratePrimaryVertex(anEvent);

    p = randP();
    particleGun->SetParticleMomentumDirection(p);
    particleGun->SetParticleEnergy(energy[2]);
    particleGun->GeneratePrimaryVertex(anEvent);

    p = randP();
    particleGun->SetParticleMomentumDirection(p);
    particleGun->SetParticleEnergy(energy[3]);
    particleGun->GeneratePrimaryVertex(anEvent);
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

G4ThreeVector PrimaryGeneratorAction::randE2(G4ThreeVector P){

  G4double theta = P.getTheta();
  G4double phi = P.getPhi();

  TF1 *fPDF = new TF1("fPDF","1.250-3.75*cos(x)**2+5.00*cos(x)**4",0.,3.14159);

  double theta2 = fPDF->GetRandom();
  double phi2 = 2*3.14159*G4UniformRand();

  G4RotationMatrix *rot = new G4RotationMatrix();
  rot->rotateY(theta);
  rot->rotateZ(phi);

  G4ThreeVector e2 (1,0,0);
  e2.setTheta(theta2);
  e2.setPhi(phi2);
  e2.setMag(1.);

  e2.transform(*rot);
  delete rot;
  delete fPDF;

  //G4cout << "Theta should be " << std::cos(theta2) << ": " << e2.dot(P) << G4endl;

  return e2;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
