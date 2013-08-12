#include "plotter.h"
#include "class2.h"

using namespace std;
using namespace RooFit;


plotter::plotter()
{
   ws = new RooWorkspace("ws");
   mass = new RooRealVar("mass","diphoton mass",110,150);
   cosT = new RooRealVar("cosT","Collins - Soper angle",0,1);
}

plotter::~plotter()
{
   delete ws;
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
//   signalWidth = sigma_sig_mass->getVal();

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
}

void plotter::calculate()
{
   int nToys = 3;
   Class2 thing;
   thing.setPdfs(ws);
   thing.setNBins(nBins);
   lumi.push_back(10);
   lumi.push_back(20);
   lumi.push_back(0);

   for(vector<double>::iterator lumiIt = lumi.begin(); lumiIt != lumi.end(); lumiIt++)
   {
      thing.setNSignal(lumi_to_nsignal(*lumiIt));
      vector<double> pvals;
      for(int i=0; i<nToys; i++)
      {
	 thing.generate_toy();
	 thing.extract_signal();
	 pvals.push_back(thing.getPvalue());
      }
      double sum = std::accumulate(pvals.begin(), pvals.end(), 0.0);
      double meanPval = sum / pvals.size();

      double sq_sum = std::inner_product(pvals.begin(), pvals.end(), pvals.begin(), 0.0);
      double stdev = std::sqrt(sq_sum / pvals.size() - meanPval * meanPval);
      pvalueMean.push_back(meanPval);
      pvalueSigma.push_back(stdev);
   }
}

void plotter::make_plot_of_toy()
{
   Class2 thing;
   thing.setNSignal(lumi_to_nsignal(plotLumi));
   thing.setPdfs(ws);
   cout<<"setting number of bins to "<<nBins<<endl;
   thing.setNBins(nBins);
   thing.generate_toy();
   thing.extract_signal();
   thing.plot(plot_dir);
   thing.getPvalue();

}


double plotter::lumi_to_nsignal(double lumi)
{
   double crossSection = 49.85e-12;
   double BR = 2.28e-3;
   double nsignal = lumi * 1.0e15 * crossSection * BR * acceptance_x_efficiency;
   return nsignal;
}
