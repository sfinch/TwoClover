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
  NaIEn = 0;
  for (int i=0; i<2; i++){
    for (int j=0; j<4; j++){
      EnergyDep[i][j] = 0.;
	}
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* evt)
{
  //accumulates statistic
  double bin[2][4];
  for (int i=0; i<2; i++){
    for (int j=0; j<4; j++){
      bin[i][j] = EnergyDep[i][j]/(keV);
    }
  }

  runAct->fillPerEvent(bin, NaIEn);
  
  //print per event (modulo n)
  G4int evtNb = evt->GetEventID();
  if (evtNb%printModulo == 0) {

    G4cout << "   Energy deposited in crystal 1: " << G4endl;
    G4cout << G4BestUnit(EnergyDep[0][0],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[0][1],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[0][2],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[0][3],"Energy") << G4endl;
    G4cout << "   Energy deposited in crystal 2: " << G4endl;
    G4cout << G4BestUnit(EnergyDep[1][0],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[1][1],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[1][2],"Energy") << std::setw(5)
           << G4BestUnit(EnergyDep[1][3],"Energy") << G4endl;

    G4cout << "   Total energy deposited in NaI: " << std::setw(7)
           << G4BestUnit(NaIEn,"Energy") << G4endl;
	  
    G4cout << "---> End of event: " << evtNb << G4endl;	
  }
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
