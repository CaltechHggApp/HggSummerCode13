#include "MakePlots.h"

using namespace std;

void plot_eta(vector<double> lumis, vector<string> etas)
{
   TCanvas c1("c1","");
   c1.SetLogy();


   const int n=1;
   double x[n] = {0};
   double y[n] = {0};       // these are janky but they make it work
   double exl[n] = {0.};


   TGraphAsymmErrors graph(n,x,y,exl,exl,exl,exl);

   graph.SetTitle("");
   graph.GetXaxis()->SetTitle("#int Lumi (fb^{-1})");
   graph.GetYaxis()->SetTitle("p-value (expected)");

   graph.GetXaxis()->SetLimits(0,30);
   graph.SetMaximum(1.0e-1);
   graph.SetMinimum(1.0e-10);
   graph.Draw("AP");

   TLegend leg(.13,.12,.3,.3);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);

   for(vector<double>::iterator lumiIt= lumis.begin(); lumiIt!= lumis.end(); lumiIt++)
   {
      for(vector<string>::iterator etaIt = etas.begin(); etaIt!= etas.end(); etaIt++)
      {
	 TString filename = Form("./tempData/toy_%1.0ffb_eta%s_.024EB_.039EE_0qq.root",*lumiIt,(*etaIt).c_str());
	 TFile f(filename);
	 if(f.IsZombie())
	 {
	    cout<<"Error: file doesn't exit"<<endl;
	    exit(-1);
	 }
	 TGraphAsymmErrors *g = (TGraphAsymmErrors*)f.Get("graph");

	 Color_t color = kGray;
	 if(*etaIt==1.4) color = kBlue;
	 if(*etaIt==2.5) color = kGreen;
	 if(*etaIt==3.0) color = kRed;
	 if(*etaIt==4.0) color = kBlack;

	 
	 g->SetLineColor(color);
	 g->SetMarkerColor(color);
	 g->SetMarkerStyle(20);
	 g->Draw("PSAME");

	 leg.AddEntry(g, Form("#eta < %s",(*etaIt).c_str()),"lp");

      }
   }

   leg.Draw();

//   double xlow = graph.GetXaxis()->GetBinLowEdge(graph.GetXaxis()->GetFirst());
//   double xhi =  graph.GetXaxis()->GetBinUpEdge(graph.GetXaxis()->GetLast());
   double xlow=5;
   double xhi=30;
   double num =xhi * 1.01;

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

   c1.Update();
   c1.SaveAs("./plots/plot_eta.pdf");
}

/*
void plot_res()
{
   TCanvas c1("c1","");
   c1.SetLogy();

   TFile fOpt("./tempData/toys_2gg_optimistic_25.root");
   TGraphAsymmErrors *gOpt = (TGraphAsymmErrors*)fOpt.Get("graph");
   gOpt->SetMaximum(1.0e-1);
//   gOpt->SetMinimum(1.0e-24);
   gOpt->SetMarkerStyle(20);
   gOpt->SetLineColor(kRed);
   gOpt->SetMarkerColor(kRed);
   gOpt->GetXaxis()->SetTitle("#int L");
   gOpt->GetYaxis()->SetTitle("p-value (expected)");
   gOpt->SetTitle("");
   gOpt->Draw("AP");



   TFile fCurrent("./tempData/toys_2gg_current_25.root");
   TGraphAsymmErrors *gCurrent = (TGraphAsymmErrors*)fCurrent.Get("graph");
   gCurrent->SetMarkerStyle(20);
   gCurrent->SetLineColor(kGreen);
   gCurrent->SetMarkerColor(kGreen);
   gCurrent->Draw("PSAME");

   TFile fAge("./tempData/toys_2gg_aging_25.root");
   TGraphAsymmErrors *gAge = (TGraphAsymmErrors*)fAge.Get("graph");
   gAge->SetMarkerStyle(20);
   gAge->SetLineColor(kBlue);
   gAge->SetMarkerColor(kBlue);
   gAge->Draw("PSAME");

   TLegend leg(.13,.12,.3,.25);
   leg.SetTextSize(0.04);
   leg.SetFillColor(0);
   leg.SetBorderSize(0);
   leg.AddEntry(gAge, "EB=4%;  EE=6%","lp");
   leg.AddEntry(gCurrent, "EB=2.4%;  EE=3.9%","lp");
   leg.AddEntry(gOpt, "EB=1%;  EE=1.5%","lp");
   leg.Draw();


   double xlow = gOpt->GetXaxis()->GetBinLowEdge(gOpt->GetXaxis()->GetFirst());
   double xhi =  gOpt->GetXaxis()->GetBinUpEdge(gOpt->GetXaxis()->GetLast());

   double num= xhi * 1.01;

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

   TLine l6(xlow,9.87e-10, xhi,9.87e-10);
   l6.SetLineStyle(3);
   l6.Draw();
   TLatex t6(num,9.87e-10,"6 #sigma");
   t6.SetTextSize(0.035);
   t6.Draw();

   c1.SaveAs("./plots/plot_res.pdf");
}

*/
