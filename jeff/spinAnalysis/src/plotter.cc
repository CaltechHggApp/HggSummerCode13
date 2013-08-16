#include "plotter.h"
#include "class2.h"

using namespace std;
using namespace RooFit;


plotter::plotter()
{
   ws = new RooWorkspace("ws");
   mass = new RooRealVar("mass","diphoton mass",110,140);
   cosT = new RooRealVar("cosT","Collins - Soper angle",0,1);
   mass->setBins(30);
}

plotter::~plotter()
{
   delete ws;
   delete mass;
   delete cosT;
}


void plotter::makePdfs()
{
   cosT->setBins(nBins);
   readMC();
   double n_before = MC0->numEntries();
   MC0=applyCuts(MC0);
   MC2=applyCuts(MC2);
   double n_after = MC0->numEntries();
   acceptance_x_efficiency = n_after / n_before;

   // these are just to define the pfd's and aren't used anywhere else
   RooRealVar mean_sig_mass("mean_sig_mass","",125,80,160);
   RooRealVar sigma_sig_mass("sigma_sig_mass","",2,0,10);
   RooRealVar c_bkg_mass("c_bkg_mass","",-.05,-5,5);
   RooRealVar p1_bkg_cosT("p1_bkg_cosT","",0,-100,100);
   RooRealVar p2_bkg_cosT("p2_bkg_cosT","",1,-100,100);


   // signal pdf's
   RooGaussian model_sig_mass("model_sig_mass","",*mass,mean_sig_mass,sigma_sig_mass);
   model_sig_mass.fitTo(*(MC0->reduce("mass")),PrintLevel(-1));

   RooDataHist hist0("hist0","",*cosT, *(MC0->reduce("cosT")));
   RooHistPdf model_sig0_cosT("model_sig0_cosT","",*cosT,hist0);

   RooDataHist hist2("hist2","",*cosT, *(MC2->reduce("cosT")));
   RooHistPdf model_sig2_cosT("model_sig2_cosT","",*cosT,hist2);


   // background pdf's
   RooExponential model_bkg_mass("model_bkg_mass","",*mass,c_bkg_mass);
   RooPolynomial model_bkg_cosT("model_bkg_cosT","",*cosT,RooArgList(p1_bkg_cosT,p2_bkg_cosT));

   ws->import(model_sig_mass);
   ws->import(model_sig0_cosT);
   ws->import(model_sig2_cosT);
   ws->import(model_bkg_mass);
   ws->import(model_bkg_cosT);

   TCanvas c1;
   RooPlot *frame1 = cosT->frame();
   model_sig0_cosT.plotOn(frame1,LineColor(kBlack));
   frame1->SetTitle("");
   frame1->SetXTitle("cos(#theta)");
   frame1->SetYTitle("");
   frame1->Draw();
   c1.SaveAs("cosT_sig0.pdf");

   TCanvas c2;
   RooPlot *frame2 = cosT->frame();
   model_sig2_cosT.plotOn(frame2,LineColor(kBlack));
   frame2->SetTitle("");
   frame2->SetXTitle("cos(#theta)");
   frame2->SetYTitle("");
   frame2->Draw();
   c2.SaveAs("cosT_sig2.pdf");

   delete MC0;
   delete MC2;
}


RooDataSet*  plotter::applyCuts(RooDataSet* originalData)
{
   RooRealVar pt1("pt1","",0,500);
   RooRealVar pt2("pt2","",0,500);
   RooRealVar maxEta("maxEta","",0,50);
   const RooArgSet *event;

   RooDataSet *acceptedData = new RooDataSet("acceptedData","",RooArgSet(*mass,*cosT));

   int nEntries=originalData->numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = originalData->get(i);
      if(event->getRealValue("pt1") < 24) continue;
      if(event->getRealValue("pt2") < 16) continue;
      if(event->getRealValue("maxEta") > etaMax) continue;

      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      acceptedData->add(RooArgSet(*mass,*cosT));
   }
   delete originalData;
   return acceptedData;
}

void plotter::readMC()
{
   RooRealVar pt1("pt1","",0,500);
   RooRealVar pt2("pt2","",0,500);
   RooRealVar maxEta("maxEta","",0,50);

   TFile file0("./lib/0_" + MC_filename);
   MC0 = new RooDataSet("MC0","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*((TTree*)file0.Get("tree"))));

   TFile file2("./lib/2_" + MC_filename);
   MC2 = new RooDataSet("MC2","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*((TTree*)file2.Get("tree"))));
   file0.Close();
   file2.Close();
}

void plotter::calculate()
{
   Class2 thing;
   thing.setPdfs(ws);
   thing.setNBins(nBins);
   for(vector<double>::iterator lumiIt = lumi.begin(); lumiIt != lumi.end(); lumiIt++)
   {
      thing.setNSignal(lumi_to_nsignal(*lumiIt));
      double pvals[nToys];
      thing.prepare_gen();
      gStyle->SetOptStat("orme");
      TFile f("rawr.root","recreate");
      TH1F hist("hist","",10,0,.01);
      hist.StatOverflows();
      for(int i=0; i<nToys; i++)
      {
	 cout<<"making toy "<<i<<" at "<<*lumiIt<<"fb"<<endl;
	 thing.generate_toy();
	 thing.extract_signal();
	 pvals[i] = thing.getPvalue();
       hist.Fill(pvals[i]);
      }
      hist.SetLineColor(kBlack);
      hist.SetMarkerStyle(20);
      hist.GetXaxis()->SetTitle("p-value");
      hist.GetYaxis()->SetTitle("Events / ");
      hist.Write();
      double quantiles[3];
      double prob[3]={.159,.5,.841};
      TMath::Quantiles(nToys,3,pvals,quantiles,prob,kFALSE);

      pvalue1stQuartile.push_back(quantiles[0]);
      pvalueMedian.push_back(quantiles[1]);
      pvalue3rdQuartile.push_back(quantiles[2]);
   }

   for(vector<double>::iterator lumiIt = lumi.begin(); lumiIt != lumi.end(); lumiIt++)
   {
      int index = lumiIt - lumi.begin();
      cout<<"lumi is "<<lumi[index]<<";  pval Q1 is "<<pvalue1stQuartile[index]<<";  pval med is "<<pvalueMedian[index]<<";  pval Q3 is "<<pvalue3rdQuartile[index]<<endl;
   }
}

void plotter::make_plot_of_toy()
{
   cout<<"\nMAKING PLOT OF SINGLE TOY"<<endl;
   Class2 thing;
   thing.setNSignal(lumi_to_nsignal(plotLumi));
   thing.setPdfs(ws);
   thing.setNBins(nBins);
   thing.prepare_gen();
   thing.generate_toy();
   thing.extract_signal();
   thing.plot();
   cout<<"pval of plot is "<<thing.getPvalue()<<endl;

}

void plotter::make_plot_lumi(TString filename)
{
   cout<<"\nMAKING PVALUE VS LUMI PLOT"<<endl;
   int n = lumi.size();
   double x[n];
   double y[n];
   double dx_low[n];
   double dx_hi[n];
   double dy_low[n];
   double dy_hi[n];

   for(int i=0; i<n; i++)
   {
      x[i] = lumi[i];
      y[i] = pvalueMedian[i];
      dy_low[i] = pvalue1stQuartile[i];
      dy_hi[i] = pvalue3rdQuartile[i];
      dx_low[i] = 0.;
      dx_hi[i]  = 0.;
   }

   TFile f("./plots/"+ filename,"recreate");
   TGraphAsymmErrors graph(n,x,y,dx_low,dx_hi, dy_low, dy_hi);
   graph.SetName("graph");
   graph.SetTitle("pval vs. lumi");
   graph.SetMarkerStyle(20);
   graph.SetMarkerSize(1.0);
   graph.GetXaxis()->SetTitle("lumi");
   graph.GetYaxis()->SetTitle("expected pvalue");
   graph.SetMaximum(1.0e-1);
   graph.SetMinimum(1.0e-7);
   graph.Write();
   f.Close();
}


double plotter::lumi_to_nsignal(double lumi)
{
   double crossSection = 49.85e-12;
   double BR = 2.28e-3;
   double nsignal = lumi * 1.0e15 * crossSection * BR * acceptance_x_efficiency * .8;
   return nsignal;
}
