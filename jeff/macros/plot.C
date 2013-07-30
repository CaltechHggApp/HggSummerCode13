// macro to create various plots for h->gg spin
// specifically those used in my presentation on July 2
// To run: change the variable num below to the value
// associated with the plot you want. Then do   root plot.C
// requires plot.C to be in the same folder as histos14.root
// and histos8.root


#define num 1

/*    
1 maxEta_noCuts
2 maxEta_cuts
3 ptgg
4 etagg
5 cosTheta8
6 cosTheta14
7 eta backup
8 cosTheta backup
*/

void plot()
{
   TFile *f8 = new TFile("histos8.root");
   TFile *f14 = new TFile("histos14.root");

   const float sigma_8 = 19.52e-12;
   const float sigma_14 = 49.85e-12;
   const float BR = 2.28e-3;
   const float L = 1e15;


   mgg_8 = (TH1F*)f8->Get("mgg");
   mgg_14 = (TH1F*)f14->Get("mgg");
   maxEta_8 = (TH1F*)f8->Get("maxEta");
   maxEta_14 = (TH1F*)f14->Get("maxEta");
   cosTheta8 = (TH1F*)f8->Get("cosTheta");
   cosTheta14 = (TH1F*)f14->Get("cosTheta");
   cosTheta8_subLead = (TH1F*)f8->Get("cosTheta_subLead");
   cosTheta14_subLead = (TH1F*)f14->Get("cosTheta_subLead");
   ptgg_8 = (TH1F*)f8->Get("ptgg");
   ptgg_14 = (TH1F*)f14->Get("ptgg");
   etagg_8 = (TH1F*)f8->Get("etagg");
   etagg_14 = (TH1F*)f14->Get("etagg");
   maxEta_32_24_8 = (TH1F*)f8->Get("maxEta_32_24");
   maxEta_44_16_8 = (TH1F*)f8->Get("maxEta_44_16");
   maxEta_60_24_8 = (TH1F*)f8->Get("maxEta_60_24");
   maxEta_32_24_14 = (TH1F*)f14->Get("maxEta_32_24");
   maxEta_44_16_14 = (TH1F*)f14->Get("maxEta_44_16");
   maxEta_60_24_14 = (TH1F*)f14->Get("maxEta_60_24");



   pt_eta_32_24_25_8 = (TH1F*)f8->Get("pt_eta_32_24_25");
   pt_eta_32_24_30_8 = (TH1F*)f8->Get("pt_eta_32_24_30");
   pt_eta_32_24_40_8 = (TH1F*)f8->Get("pt_eta_32_24_40");
   pt_eta_32_24_14_8 = (TH1F*)f8->Get("pt_eta_32_24_14");
   pt_eta_44_16_25_8 = (TH1F*)f8->Get("pt_eta_44_16_25");
   pt_eta_44_16_30_8 = (TH1F*)f8->Get("pt_eta_44_16_30");
   pt_eta_44_16_40_8 = (TH1F*)f8->Get("pt_eta_44_16_40");
   pt_eta_44_16_14_8 = (TH1F*)f8->Get("pt_eta_44_16_14");
   pt_eta_60_24_25_8 = (TH1F*)f8->Get("pt_eta_60_24_25");
   pt_eta_60_24_30_8 = (TH1F*)f8->Get("pt_eta_60_24_30");
   pt_eta_60_24_40_8 = (TH1F*)f8->Get("pt_eta_60_24_40");
   pt_eta_60_24_14_8 = (TH1F*)f8->Get("pt_eta_60_24_14");

   pt_eta_32_24_25_14 = (TH1F*)f14->Get("pt_eta_32_24_25");
   pt_eta_32_24_30_14 = (TH1F*)f14->Get("pt_eta_32_24_30");
   pt_eta_32_24_40_14 = (TH1F*)f14->Get("pt_eta_32_24_40");
   pt_eta_32_24_14_14 = (TH1F*)f14->Get("pt_eta_32_24_14");
   pt_eta_44_16_25_14 = (TH1F*)f14->Get("pt_eta_44_16_25");
   pt_eta_44_16_30_14 = (TH1F*)f14->Get("pt_eta_44_16_30");
   pt_eta_44_16_40_14 = (TH1F*)f14->Get("pt_eta_44_16_40");
   pt_eta_44_16_14_14 = (TH1F*)f14->Get("pt_eta_44_16_14");
   pt_eta_60_24_25_14 = (TH1F*)f14->Get("pt_eta_60_24_25");
   pt_eta_60_24_30_14 = (TH1F*)f14->Get("pt_eta_60_24_30");
   pt_eta_60_24_40_14 = (TH1F*)f14->Get("pt_eta_60_24_40");
   pt_eta_60_24_14_14 = (TH1F*)f14->Get("pt_eta_60_24_14");


   gStyle->SetOptStat(11);
   int nEvents_8 = mgg_8->GetEntries();
   int nEvents_14 = mgg_14->GetEntries();
   float lumiScale_8 = sigma_8 * BR * L / (float)nEvents_8;
   float lumiScale_14 = sigma_14 * BR * L / (float)nEvents_14;



   if(num==1){
   TCanvas *c1 = new TCanvas("c1");
   gStyle->SetOptStat(11);
   int nEvents_8 = maxEta_8->GetEntries();
   int nEvents_14 = maxEta_14->GetEntries();
   maxEta_8->Draw();
//   maxEta_14->Scale(sigma_14 * BR * L / nEvents_14);
   maxEta_8->Scale(1/maxEta_8->Integral());
   maxEta_8->SetLineColor(kBlue);
   maxEta_8->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
//   maxEta_14->SetYTitle("Events / fb^{-1}");
   maxEta_8->SetYTitle("Events (arbitrary units)");
   maxEta_8->SetTitle("|#eta|_{max}");
   maxEta->GetYaxis()->SetTitleOffset(1.4);
   maxEta_14->Draw("same");
//   maxEta_8->Scale(sigma_8 * BR * L / nEvents_8);
   maxEta_14->Scale(1/maxEta_14->Integral());
   maxEta_14->SetLineColor(kRed);

   TLegend *leg = new TLegend(0.7,0.6,.85,.75);
   leg->SetTextSize(0.05);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(maxEta_8, "8TeV","l");
   leg->AddEntry(maxEta_14, "14TeV","l");
   leg->Draw();
   }



   if(num==7){
   TCanvas *c1 = new TCanvas("c1");
   c1->cd();
   c1->Divide(3);
   c1->cd(1);
   maxEta_32_24_8->Draw();
   maxEta_32_24_8->Scale(1/maxEta_32_24_8->Integral());
   maxEta_32_24_8->SetLineColor(kBlue);
   maxEta_32_24_8->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
   maxEta_32_24_8->SetYTitle("Events (arbitrary units)");
   maxEta_32_24_8->SetTitle("p_{T}^{lead} > 32, p_{T}^{sublead} > 24");
   maxEta_32_24_14->Draw("same");
   maxEta_32_24_14->Scale(1/maxEta_32_24_14->Integral());
   maxEta_32_24_14->SetLineColor(kRed);

   c1->cd(2);
   maxEta_44_16_8->Draw();
   maxEta_44_16_8->Scale(1/maxEta_44_16_8->Integral());
   maxEta_44_16_8->SetLineColor(kBlue);
   maxEta_44_16_8->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
   maxEta_44_16_8->SetYTitle("Events (arbitrary units)");
   maxEta_44_16_8->SetTitle("p_{T}^{lead} > 44, p_{T}^{sublead} > 16");
   maxEta_44_16_14->Draw("same");
   maxEta_44_16_14->Scale(1/maxEta_44_16_14->Integral());
   maxEta_44_16_14->SetLineColor(kRed);

   c1->cd(3);
   maxEta_60_24_8->Draw();
   maxEta_60_24_8->Scale(1/maxEta_60_24_8->Integral());
   maxEta_60_24_8->SetLineColor(kBlue);
   maxEta_60_24_8->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
   maxEta_60_24_8->SetYTitle("Events (arbitrary units)");
   maxEta_60_24_8->SetTitle("p_{T}^{lead} > 60, p_{T}^{sublead} > 24");
   maxEta_60_24_14->Draw("same");
   maxEta_60_24_14->Scale(1/maxEta_60_24_14->Integral());
   maxEta_60_24_14->SetLineColor(kRed);

   TLegend *leg = new TLegend(0.7,0.6,.85,.75);
   leg->SetTextSize(0.05);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(maxEta_60_24_8, "8TeV","l");
   leg->AddEntry(maxEta_60_24_14, "14TeV","l");
   leg->Draw();

   }



   if(num==2){
   TCanvas *c10 = new TCanvas("c10");
   c10->Divide(2);
   c10->cd(1);
   maxEta_32_24_8->Draw();
   maxEta_32_24_8->Scale(1/maxEta_32_24_8->Integral());
   maxEta_32_24_8->SetLineColor(kBlue);
   maxEta_32_24_8->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
   maxEta_32_24_8->SetYTitle("Events");
   maxEta_32_24_8->SetTitle("8TeV");
   maxEta_32_24_8->SetMaximum(.06);
   maxEta_44_16_8->Draw("same");
   maxEta_44_16_8->Scale(1/maxEta_44_16_8->Integral());
   maxEta_44_16_8->SetLineColor(kGreen);
   maxEta_60_24_8->Draw("same");
   maxEta_60_24_8->Scale(1/maxEta_60_24_8->Integral());
   maxEta_60_24_8->SetLineColor(kRed);

   c10->cd(2);
   maxEta_32_24_14->Draw();
   maxEta_32_24_14->Scale(1/maxEta_32_24_14->Integral());
   maxEta_32_24_14->SetLineColor(kBlue);
   maxEta_32_24_14->SetXTitle("max(|#eta_{1}|,|#eta_{2}|)");
   maxEta_32_24_14->SetYTitle("Events");
   maxEta_32_24_14->SetTitle("14TeV");
   maxEta_32_24_14->SetMaximum(.06);
   maxEta_44_16_14->Draw("same");
   maxEta_44_16_14->Scale(1/maxEta_44_16_14->Integral());
   maxEta_44_16_14->SetLineColor(kGreen);
   maxEta_60_24_14->Draw("same");
   maxEta_60_24_14->Scale(1/maxEta_60_24_14->Integral());
   maxEta_60_24_14->SetLineColor(kRed);

   TLegend *leg10 = new TLegend(0.4,0.6,.65,.75);
   leg10->SetTextSize(0.05);
   leg10->SetFillColor(0);
   leg10->SetBorderSize(0);
   leg10->AddEntry(maxEta_32_24_14, "p_{T}^{lead} > 32, p_{T}^{sublead} > 24","l");
   leg10->AddEntry(maxEta_44_16_14, "p_{T}^{lead} > 44, p_{T}^{sublead} > 16","l");
   leg10->AddEntry(maxEta_60_24_14, "p_{T}^{lead} > 60, p_{T}^{sublead} > 24","l");
   leg10->Draw();
   }




   if(num==5){
   TCanvas *c6 = new TCanvas("c6");

   c6->SetTitle("8TeV");
   c6->Divide(3);
   c6->cd(1);
   pt_eta_32_24_40_8->Draw();
   pt_eta_32_24_40_8->Scale(lumiScale_8);
   pt_eta_32_24_40_8->SetLineColor(kBlack);
   pt_eta_32_24_40_8->SetXTitle("|cos(#theta)|");
   pt_eta_32_24_40_8->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_32_24_40_8->SetTitle("p_{T}^{lead} > 32, p_{T}^{sublead} > 24");
   pt_eta_32_24_40_8->SetMaximum(2.5);
   pt_eta_32_24_25_8->Draw("same");
   pt_eta_32_24_25_8->Scale(lumiScale_8);
   pt_eta_32_24_25_8->SetLineColor(kRed);
   pt_eta_32_24_30_8->Draw("same");
   pt_eta_32_24_30_8->Scale(lumiScale_8);
   pt_eta_32_24_30_8->SetLineColor(kGreen);
   pt_eta_32_24_14_8->Draw("same");
   pt_eta_32_24_14_8->SetLineColor(kBlue);
   pt_eta_32_24_14_8->Scale(lumiScale_8);
   c6->cd(2);
   pt_eta_44_16_40_8->Draw();
   pt_eta_44_16_40_8->Scale(lumiScale_8);
   pt_eta_44_16_40_8->SetLineColor(kBlack);
   pt_eta_44_16_40_8->SetTitle("p_{T}^{lead} > 44, p_{T}^{sublead} > 16");
   pt_eta_44_16_40_8->SetXTitle("|cos(#theta)|");
   pt_eta_44_16_40_8->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_44_16_40_8->SetMaximum(2.5);
   pt_eta_44_16_25_8->Draw("same");
   pt_eta_44_16_25_8->Scale(lumiScale_8);
   pt_eta_44_16_25_8->SetLineColor(kRed);
   pt_eta_44_16_30_8->Draw("same");
   pt_eta_44_16_30_8->Scale(lumiScale_8);
   pt_eta_44_16_30_8->SetLineColor(kGreen);
   pt_eta_44_16_14_8->Draw("same");
   pt_eta_44_16_14_8->Scale(lumiScale_8);
   pt_eta_44_16_14_8->SetLineColor(kBlue);
   c6->cd(3);
   pt_eta_60_24_40_8->Draw();
   pt_eta_60_24_40_8->Scale(lumiScale_8);
   pt_eta_60_24_40_8->SetLineColor(kBlack);
   pt_eta_60_24_40_8->SetTitle("p_{T}^{lead} > 60, p_{T}^{sublead} > 24");
   pt_eta_60_24_40_8->SetXTitle("|cos(#theta)|");
   pt_eta_60_24_40_8->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_60_24_40_8->SetMaximum(2.5);
   pt_eta_60_24_25_8->Draw("same");
   pt_eta_60_24_25_8->Scale(lumiScale_8);
   pt_eta_60_24_25_8->SetLineColor(kRed);
   pt_eta_60_24_30_8->Draw("same");
   pt_eta_60_24_30_8->Scale(lumiScale_8);
   pt_eta_60_24_30_8->SetLineColor(kGreen);
   pt_eta_60_24_14_8->Draw("same");
   pt_eta_60_24_14_8->Scale(lumiScale_8);
   pt_eta_60_24_14_8->SetLineColor(kBlue);

   TLegend *leg = new TLegend(0.5,0.6,.85,.75);
   leg->SetTextSize(0.05);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(pt_eta_60_24_40_8, "|#eta| < 4.0","l");
   leg->AddEntry(pt_eta_60_24_30_8, "|#eta| < 3.0","l");
   leg->AddEntry(pt_eta_60_24_25_8, "|#eta| < 2.5","l");
   leg->AddEntry(pt_eta_60_24_14_8, "|#eta| < 1.4","l");
   leg->Draw();
   }


   if(num==6){
      TCanvas *c6 = new TCanvas("c6");

      c6->SetTitle("14TeV");
      c6->Divide(3);
      c6->cd(1);
      pt_eta_32_24_40_14->Draw();
      pt_eta_32_24_40_14->Scale(lumiScale_14);
      pt_eta_32_24_40_14->SetLineColor(kBlack);
      pt_eta_32_24_40_14->SetXTitle("|cos(#theta)|");
      pt_eta_32_24_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
      pt_eta_32_24_40_14->SetTitle("p_{T}^{lead} > 32, p_{T}^{sublead} > 24");
      pt_eta_32_24_40_14->SetMaximum(6.5);
      pt_eta_32_24_25_14->Draw("same");
      pt_eta_32_24_25_14->Scale(lumiScale_14);
      pt_eta_32_24_25_14->SetLineColor(kRed);
      pt_eta_32_24_30_14->Draw("same");
      pt_eta_32_24_30_14->Scale(lumiScale_14);
      pt_eta_32_24_30_14->SetLineColor(kGreen);
      pt_eta_32_24_14_14->Draw("same");
      pt_eta_32_24_14_14->SetLineColor(kBlue);
      pt_eta_32_24_14_14->Scale(lumiScale_14);
      c6->cd(2);
      pt_eta_44_16_40_14->Draw();
      pt_eta_44_16_40_14->Scale(lumiScale_14);
      pt_eta_44_16_40_14->SetLineColor(kBlack);
      pt_eta_44_16_40_14->SetTitle("p_{T}^{lead} > 44, p_{T}^{sublead} > 16");
      pt_eta_44_16_40_14->SetXTitle("|cos(#theta)|");
      pt_eta_44_16_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
      pt_eta_44_16_40_14->SetMaximum(6.5);
      pt_eta_44_16_25_14->Draw("same");
      pt_eta_44_16_25_14->Scale(lumiScale_14);
      pt_eta_44_16_25_14->SetLineColor(kRed);
      pt_eta_44_16_30_14->Draw("same");
      pt_eta_44_16_30_14->Scale(lumiScale_14);
      pt_eta_44_16_30_14->SetLineColor(kGreen);
      pt_eta_44_16_14_14->Draw("same");
      pt_eta_44_16_14_14->Scale(lumiScale_14);
      pt_eta_44_16_14_14->SetLineColor(kBlue);
      c6->cd(3);
      pt_eta_60_24_40_14->Draw();
      pt_eta_60_24_40_14->Scale(lumiScale_14);
      pt_eta_60_24_40_14->SetLineColor(kBlack);
      pt_eta_60_24_40_14->SetTitle("p_{T}^{lead} > 60, p_{T}^{sublead} > 24");
      pt_eta_60_24_40_14->SetXTitle("|cos(#theta)|");
      pt_eta_60_24_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
      pt_eta_60_24_40_14->SetMaximum(6.5);
      pt_eta_60_24_25_14->Draw("same");
      pt_eta_60_24_25_14->Scale(lumiScale_14);
      pt_eta_60_24_25_14->SetLineColor(kRed);
      pt_eta_60_24_30_14->Draw("same");
      pt_eta_60_24_30_14->Scale(lumiScale_14);
      pt_eta_60_24_30_14->SetLineColor(kGreen);
      pt_eta_60_24_14_14->Draw("same");
      pt_eta_60_24_14_14->Scale(lumiScale_14);
      pt_eta_60_24_14_14->SetLineColor(kBlue);

      TLegend *leg = new TLegend(0.5,0.7,.85,.75);
      leg->SetTextSize(0.05);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->AddEntry(pt_eta_60_24_40_14, "|#eta| < 4.0","l");
      leg->AddEntry(pt_eta_60_24_30_14, "|#eta| < 3.0","l");
      leg->AddEntry(pt_eta_60_24_25_14, "|#eta| < 2.5","l");
      leg->AddEntry(pt_eta_60_24_14_14, "|#eta| < 1.4","l");
      leg->Draw();
   }









   if(num==8){
   TCanvas *c2 = new TCanvas("c2");

   c2->cd();
   c2->Divide(4,3);
   c2->cd(4);
   pt_eta_32_24_14_14->Draw();
   pt_eta_32_24_14_14->SetTitle("|#eta|_{max} < 1.4");
   pt_eta_32_24_14_14->SetXTitle("|cos(#theta)|");
   pt_eta_32_24_14_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_32_24_14_14->Scale(lumiScale_14);
   pt_eta_32_24_14_14->SetMaximum(6.4);
   pt_eta_32_24_14_14->SetLineColor(kRed);
   pt_eta_32_24_14_8->Draw("same");
   pt_eta_32_24_14_8->Scale(lumiScale_8);
   pt_eta_32_24_14_8->SetLineColor(kBlue);
   c2->cd(3);
   pt_eta_32_24_25_14->Draw();
   pt_eta_32_24_25_14->SetTitle("|#eta|_{max} < 2.5");
   pt_eta_32_24_25_14->SetXTitle("|cos(#theta)|");
   pt_eta_32_24_25_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_32_24_25_14->Scale(lumiScale_14);
   pt_eta_32_24_25_14->SetMaximum(6.4);
   pt_eta_32_24_25_14->SetLineColor(kRed);
   pt_eta_32_24_25_8->Draw("same");
   pt_eta_32_24_25_8->Scale(lumiScale_8);
   pt_eta_32_24_25_8->SetLineColor(kBlue);
   c2->cd(2);
   pt_eta_32_24_30_14->Draw();
   pt_eta_32_24_30_14->SetTitle("|#eta|_{max} < 3.0");
   pt_eta_32_24_30_14->SetXTitle("|cos(#theta)|");
   pt_eta_32_24_30_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_32_24_30_14->Scale(lumiScale_14);
   pt_eta_32_24_30_14->SetMaximum(6.4);
   pt_eta_32_24_30_14->SetLineColor(kRed);
   pt_eta_32_24_30_8->Draw("same");
   pt_eta_32_24_30_8->Scale(lumiScale_8);
   pt_eta_32_24_30_8->SetLineColor(kBlue);
   c2->cd(1);
   pt_eta_32_24_40_14->Draw();
   pt_eta_32_24_40_14->SetTitle("|#eta|_{max} < 4.0");
   pt_eta_32_24_40_14->SetXTitle("|cos(#theta)|");
   pt_eta_32_24_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_32_24_40_14->Scale(lumiScale_14);
   pt_eta_32_24_40_14->SetMaximum(6.4);
   pt_eta_32_24_40_14->SetLineColor(kRed);
   pt_eta_32_24_40_8->Draw("same");
   pt_eta_32_24_40_8->Scale(lumiScale_8);
   pt_eta_32_24_40_8->SetLineColor(kBlue);

   c2->cd(8);
   pt_eta_44_16_14_14->Draw();
//   pt_eta_44_16_14_14->SetTitle("|\eta|_{max} < 1.4");
   pt_eta_44_16_14_14->SetXTitle("|cos(#theta)|");
   pt_eta_44_16_14_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_44_16_14_14->Scale(lumiScale_14);
   pt_eta_44_16_14_14->SetMaximum(6.4);
   pt_eta_44_16_14_14->SetLineColor(kRed);
   pt_eta_44_16_14_8->Draw("same");
   pt_eta_44_16_14_8->Scale(lumiScale_8);
   pt_eta_44_16_14_8->SetLineColor(kBlue);
   c2->cd(7);
   pt_eta_44_16_25_14->Draw();
//   pt_eta_44_16_25_14->SetTitle("|\eta|_{max} < 2.5");
   pt_eta_44_16_25_14->SetXTitle("|cos(#theta)|");
   pt_eta_44_16_25_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_44_16_25_14->Scale(lumiScale_14);
   pt_eta_44_16_25_14->SetMaximum(6.4);
   pt_eta_44_16_25_14->SetLineColor(kRed);
   pt_eta_44_16_25_8->Draw("same");
   pt_eta_44_16_25_8->Scale(lumiScale_8);
   pt_eta_44_16_25_8->SetLineColor(kBlue);
   c2->cd(6);
   pt_eta_44_16_30_14->Draw();
//   pt_eta_44_16_30_14->SetTitle("|\eta|_{max} < 3.0");
   pt_eta_44_16_30_14->SetXTitle("|cos(#theta)|");
   pt_eta_44_16_30_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_44_16_30_14->Scale(lumiScale_14);
   pt_eta_44_16_30_14->SetMaximum(6.4);
   pt_eta_44_16_30_14->SetLineColor(kRed);
   pt_eta_44_16_30_8->Draw("same");
   pt_eta_44_16_30_8->Scale(lumiScale_8);
   pt_eta_44_16_30_8->SetLineColor(kBlue);
   c2->cd(5);
   pt_eta_44_16_40_14->Draw();
//   pt_eta_44_16_40_14->SetTitle("|\eta|_{max} < 4.0");
   pt_eta_44_16_40_14->SetXTitle("|cos(#theta)|");
   pt_eta_44_16_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_44_16_40_14->Scale(lumiScale_14);
   pt_eta_44_16_40_14->SetMaximum(6.4);
   pt_eta_44_16_40_14->SetLineColor(kRed);
   pt_eta_44_16_40_8->Draw("same");
   pt_eta_44_16_40_8->Scale(lumiScale_8);
   pt_eta_44_16_40_8->SetLineColor(kBlue);

   c2->cd(12);
   pt_eta_60_24_14_14->Draw();
//   pt_eta_60_24_14_14->SetTitle("|\eta|_{max} < 1.4");
   pt_eta_60_24_14_14->SetXTitle("|cos(#theta)|");
   pt_eta_60_24_14_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_60_24_14_14->Scale(lumiScale_14);
   pt_eta_60_24_14_14->SetMaximum(6.4);
   pt_eta_60_24_14_14->SetLineColor(kRed);
   pt_eta_60_24_14_8->Draw("same");
   pt_eta_60_24_14_8->Scale(lumiScale_8);
   pt_eta_60_24_14_8->SetLineColor(kBlue);
   TLegend *leg = new TLegend(0.5,0.6,.85,.75);
   leg->SetTextSize(0.05);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(pt_eta_60_24_14_8, "8 TeV","l");
   leg->AddEntry(pt_eta_60_24_14_14, "14 TeV","l");
   leg->Draw();

   c2->cd(11);
   pt_eta_60_24_25_14->Draw();
//   pt_eta_60_24_25_14->SetTitle("|\eta|_{max} < 2.5");
   pt_eta_60_24_25_14->SetXTitle("|cos(#theta)|");
   pt_eta_60_24_25_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_60_24_25_14->Scale(lumiScale_14);
   pt_eta_60_24_25_14->SetMaximum(6.4);
   pt_eta_60_24_25_14->SetLineColor(kRed);
   pt_eta_60_24_25_8->Draw("same");
   pt_eta_60_24_25_8->Scale(lumiScale_8);
   pt_eta_60_24_25_8->SetLineColor(kBlue);
   c2->cd(10);
   pt_eta_60_24_30_14->Draw();
//   pt_eta_60_24_30_14->SetTitle("|\eta|_{max} < 3.0");
   pt_eta_60_24_30_14->SetXTitle("|cos(#theta)|");
   pt_eta_60_24_30_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_60_24_30_14->Scale(lumiScale_14);
   pt_eta_60_24_30_14->SetMaximum(6.4);
   pt_eta_60_24_30_14->SetLineColor(kRed);
   pt_eta_60_24_30_8->Draw("same");
   pt_eta_60_24_30_8->Scale(lumiScale_8);
   pt_eta_60_24_30_8->SetLineColor(kBlue);
   c2->cd(9);
   pt_eta_60_24_40_14->Draw();
//   pt_eta_60_24_40_14->SetTitle("|\eta|_{max} < 4.0");
   pt_eta_60_24_40_14->SetXTitle("|cos(#theta)|");
   pt_eta_60_24_40_14->SetYTitle("Events / fb^{-1} / 0.05 GeV");
   pt_eta_60_24_40_14->Scale(lumiScale_14);
   pt_eta_60_24_40_14->SetMaximum(6.4);
   pt_eta_60_24_40_14->SetLineColor(kRed);
   pt_eta_60_24_40_8->Draw("same");
   pt_eta_60_24_40_8->Scale(lumiScale_8);
   pt_eta_60_24_40_8->SetLineColor(kBlue);
   }  





   if(num==3){
   TCanvas *c8 = new TCanvas("c8");
   ptgg_8->Draw();
   ptgg_8->SetTitle("p_{T}^{#gamma#gamma}");
   ptgg_8->SetXTitle("p_{T}^{#gamma#gamma}");
   ptgg_8->SetYTitle("Events (arbitrary units)");
   ptgg_8->Scale(1/ptgg_8->Integral());
   ptgg_8->SetLineColor(kBlue);
   ptgg_14->Draw("same");
   ptgg_14->Scale(1/ptgg_14->Integral());
   ptgg_14->SetLineColor(kRed);

   TLegend *leg = new TLegend(0.7,0.6,.85,.75);
   leg->SetTextSize(0.05);
   leg->SetFillColor(0);
   leg->SetBorderSize(0);
   leg->AddEntry(ptgg_8, "8TeV","l");
   leg->AddEntry(ptgg_14, "14TeV","l");
   leg->Draw();
   }

   


   if(num==4){
      TCanvas *c100 = new TCanvas("c100");
      etagg_8->Draw();
      etagg_8->SetTitle("#eta_{#gamma#gamma}");
      etagg_8->SetXTitle("#eta_{#gamma#gamma}");
      etagg_8->SetYTitle("Events (arbitrary units)");
      etagg_8->SetLineColor(kBlue);
      etagg_8->Scale(1/etagg_8->Integral());
      etagg_14->Draw("same");
      etagg_14->Scale(1/etagg_14->Integral());
      etagg_14->SetLineColor(kRed);

      TLegend *leg = new TLegend(0.7,0.6,.85,.75);
      leg->SetTextSize(0.05);
      leg->SetFillColor(0);
      leg->SetBorderSize(0);
      leg->AddEntry(etagg_8, "8TeV","l");
      leg->AddEntry(etagg_14, "14TeV","l");
      leg->Draw();
   }

}



 
