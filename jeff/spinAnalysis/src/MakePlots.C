#include "MakePlots.h"

void plot_eta()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile f40("./tempData/toys_2gg_current_40.root");
   TGraphAsymmErrors *g40 = (TGraphAsymmErrors*)f40.Get("graph");
   g40->SetMarkerStyle(20);
   g40->SetMarkerSize(1.0);
//   g40->SetMinimum(1.0e-7);
//   g40->SetMaximum(1.0e-1);
   g40->SetLineColor(kBlue);
   g40->SetMarkerColor(kBlue);
   g40->GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   g40->GetYaxis()->SetTitle("p-value (expected)");
   g40->SetTitle("");
   g40->Draw("AP");

   TFile f25("./tempData/toys_2gg_current_25.root");
   TGraphAsymmErrors *g25 = (TGraphAsymmErrors*)f25.Get("graph");
   g25->SetMarkerStyle(20);
   g25->SetLineColor(kGreen);
   g25->SetMarkerColor(kGreen);
   g25->Draw("PSAME");

   TFile f30("./tempData/toys_2gg_current_30.root");
   TGraphAsymmErrors *g30 = (TGraphAsymmErrors*)f30.Get("graph");
   g30->SetMarkerStyle(20);
   g30->SetLineColor(kBlack);
   g30->SetMarkerColor(kBlack);
   g30->Draw("PSAME");

   TFile f14("./tempData/toys_2gg_current_14.root");
   TGraphAsymmErrors *g14 = (TGraphAsymmErrors*)f14.Get("graph");
   g14->SetMarkerStyle(20);
   g14->SetLineColor(kRed);
   g14->SetMarkerColor(kRed);
   g14->Draw("PSAME");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(g14, "#eta < 1.4","lp");
   leg.AddEntry(g25, "#eta < 2.5","lp");
   leg.AddEntry(g30, "#eta < 3.0","lp");
   leg.AddEntry(g40, "#eta < 4.0","lp");
   leg.Draw();

   double xlow = g40->GetXaxis()->GetBinLowEdge(g40->GetXaxis()->GetFirst());
   double xhi =  g40->GetXaxis()->GetBinUpEdge(g40->GetXaxis()->GetLast());

   TLine l2(xlow,.0228, xhi,.0228);
   l2.SetLineStyle(3);
   l2.Draw();
   TLatex t2(21.3,.0228,"2 #sigma");
   t2.SetTextSize(0.035);
   t2.Draw();

   TLine l3(xlow,1.35e-3, xhi,1.35e-3);
   l3.SetLineStyle(3);
   l3.Draw();
   TLatex t3(21.3,1.35e-3,"3 #sigma");
   t3.SetTextSize(0.035);
   t3.Draw();

   TLine l4(xlow,3.17e-5, xhi,3.17e-5);
   l4.SetLineStyle(3);
   l4.Draw();
   TLatex t4(21.3,3.17e-5,"4 #sigma");
   t4.SetTextSize(0.035);
   t4.Draw();

   TLine l5(xlow,2.87e-7, xhi,2.87e-7);
   l5.SetLineStyle(3);
   l5.Draw();
   TLatex t5(21.3,2.87e-7,"5 #sigma");
   t5.SetTextSize(0.035);
   t5.Draw();

   c1.SaveAs("./plots/plot_eta.pdf");
}


void plot_res()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile fOpt("./tempData/toys_2gg_optimistic_25.root");
   TGraphAsymmErrors *gOpt = (TGraphAsymmErrors*)fOpt.Get("graph");
   gOpt->SetMarkerStyle(20);
   gOpt->SetLineColor(kBlue);
   gOpt->SetMarkerColor(kBlue);
   gOpt->Draw("AP");
   gOpt->GetXaxis()->SetTitle("#int L");
   gOpt->GetYaxis()->SetTitle("p-value (expected)");
   gOpt->SetTitle("");



   TFile fCurrent("./tempData/toys_2gg_current_25.root");
   TGraphAsymmErrors *gCurrent = (TGraphAsymmErrors*)fCurrent.Get("graph");
   gCurrent->SetMarkerStyle(20);
   gCurrent->SetLineColor(kGreen);
   gCurrent->SetMarkerColor(kGreen);
   gCurrent->Draw("PSAME");

   TFile fAge("./tempData/toys_2gg_aging_25.root");
   TGraphAsymmErrors *gAge = (TGraphAsymmErrors*)fAge.Get("graph");
   gAge->SetMarkerStyle(20);
   gAge->SetLineColor(kRed);
   gAge->SetMarkerColor(kRed);
   gAge->Draw("PSAME");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(gAge, "EB=4%;  EE=6%","lp");
   leg.AddEntry(gCurrent, "EB=2.4%;  EE=3.9%","lp");
   leg.AddEntry(gOpt, "EB=1%;  EE=1.5%","lp");
   leg.Draw();


   double xlow = gOpt->GetXaxis()->GetBinLowEdge(gOpt->GetXaxis()->GetFirst());
   double xhi =  gOpt->GetXaxis()->GetBinUpEdge(gOpt->GetXaxis()->GetLast());

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

   c1.SaveAs("./plots/plot_res.pdf");
}

void plot_gg_qq()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile fgg("./tempData/toys_2gg_current_25.root");
   TGraphAsymmErrors *ggg = (TGraphAsymmErrors*)fgg.Get("graph");
   ggg->SetMarkerStyle(20);
   ggg->SetLineColor(kBlack);
   ggg->SetMarkerColor(kBlack);
   ggg->Draw("AP");
   ggg->GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   ggg->GetYaxis()->SetTitle("p-value (expected)");
   ggg->SetTitle("");
//   ggg->SetMinimum(1.0e-135);

   TFile fqq("./tempData/toys_2qq_current_25.root");
   TGraphAsymmErrors *gqq = (TGraphAsymmErrors*)fqq.Get("graph");
   gqq->SetMarkerStyle(20);
   gqq->SetLineColor(kBlue);
   gqq->SetMarkerColor(kBlue);
   gqq->Draw("PSAME");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(ggg, "gluon fusion","lp");
   leg.AddEntry(gqq, "q qbar production","lp");
   leg.Draw();

   double xlow = ggg->GetXaxis()->GetBinLowEdge(ggg->GetXaxis()->GetFirst());
   double xhi =  ggg->GetXaxis()->GetBinUpEdge(ggg->GetXaxis()->GetLast());

   double num=20;
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

   c1.SaveAs("./plots/plot_gg_qq.pdf");
}


