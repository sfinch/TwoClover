//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// src/SteppingAction.cc
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "EventAction.hh"

#include "G4Step.hh"

//#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(DetectorConstruction* det, EventAction* evt)
:detector(det), eventaction(evt)                     
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  // get volume of the current step
  G4VPhysicalVolume* volume 
  = aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  
  for (int i=0; i<2; i++){
    for (int j=0; j<4; j++){
      if (volume == detector->GetClover(i)->GetCrystal(j)){
        eventaction->AddEn(i,j,edep);
      }
    }
  }

  if (volume == detector->GetNaIAnnulus()->GetCrystal()) eventaction->AddVEn(edep);
  
  //example of saving random number seed of this event, under condition
  // if (condition) G4RunManager::GetRunManager()->rndmSaveThisEvent(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
