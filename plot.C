// plot.C
// Plots energy histograms for both clovers (with and vithout veto)
// single segment, addback, singles mode, and number of hits are plotted

void plot(){
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	
    // histograms
	TH1F *hEn[2][4];
	TH1F *hEnV[2][4];

	TH1F *hAdd[2];
	TH1F *hSing[2];
	TH1F *hHits[2];

	TH1F *hAddV[2];
	TH1F *hSingV[2];
	TH1F *hHitsV[2];

    // the TFile
	TFile *fMC = new TFile("simulated.root");
	
    // get histos
	for (int i=0; i<2; i++){
		int clover = i+1;	
		
		hAdd[i] = (TH1F*)fMC->Get(Form("hAdd%i",clover));
		hSing[i] = (TH1F*)fMC->Get(Form("hSing%i",clover));
		hHits[i] = (TH1F*)fMC->Get(Form("hHits%i",clover));

		hAddV[i] = (TH1F*)fMC->Get(Form("hAddV%i",clover));
		hSingV[i] = (TH1F*)fMC->Get(Form("hSingV%i",clover));
		hHitsV[i] = (TH1F*)fMC->Get(Form("hHitsV%i",clover));
		for (int j=0; j<4; j++){
			int seg = j+1;

			hEn[i][j] = (TH1F*)fMC->Get(Form("hEn%i%i",clover,seg));
			hEnV[i][j] = (TH1F*)fMC->Get(Form("hEnV%i%i",clover,seg));
		}
	}

    // plot individual segments
	TCanvas *c1 = new TCanvas("c1","Simulated Data",1000,600);
	c1->Divide(4,2);
	int counter = 1;
	for (int i=0; i<2; i++){
		for (int j=0; j<4; j++){
			c1->cd(counter);
			gPad->SetLogy();

			hEn[i][j]->Rebin(2);
			hEn[i][j]->GetXaxis()->SetRange(0,1500);
			hEn[i][j]->GetXaxis()->SetTitle("E [keV]");
			hEn[i][j]->GetYaxis()->SetTitle("counts");
			hEn[i][j]->SetTitle();
			hEn[i][j]->Draw();

			hEnV[i][j]->Rebin(2);
			hEnV[i][j]->SetLineColor(2);
			hEnV[i][j]->Draw("same");

			counter++;
		}
	}

    // plot addback
	TCanvas *cAdd = new TCanvas("cAdd","Addback Simulated Data",1000,600);
	cAdd->Divide(1,2);
	for (int i=0; i<2; i++){
		cAdd->cd(i+1);
		gPad->SetLogy();

		hAdd[i]->Rebin(2);
		hAdd[i]->GetXaxis()->SetRange(0,1500);
		hAdd[i]->GetXaxis()->SetTitle("E [keV]");
		hAdd[i]->GetYaxis()->SetTitle("counts");
		hAdd[i]->SetTitle();
		hAdd[i]->Draw();

		hAddV[i]->Rebin(2);
		hAddV[i]->SetLineColor(2);
		hAddV[i]->Draw("same");

	}

    // plot singles mode
	TCanvas *cSing = new TCanvas("cSing","Singles Simulated Data",1000,600);
	cSing->Divide(1,2);
	for (int i=0; i<2; i++){
		cSing->cd(i+1);
		gPad->SetLogy();

		hSing[i]->Rebin(2);
		hSing[i]->GetXaxis()->SetRange(0,1500);
		hSing[i]->GetXaxis()->SetTitle("E [keV]");
		hSing[i]->GetYaxis()->SetTitle("counts");
		hSing[i]->SetTitle();
		hSing[i]->Draw();

		hSingV[i]->Rebin(2);
		hSingV[i]->SetLineColor(2);
		hSingV[i]->Draw("same");
	}

    // plot number of hits per clover 
	TCanvas *cHits = new TCanvas("cHits","Hits Simulated Data",1000,600);
	cHits->Divide(1,2);
	for (int i=0; i<2; i++){
		cHits->cd(i+1);
		gPad->SetLogy();

		hHits[i]->Draw();
		hHits[i]->GetXaxis()->SetRange(0,1500);
		hHits[i]->GetXaxis()->SetTitle("E [keV]");
		hHits[i]->GetYaxis()->SetTitle("counts");
		hHits[i]->SetTitle();

		hHitsV[i]->SetLineColor(2);
		hHitsV[i]->Draw("same");
	}
}
