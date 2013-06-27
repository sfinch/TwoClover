//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "RunAction.hh"
#include "EventActionMessenger.hh"

#include "G4Event.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RunAction* run)
:runAct(run),printModulo(1),eventMessenger(0)
{
  eventMessenger = new EventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* evt)
{  
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) { 
    G4cout << "\n---> Begin of event: " << evtNb << G4endl;
    CLHEP::HepRandom::showEngineStatus();
  }
 
  // initialisation per event
  EnergyDep[0] = 0.;
  EnergyDep[1] = 0.;
  EnergyDep[2] = 0.;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  double bin[3];
  bin[0] = EnergyDep[0]/(keV);
  bin[1] = EnergyDep[1]/(keV);
  bin[2] = EnergyDep[2]/(keV);

  runAct->fillPerEvent(bin);
  
  //print per event (modulo n)
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) {

    G4cout << "   Total energy deposited in crystal 1: " << std::setw(7)
           << G4BestUnit(EnergyDep[0],"Energy") << G4endl;
    G4cout << "   Total energy deposited in crystal 2: " << std::setw(7)
           << G4BestUnit(EnergyDep[1],"Energy") << G4endl;
    G4cout << "   Total energy deposited in NaI: " << std::setw(7)
           << G4BestUnit(EnergyDep[2],"Energy") << G4endl;
	  
    G4cout << "---> End of event: " << evtNb << G4endl;	
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
