// C++ includes
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <fstream>

// ROOT includes

//#include <TROOT.h>
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
   int lumi = atoi(cfgReader.getParameter("lumi").c_str());
   int energy = atoi(cfgReader.getParameter("energy").c_str());
   int n_bkg = atoi(cfgReader.getParameter("n_bkg").c_str());



   if(runMC)
   {
      TString filename = (energy == 8) ? "histos8.root" : "histos14.root";
      TTree *tree = makeTree(energy, allMC); 
      HggSpin* t = new HggSpin(tree);
      t->Loop(filename);
   }


   Class2* obj = new Class2();
   obj->setEtaRanges();
   obj->fitMC();
   obj->generate();
   obj->extract();
   obj->plot();

}
