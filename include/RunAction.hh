//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// include/RunAction.hh
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TFile.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class RunActionMessenger;

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);
    
    void fillPerEvent(double[][4], double); 
    void SetFileName(G4String val) {fileName = val;}
    G4String GetFileName()        {return fileName;}

  private:
    // info about event
    double cutoff[2][4];
    double enDep[2][4];
    double enAdd[2];
    double enSing[2];
    int    hits[2];
    double NaIEn;
    // histograms
    TH1F *hEn[2][4];
    TH1F *hEnV[2][4];
    TH1F *hAdd[2];
    TH1F *hAddV[2];
    TH1F *hSing[2];
    TH1F *hSingV[2];
    TH1F *hHits[2];
    TH1F *hHitsV[2];
    TH1F *hNaI;
    TTree *tMC;
    TFile *fMC;

    G4String fileName;

    RunActionMessenger *runMessenger;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

