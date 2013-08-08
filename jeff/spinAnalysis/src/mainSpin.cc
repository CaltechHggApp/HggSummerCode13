// C++ includes
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <fstream>

// ROOT includes

#include <TROOT.h>
#include <TFile.h>
#include <TString.h>
#include <TTree.h>
#include <TChain.h>
#include <TH1F.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TRandom3.h>
#include <TF1.h>

#include "analyzerSpin.cc"
#include "class2.cc"
#include "../include/ReadConfig.hh"

using namespace std;
using namespace RooFit;

int main()
{
   (RooRandom::randomGenerator())->SetSeed(0);
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);


   ReadConfig cfgReader("config.cfg");
   int runMC = atoi(cfgReader.getParameter("runMC").c_str());
   int allMC = atoi(cfgReader.getParameter("allMC").c_str());
   int EB_res = atoi(cfgReader.getParameter("EB_res").c_str());
   int EE_res = atoi(cfgReader.getParameter("EE_res").c_str());
   float maxEta = atof(cfgReader.getParameter("maxEta").c_str());
   float lumi = atof(cfgReader.getParameter("lumi").c_str());
   int nBins = atoi(cfgReader.getParameter("nBins").c_str());

   int energy = atoi(cfgReader.getParameter("energy").c_str());
   int n_bkg = atoi(cfgReader.getParameter("n_bkg").c_str());
   TString plot_dir = cfgReader.getParameter("plot_dir").c_str();


   if(runMC)
   {
      cout<<"Running over spin0 MC"<<endl;
      TTree *tree0 = makeTree(0, allMC); 
      HggSpin analyzer0(tree0);
      analyzer0.Loop("MC_output_0.root");
      
      cout<<"Running over spin2 MC"<<endl;
      TTree *tree2 = makeTree(2,allMC);
      HggSpin analyzer2(tree2);
      analyzer2.Loop("MC_output_2.root");
   }


   Class2 obj;
   obj.setNBins(nBins);
   obj.setNSignal(lumi, n_bkg);
   obj.create_signal_pdfs();
   obj.create_spin2_pdf();
   obj.generate();
   obj.determineYield();
   obj.extractSignal();
   obj.plot(plot_dir);

   return 0;
}
