//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

class RunAction;
class EventActionMessenger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
public:
  EventAction(RunAction*);
  virtual ~EventAction();

  void  BeginOfEventAction(const G4Event*);
  void    EndOfEventAction(const G4Event*);
    
  void AddEn(G4int det, G4int seg, G4double de) {EnergyDep[det][seg] += de;};
  void AddVEn(G4double de)                      {NaIEn += de;};

  void SetPrintModulo(G4int val)  {printModulo = val;};
    
private:
   RunAction*  runAct;
   
   G4double  EnergyDep[2][4];
   G4double  NaIEn;
                     
   G4int     printModulo;
                             
   EventActionMessenger*  eventMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
