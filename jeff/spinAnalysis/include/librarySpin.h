#ifndef librarySpin_h
#define librarySpin_h

#include <TROOT.h>
#include <TTree.h>
#include <TChain.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>


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

using namespace std;

float smear(float energy, float eta, float EB_res, float EE_res);

Float_t calculateHelicity(TLorentzVector gamma1, TLorentzVector gamma2);

Float_t calculateAngle_cs(TLorentzVector gamma1, TLorentzVector gamma2);

TTree* makeTree(int spin, int all);
#endif
