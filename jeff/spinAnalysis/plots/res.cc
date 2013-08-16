void res()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile fAge("aging_25.root");
   TGraphAsymmErrors *gAge = (TGraphAsymmErrors*)fAge.Get("graph");
   gAge->SetLineColor(kBlue);
   gAge->SetMarkerColor(kBlue);
   gAge->Draw("AP");
   gAge->GetXaxis()->SetTitle("#int L");
   gAge->GetYaxis()->SetTitle("p-value (expected)");
   gAge->SetTitle("ECAL Resolution: high pilup, current, optimistic");



   TFile fCurrent("current_25.root");
   TGraphAsymmErrors *gCurrent = (TGraphAsymmErrors*)fCurrent.Get("graph");
   gCurrent->SetLineColor(kGreen);
   gCurrent->SetMarkerColor(kGreen);
   gCurrent->Draw("PSAME");

   TFile fOpt("optimistic_25.root");
   TGraphAsymmErrors *gOpt = (TGraphAsymmErrors*)fOpt.Get("graph");
   gOpt->SetLineColor(kRed);
   gOpt->SetMarkerColor(kRed);
   gOpt->Draw("PSAME");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(gAge, "EB=4%;  EE=6%","lp");
   leg.AddEntry(gCurrent, "EB=2.4%;  EE=3.9%","lp");
   leg.AddEntry(gOpt, "EB=1%;  EE=1.5%","lp");
   leg.Draw();


   double xlow = gAge->GetXaxis()->GetBinLowEdge(gAge->GetXaxis()->GetFirst());
   double xhi =  gAge->GetXaxis()->GetBinUpEdge(gAge->GetXaxis()->GetLast());

   TLine l2(xlow,.0228, xhi,.0228);
   l2.SetLineStyle(3);
   l2.Draw();

   TLine l3(xlow,1.35e-3, xhi,1.35e-3);
   l3.SetLineStyle(3);
   l3.Draw();

   TLine l4(xlow,3.17e-5, xhi,3.17e-5);
   l4.SetLineStyle(3);
   l4.Draw();

   TLine l5(xlow,2.87e-7, xhi,2.87e-7);
   l5.SetLineStyle(3);
   l5.Draw();



   TPaveText text(.1,.3,.2,.4);
   text.AddText("3 sigma");
   text.Draw();


   TLatex *prelim = new TLatex(8.,1.0e-5,"CMS Preliminary");
   prelim->SetNDC();
   prelim->SetTextSize(0.045);
   prelim->SetTextColor(kBlack);
   prelim->Draw();

   c1.SaveAs("plot.pdf");
}
