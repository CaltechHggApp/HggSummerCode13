void pval_vs_lumi_gg()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile f25("testgg_short.root");
   TGraphAsymmErrors *g25 = (TGraphAsymmErrors*)f25.Get("graph");
//   g25->SetLineColor(kBlue);
//   g25->SetMarkerColor(kBlue);
   g25->Draw("AP");
   g25->GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   g25->GetYaxis()->SetTitle("p-value (expected)");
   g25->SetTitle("");
   g25->SetMinimum(1.0e-9);
   g25->GetXaxis()->SetRange(0,130);

   double xlow = g25->GetXaxis()->GetBinLowEdge(g25->GetXaxis()->GetFirst());
   double xhi =  g25->GetXaxis()->GetBinUpEdge(g25->GetXaxis()->GetLast());
/*
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
*/

   double num = 22;
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



   c1.SaveAs("plot_gg.pdf");
}
