void plot(){
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	
	TH1F *hEn[2];
	TH1F *hEnV[2];
	TH2F *h2D;
	TH2F *h2D;

	TFile *fMC = new TFile("simulated.root");
	
	h2D = (TH2F*)fMC->Get("h2D");
	h2DV = (TH2F*)fMC->Get("h2DV");
	for (int i=0; i<2; i++){
		hEn[i] = (TH1F*)fMC->Get(Form("hEn%i",i));
		hEnV[i] = (TH1F*)fMC->Get(Form("hEnV%i",i));
	}

	TCanvas *c1 = new TCanvas("c1","Simulated Data",800,600);
	c1->Divide(1,2);
	for (int i=0; i<2; i++){
		c1->cd(i+1);
		gPad->SetLogy();
		hEn[i]->Draw();
		hEn[i]->GetXaxis()->SetTitle("E [keV]");
		hEn[i]->GetYaxis()->SetTitle("counts");
		hEn[i]->SetTitle();

		hEnV[i]->SetLineColor(2);
		hEnV[i]->Draw("same");
	}

	TCanvas *c2 = new TCanvas("c2","Simulated Data",1000,600);
	c2->Divide(2,1);
	c2->cd(1);
	h2D->Draw("col");
	c2->cd(2);
	h2DV->Draw("col");

}
