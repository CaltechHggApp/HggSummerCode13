// Main file for spin analysis. See the README.
// To change between 8TeV and 14TeV change this:
#define ENERGY 8;  // 8 or 14
// Make this 0 to test the code with 1 file, or 1 to run over all data
#define ALL 0;  // true/false


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

//#include "analyzerSpin.cc"
#include "class2.h"

using namespace std;
using namespace RooFit;

int main()
{
   (RooRandom::randomGenerator())->SetSeed(0);
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

//   int dataset = ENERGY;
//   int all = ALL;
//   TString filename = (dataset == 8) ? "histos8.root" : "histos14.root";
//   TTree *tree = makeTree(dataset, all);
//   HggSpin* t = new HggSpin(tree);
//   t->Loop(filename);



   Class2* obj = new Class2();
   obj->fitMC();
   obj->generate();
   obj->extract();
   obj->plot();
}
