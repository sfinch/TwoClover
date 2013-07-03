//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "RunAction.hh"

#include "RunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include "TH1.h"
#include "TFile.h"
#include "TTree.h"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
:fileName("simulated")
{
	runMessenger = new RunActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
	delete runMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* aRun)
{ 
  G4String name = fileName+".root";
  fMC = new TFile(name.c_str(),"RECREATE");
  tMC = new TTree("tree2","MonteCarlo Data");

  tMC->Branch("NaI",&NaIEn,"NaI/D");
  for (int i=0; i<2; i++){
    int det = i+1;
  	tMC->Branch(Form("cAdd%i",det),&enAdd[i],Form("cAdd%i/D",det));
  	tMC->Branch(Form("cSing%i",det),&enSing[i],Form("cSing%i/D",det));
  	tMC->Branch(Form("cHits%i",det),&hits[i],Form("cHits%i/I",det));
  	for (int j=0; j<4; j++){
      int seg = j+1;
  	  tMC->Branch(Form("cE%i%i",det,seg),&enDep[i][j],Form("cE%i%i/D",det,seg));
	}
  }

  G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
    
  //initialize cumulative quantities
  hNaI = new TH1F("hNaI","hNaI",3000,0,3000);
  for (int i=0; i<2; i++){
    int det = i+1;
  	hAdd[i] = new TH1F(Form("hAdd%i",det),Form("hAdd%i",det),6000,0,3000);
  	hAddV[i] = new TH1F(Form("hAddV%i",det),Form("hAddV%i",det),6000,0,3000);
  	hSing[i] = new TH1F(Form("hSing%i",det),Form("hAdd%i",det),6000,0,3000);
  	hSingV[i] = new TH1F(Form("hSingV%i",det),Form("hAddV%i",det),6000,0,3000);
  	hHits[i] = new TH1F(Form("hHits%i",det),Form("hHits%i",det),5,0,4);
  	hHitsV[i] = new TH1F(Form("hHitsV%i",det),Form("hHitsV%i",det),5,0,4);
  	for (int j=0; j<4; j++){
      int seg = j+1;
  	  hEn[i][j] = new TH1F(Form("hEn%i%i",det,seg),Form("hEn%i%i",det,seg),6000,0,3000);
  	  hEnV[i][j] = new TH1F(Form("hEnV%i%i",det,seg),Form("hEnV%i%i",det,seg),6000,0,3000);
	}
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::fillPerEvent(double E[][4], double NaIE)
{
  NaIEn = NaIE;
  double maxE = 0;
  for (int i=0; i<2; i++){
    enAdd[i]=0;
    enSing[i]=0;
    hits[i]=0;
  }

  for (int i=0; i<2; i++){
    for (int j=0; j<4; j++){
	  enDep[i][j] = E[i][j];
	  if (enDep[i][j]>50){
	  	enAdd[i]+=enDep[i][j];
		hits[i]++;
	  }
  	  if (enDep[i][j] > maxE){
  	    maxE = enDep[i][j];
      }
	}
	if (hits[i]==1){
	  hSing[i] = hAdd[i];
	  hAdd[i] = 0;
	}
  }

  //accumulate statistic
  if (maxE>200){	// trigger
  	hNaI->Fill(NaIEn);
  	for (int i=0; i<2; i++){
	  hAdd[i]->Fill(enAdd[i]);
	  hSing[i]->Fill(enSing[i]);
	  hHits[i]->Fill(hits[i]);
	  if (NaIEn<50){
	    hAddV[i]->Fill(enAdd[i]);
	    hSingV[i]->Fill(enSing[i]);
	    hHitsV[i]->Fill(hits[i]);
	  }
   	  for (int j=0; j<4; j++){
  		hEn[i][j]->Fill(enDep[i][j]);
		if (NaIEn<50){
		  hEnV[i][j]->Fill(enDep[i][j]);
  		}
	  }
	}
  	tMC->Fill();
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* aRun)
{
  G4int NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

    
  hNaI->Write();
  for (int i=0; i<2; i++){
    hAdd[i]->Write();
    hSing[i]->Write();
    hHits[i]->Write();
    hAddV[i]->Write();
    hSingV[i]->Write();
    hHitsV[i]->Write();
    for (int j=0; j<4; j++){
  	  hEn[i][j]->Write();
  	  hEnV[i][j]->Write();
	}
  }
  tMC->Write("", TObject::kOverwrite);
  fMC->Write();
  fMC->Close();

  //print
  G4cout
     << "\n--------------------End of Run------------------------------\n"
	 << "   Data saved to file " << fileName << ".root"
     << "\n------------------------------------------------------------\n"
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
