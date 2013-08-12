#ifndef analyzerSpin_h
#define analyzerSpin_h

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

const Int_t kMaxGenParticle = 500;

class HggSpin {
public :
   TTree   *chain;

   Int_t   nMc;
   Float_t pMc[kMaxGenParticle];
   Float_t thetaMc[kMaxGenParticle];
   Float_t etaMc[kMaxGenParticle];
   Float_t phiMc[kMaxGenParticle];
   Int_t idMc[kMaxGenParticle];
   Int_t mothMc[kMaxGenParticle];
   Int_t statusMc[kMaxGenParticle];

   Int_t motherID[kMaxGenParticle];

   Float_t         genVertexX;
   Float_t         genVertexY;
   Float_t         genVertexZ;
   Float_t GenParticle_pt[kMaxGenParticle];
   Float_t GenParticle_mass[kMaxGenParticle];
   Int_t   GenJet_;
   Int_t   Electron_;
   Int_t   Muon_;
   Int_t   PFJet_;
   Int_t   Photon_;
   Int_t   PFCandidate_;
   Int_t   Vertex_;


   TBranch        *b_GenParticle_;
   TBranch        *b_GenParticle_pt;
   TBranch        *b_GenParticle_eta;
   TBranch        *b_GenParticle_phi;
   TBranch        *b_GenParticle_mass;
   TBranch        *b_GenParticle_pdgid;
   TBranch        *b_GenParticle_status;
   TBranch        *b_GenParticle_motherPdgID;
   TBranch        *b_GenJet_;
   TBranch        *b_Electron_;
   TBranch        *b_Muon_;
   TBranch        *b_PFJet_;
   TBranch        *b_Photon_;
   TBranch        *b_PFCandidate_;
   TBranch        *b_Vertex_;

   float EB_res;
   float EE_res;

   HggSpin(TTree*);
   ~HggSpin();
   void Init_vecbos(TTree *tree);
   void Init_bacon(TTree *tree);
   void setResolution(float EB_res, float EE_res);
   void Loop(TString);
   void GetEntry(Long64_t);

};

#endif
