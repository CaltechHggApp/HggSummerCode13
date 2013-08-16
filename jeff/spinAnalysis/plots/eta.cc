void eta()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile f14("current_14.root");
   TGraphAsymmErrors *g14 = (TGraphAsymmErrors*)f14.Get("graph");
   g14->SetLineColor(kBlue);
   g14->SetMarkerColor(kBlue);
   g14->Draw("AP");
   g14->GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   g14->GetYaxis()->SetTitle("p-value (expected)");
   g14->SetTitle("ECAL #eta detection limit");


   TFile f25("current_25.root");
   TGraphAsymmErrors *g25 = (TGraphAsymmErrors*)f25.Get("graph");
   g25->SetLineColor(kGreen);
   g25->SetMarkerColor(kGreen);
   g25->Draw("PSAME");

   TFile f30("current_30.root");
   TGraphAsymmErrors *g30 = (TGraphAsymmErrors*)f30.Get("graph");
   g30->SetLineColor(kMagenta);
   g30->SetMarkerColor(kMagenta);
   g30->Draw("PSAME");

   TFile f40("current_40.root");
   TGraphAsymmErrors *g40 = (TGraphAsymmErrors*)f40.Get("graph");
   g40->SetLineColor(kRed);
   g40->SetMarkerColor(kRed);
   g40->Draw("PSAME");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(g14, "#eta < 1.4","lp"); //p
   leg.AddEntry(g25, "#eta < 2.5","lp");
   leg.AddEntry(g30, "#eta < 3.0","lp");
   leg.AddEntry(g40, "#eta < 4.0","lp");
   leg.Draw();


   double xlow = g14->GetXaxis()->GetBinLowEdge(g14->GetXaxis()->GetFirst());
   double xhi =  g14->GetXaxis()->GetBinUpEdge(g14->GetXaxis()->GetLast());

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
