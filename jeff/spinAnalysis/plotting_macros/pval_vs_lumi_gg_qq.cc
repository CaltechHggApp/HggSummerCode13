void pval_vs_lumi_gg_qq()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile f14("testgg.root");
   TGraphAsymmErrors *g14 = (TGraphAsymmErrors*)f14.Get("graph");
   g14->SetLineColor(kBlack);
   g14->SetMarkerColor(kBlack);
   g14->Draw("AP");
   g14->GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   g14->GetYaxis()->SetTitle("p-value (expected)");
   g14->SetTitle("");
   g14->SetMinimum(1.0e-135);

   TFile f25("testqq.root");
   TGraphAsymmErrors *g25 = (TGraphAsymmErrors*)f25.Get("graph");
   g25->SetLineColor(kBlue);
   g25->SetMarkerColor(kBlue);
   g25->Draw("PSAME");
/*
   TFile f30("current_30.root");
   TGraphAsymmErrors *g30 = (TGraphAsymmErrors*)f30.Get("graph");
   g30->SetLineColor(kGray);
   g30->SetMarkerColor(kGray);
   g30->Draw("PSAME");

   TFile f40("current_40.root");
   TGraphAsymmErrors *g40 = (TGraphAsymmErrors*)f40.Get("graph");
   g40->SetLineColor(kRed);
   g40->SetMarkerColor(kRed);
   g40->Draw("PSAME");
*/
   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(g14, "gluon fusion","lp"); //p
   leg.AddEntry(g25, "q qbar production","lp");
//   leg.AddEntry(g30, "#eta < 3.0","lp");
//   leg.AddEntry(g40, "#eta < 4.0","lp");
   leg.Draw();


   double xlow = g14->GetXaxis()->GetBinLowEdge(g14->GetXaxis()->GetFirst());
   double xhi =  g14->GetXaxis()->GetBinUpEdge(g14->GetXaxis()->GetLast());

   double num=325;
   TLine l2(xlow,.0228, xhi,.0228);
   l2.SetLineStyle(3);
   l2.Draw();
   TLatex t2(num,.0228,"2 #sigma");
   t2.SetTextSize(0.035);
   t2.Draw();

   TLine l3(xlow,1.35e-3, xhi,1.35e-3);
   l3.SetLineStyle(3);
   l3.Draw();
   TLatex t3(num,1.35e-3,"3 #sigma");
   t3.SetTextSize(0.035);
   t3.Draw();

   TLine l4(xlow,3.17e-5, xhi,3.17e-5);
   l4.SetLineStyle(3);
   l4.Draw();
   TLatex t4(num,3.17e-5,"4 #sigma");
   t4.SetTextSize(0.035);
   t4.Draw();

   TLine l5(xlow,2.87e-7, xhi,2.87e-7);
   l5.SetLineStyle(3);
   l5.Draw();
   TLatex t5(num,2.87e-7,"5 #sigma");
   t5.SetTextSize(0.035);
   t5.Draw();


   c1.SaveAs("plot_gg_qq.pdf");
}
