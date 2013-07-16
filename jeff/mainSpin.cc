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


using namespace std;


int main()
{
   int dataset = 8; // 8 or 14 TeV samples
   int all = 0; // True:run over all data. False: just 1 file 
   TString filename = (dataset == 8) ? "histos8.root" : "histos14.root";
   TTree *tree = makeTree(dataset, all);
   HggSpin* t = new HggSpin(tree);
   t->Loop(filename);
}
