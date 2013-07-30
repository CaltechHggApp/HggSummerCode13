// create,fit, and plot toy data. Not really usful anymore
// To run do: root toy_practice.cc
// requires histos14.root to be in same folder


Double_t background(Double_t *x, Double_t *par)
{
   return par[1] * exp(par[0] * x[0]);
}

Double_t signal(Double_t *x, Double_t *par)
{
   return par[2] / par[1] / sqrt(2 * TMath::Pi()) * exp(-1 * (x[0] - par[0])*(x[0] - par[0]) / (2 * par[1] * par[1]));
}

Double_t fitFunction(Double_t *x, Double_t *par)
{
   return signal(x,par) + background(x,&par[3]);
}


TH1F* create_bkg()
{
   TF1 *bkg_func = new TF1("bkg_func", background, 115,135,2);
   bkg_func->SetParameters(-.06,1);
   TH1F *bkg = new TH1F("bkg", "", 50,115,135);
   bkg->FillRandom("bkg_func", 20000);
   return bkg;
}


void combine_sig_bkg()
{
   TH1F* s_plus_b = create_bkg();

   TFile *f14 = new TFile("histos14.root");
   TH1F *mgg = (TH1F*)f14->Get("mgg");
 
   s_plus_b->Add(mgg);
   s_plus_b->SetName("s_plus_b");

   TFile outFile("s_plus_b.root","recreate");   
   s_plus_b->Write();
   outFile.Close();
}


void fit_sb()
{
   TFile *file = new TFile("s_plus_b.root");
   TH1F *hist = (TH1F*)file->Get("s_plus_b");

   TF1 *fitFcn = new TF1("fitFcn", fitFunction, 100,150,5);
   fitFcn->SetParNames("mean","sigma","scale","bkg param","bkg param");
   fitFcn->SetParameters(124,2,1000,-.05,20000);
   hist->Fit("fitFcn","0");
   hist->SetMinimum(0);

   TF1 *signalFcn = new TF1("signalFcn", signal, 100,150,3);
   signalFcn->SetLineColor(kBlue);
   TF1 *backFcn = new TF1("backFcn", background,100,150,2);
   backFcn->SetLineColor(kBlack);

   Double_t par[5];
   fitFcn->GetParameters(par);
   signalFcn->SetParameters(par);
   backFcn->SetParameters(&par[3]);

   hist->Draw();
   hist->SetXTitle("m_{#gamma#gamma}");
   signalFcn->Draw("same"); 
   signalFcn->SetLineColor(kBlack);
   backFcn->Draw("same");
   backFcn->SetLineStyle(7);
   backFcn->SetLineColor(kRed);
} 

void fit_sig()
{
   TFile *f14 = new TFile("histos14.root");
   TH1F *mgg = (TH1F*)f14->Get("mgg");
   TF1 *fitFcn = new TF1("fitFcn", signal, 100,150,3);
   fitFcn->SetParNames("mean","sigma","scale");
   fitFcn->SetParameters(120,1,1000);
   fitFcn->SetLineColor(kBlack);
   mgg->Fit("fitFcn");
   mgg->SetMinimum(0);
   mgg->SetXTitle("m_{#gamma#gamma}");
   mgg->SetTitle("");
   
}

   
void toy_practice()
{
   gStyle->SetOptStat(0);
   gStyle->SetOptFit(1);
   combine_sig_bkg();
   TCanvas *c1 = new TCanvas("c1","",1500,600);
   c1->Divide(2);
   c1->cd(1);
   fit_sig();
   c1->cd(2);
   fit_sb();
}
