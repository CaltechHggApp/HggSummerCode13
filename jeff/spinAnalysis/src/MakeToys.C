#include "MakeToys.h"
#include "AnalyzeToy.h"

using namespace std;
using namespace RooFit;


MakeToys::MakeToys()
{
   mass = new RooRealVar("mass","diphoton mass",110,140);
   cosT = new RooRealVar("cosT","Collins - Soper angle",0,1);
   mass->setBins(30);
}

MakeToys::~MakeToys()
{
   delete mass;
   delete cosT;
}


void MakeToys::createPoint()
{
   makePdfs();
   AnalyzeToy thing;
   thing.setNBins(nBins);
   thing.setCheat(cheat);
   thing.setPdfs(ws);
   thing.setNSignal(lumi_to_nsignal(lumi));

   thing.prepare_gen();
   double pvals[nToys];
   for(int i=0; i<nToys; i++)
   {
      if(i % 10 == 0) cout<<"making toy "<<i<<" at "<<lumi<<"fb^-1"<<endl;
      thing.run();
      pvals[i] = thing.getPvalue();
   }

   // get median pval and error
   double quantiles[3];
   double prob[3]={.159,.5,.841}; // median and 1 sigma in both directions
   TMath::Quantiles(nToys,3,pvals,quantiles,prob,kFALSE);

   savePoint(quantiles);
   delete ws;
}


void MakeToys::makePdfs()
{
   importMC();
   int n_before = MC0->numEntries();
   MC0=applyCuts(MC0);
   MC2gg=applyCuts(MC2gg);
   MC2qq=applyCuts(MC2qq);
   int n_after = MC0->numEntries();
   acceptance_x_efficiency = (float)n_after / (float)n_before;

   // these are just to define the pfd's and aren't used anywhere else
   RooRealVar mean_signal_mass("mean_signal_mass","",125,123,126);
   RooRealVar sigma_signal_mass("sigma_signal_mass","",2,0,10);
   RooRealVar decayConstant_bkg_mass("decayConstant_bkg_mass","",-.05,-5,5);
   RooRealVar p1_bkg_cosT("p1_bkg_cosT","",0,-100,100);
   RooRealVar p2_bkg_cosT("p2_bkg_cosT","",1,-100,100);


   // signal pdf's
   RooGaussian model_signal_mass("model_signal_mass","",*mass,mean_signal_mass,sigma_signal_mass);
   model_signal_mass.fitTo(*(MC0->reduce("mass")),PrintLevel(-1));

   RooDataHist hist0("hist0","",*cosT, *(MC0->reduce("cosT")));
   RooHistPdf model_signal0_cosT("model_signal0_cosT","",*cosT,hist0);

   RooDataHist hist2gg("hist2gg","",*cosT, *(MC2gg->reduce("cosT")));
   RooHistPdf model_signal2gg_cosT("model_signal2gg_cosT","",*cosT,hist2gg);

   RooDataHist hist2qq("hist2qq","",*cosT, *(MC2qq->reduce("cosT")));
   RooHistPdf model_signal2qq_cosT("model_signal2qq_cosT","",*cosT,hist2qq);


   // background pdf's
   RooExponential model_bkg_mass("model_bkg_mass","",*mass,decayConstant_bkg_mass);
   RooPolynomial model_bkg_cosT("model_bkg_cosT","",*cosT,RooArgList(p1_bkg_cosT,p2_bkg_cosT));

   ws = new RooWorkspace("ws");
   ws->import(model_signal_mass);
   ws->import(model_signal0_cosT);
   ws->import(model_signal2gg_cosT);
   ws->import(model_signal2qq_cosT);
   ws->import(model_bkg_mass);
   ws->import(model_bkg_cosT);

   delete MC0;
   delete MC2gg;
   delete MC2qq;
}


RooDataSet*  MakeToys::applyCuts(RooDataSet *originalData)
{
   RooRealVar pt1("pt1","",0,500);
   RooRealVar pt2("pt2","",0,500);
   RooRealVar maxEta("maxEta","",0,50);
   const RooArgSet *event;

   RooDataSet *acceptedData = new RooDataSet("acceptedData","",RooArgSet(*mass,*cosT));

   int nEntries=originalData->numEntries();
   double etaLimit = atof(etaMax.c_str());

   for(int i=0; i<nEntries; i++)
   {
      event = originalData->get(i);
      if(event->getRealValue("pt1") < 24) continue;
      if(event->getRealValue("pt2") < 16) continue;
      if(event->getRealValue("maxEta") > etaLimit) continue;

      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      acceptedData->add(RooArgSet(*mass,*cosT));
   }
   delete originalData;
   return acceptedData;
}

void MakeToys::importMC()
{
   RooRealVar pt1("pt1","",0,500);
   RooRealVar pt2("pt2","",0,500);
   RooRealVar maxEta("maxEta","",0,50);

   TFile file0( Form("./tempData/MC_0_%sEB_%sEE.root",EB_res.c_str(),EE_res.c_str()) );
   TFile file2gg( Form("./tempData/MC_2gg_%sEB_%sEE.root",EB_res.c_str(),EE_res.c_str()) );
   TFile file2qq( Form("./tempData/MC_2qq_%sEB_%sEE.root",EB_res.c_str(),EE_res.c_str()) );

   MC0 = new RooDataSet("MC0","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*((TTree*)file0.Get("tree"))));
   MC2gg = new RooDataSet("MC2","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*((TTree*)file2gg.Get("tree"))));
   MC2qq = new RooDataSet("MC2","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*((TTree*)file2qq.Get("tree"))));

   file0.Close();
   file2gg.Close();
   file2qq.Close();

   if(MC0->numEntries()<1 || MC2gg->numEntries()<1 || MC2qq->numEntries()<1)
   {
      cout<<"Error: MC files don't exist. Do ./runMC first"<<endl;
      exit(-1);
   }

}


void MakeToys::savePoint(double *quantiles) 
{
   TString filename = Form("./tempData/toy_%dfb_eta%s_%sEB_%sEE_%dqq.root",(int)floor(lumi),etaMax.c_str(),EB_res.c_str(),EE_res.c_str(),qqbar_percent);
   TFile f(filename, "recreate");

   double pval = quantiles[1];
   double pvalErrorLow = quantiles[1] - quantiles[0];
   double pvalErrorHigh = quantiles[2] - quantiles[1];
   double errorX = 0.;
   
   TGraphAsymmErrors graph(1,&lumi,&pval,&errorX,&errorX, &pvalErrorLow, &pvalErrorHigh);
   graph.SetName("graph");
   graph.Write();
   f.Close();
}


double MakeToys::lumi_to_nsignal(double lumi)
{
   double crossSection = 49.85e-12;
   double BR = 2.28e-3;
   double nsignal = lumi * 1.0e15 * crossSection * BR * acceptance_x_efficiency * .8;
   return nsignal;
}

void MakeToys::setNBins(int nbins)
{
   nBins = nbins;
   cosT->setBins(nBins);
}
