// header

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


   HggSpin(TTree *tree=0);
   ~HggSpin();
   void Init_8tev(TTree *tree);
   void Init_14tev(TTree *tree);
   void Loop(TString);
   void GetEntry(Long64_t);

};

HggSpin::HggSpin(TTree *tree)
{
   if(tree == 0)
   {
      cout<<"No input tree"<<endl;
   }
   if(strcmp(tree->GetName(),"ntp1") == 0)
   {
      Init_8tev(tree);
   }
   if(strcmp(tree->GetName(),"Events") == 0)
   {
      Init_14tev(tree);
   }
}

HggSpin::~HggSpin()
{
}

void HggSpin::Init_8tev(TTree *tree)
{
   chain = tree;

   chain->SetBranchAddress("nMc", &nMc);
   chain->SetBranchAddress("pMc", pMc);
   chain->SetBranchAddress("thetaMc", thetaMc);
   chain->SetBranchAddress("etaMc", etaMc);
   chain->SetBranchAddress("phiMc", phiMc);
   chain->SetBranchAddress("idMc", idMc);
   chain->SetBranchAddress("mothMc", mothMc);
   chain->SetBranchAddress("statusMc", statusMc);
}

void HggSpin::Init_14tev(TTree *tree)
{
   chain = tree;
   chain->SetMakeClass(1);

   chain->SetBranchAddress("genVertexX", &genVertexX);
   chain->SetBranchAddress("genVertexY", &genVertexY);
   chain->SetBranchAddress("genVertexZ", &genVertexZ);
   chain->SetBranchAddress("GenParticle", &nMc, &b_GenParticle_);
   chain->SetBranchAddress("GenParticle.pt", GenParticle_pt, &b_GenParticle_pt);
   chain->SetBranchAddress("GenParticle.eta", etaMc, &b_GenParticle_eta);
   chain->SetBranchAddress("GenParticle.phi", phiMc, &b_GenParticle_phi);
   chain->SetBranchAddress("GenParticle.mass", GenParticle_mass, &b_GenParticle_mass);
   chain->SetBranchAddress("GenParticle.pdgid", idMc, &b_GenParticle_pdgid);
   chain->SetBranchAddress("GenParticle.status", statusMc, &b_GenParticle_status);
   chain->SetBranchAddress("GenParticle.motherPdgID", motherID, &b_GenParticle_motherPdgID)\
      ;
   chain->SetBranchAddress("GenJet", &GenJet_, &b_GenJet_);
   chain->SetBranchAddress("Electron", &Electron_, &b_Electron_);
   chain->SetBranchAddress("Muon", &Muon_, &b_Muon_);
   chain->SetBranchAddress("PFJet", &PFJet_, &b_PFJet_);
   chain->SetBranchAddress("Photon", &Photon_, &b_Photon_);
   chain->SetBranchAddress("PFCandidate", &PFCandidate_, &b_PFCandidate_);
   chain->SetBranchAddress("Vertex", &Vertex_, &b_Vertex_);
}

void HggSpin::GetEntry(Long64_t entry)
{
   chain->GetEntry(entry);

   if(strcmp(chain->GetName(), "ntp1") == 0)
   {
      for(int i=0;i<nMc;i++)
      {
         motherID[i] =idMc[mothMc[i]];
      }
   }
   if(strcmp(chain->GetName(), "Events") == 0)
   {
      for (int i=0; i<nMc; i++)
      {
         TLorentzVector particle;
         particle.SetPtEtaPhiM(GenParticle_pt[i], etaMc[i], phiMc[i], GenParticle_mass[i]);
         pMc[i] = particle.P();
         thetaMc[i] = particle.Theta();
      }
   }
}


