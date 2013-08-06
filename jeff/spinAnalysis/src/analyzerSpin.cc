// analysis file

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

#include "../include/analyzerSpin.h"
#include "librarySpin.cc"

using namespace std;

void HggSpin::Loop(TString filename)
{
   TFile *outputFile = new TFile("./lib/"+filename, "recreate");
   TTree *tree = new TTree("tree","");
   Float_t mass;
   Float_t maxEta;
   tree->Branch("mass",&mass,"mass/F");
   tree->Branch("maxEta",&maxEta,"maxEta/F");

   Long_t nEvents = chain->GetEntries();
   cout<<"number of MC events is "<<nEvents<<endl;
   const int nTop = 2;
   for(Long_t i = 0; i < nEvents; i++) // loop over events
   {
      if(i % 10000 == 0) cout<<"Processing event "<<i<<endl;
      GetEntry(i);
      Float_t top_pt[nTop] = {0.};
      Float_t top_eta[nTop] = {0.};
      Float_t top_phi[nTop] = {0.};

      for(Int_t j = 0; j < nMc; j++) // loop over particles in a given event
      {
         if(statusMc[j] != 3) continue;
         if(idMc[j] != 22) continue;
         if(motherID[j] != 25) continue;

         Float_t thisPt = smear(pMc[j], etaMc[j]) * sin(thetaMc[j]);
         Float_t thisEta = etaMc[j];
         Float_t thisPhi = phiMc[j];

         // store 2 hightest pT photons
         for(int k=0; k<nTop; k++)
         {
            if(thisPt > top_pt[k])
            {
	       std::swap(thisPt,top_pt[k]);
	       std::swap(thisEta,top_eta[k]);
	       std::swap(thisPhi,top_phi[k]);
            }
         }

      } // end loop over particles in a given event

      if(top_pt[0] < 26.) continue;
      if(top_pt[1] < 16.) continue; // there aren't two photons in event
 
      TLorentzVector gamma1, gamma2, higgs;
      gamma1.SetPtEtaPhiM(top_pt[0], top_eta[0], top_phi[0], 0);
      gamma2.SetPtEtaPhiM(top_pt[1], top_eta[1], top_phi[1], 0);
      higgs = gamma1 + gamma2;

      mass = higgs.M();
      maxEta = max(fabs(gamma1.Eta()), fabs(gamma2.Eta()));

      tree->Fill();
   } // end loop over events

   outputFile->cd();
   outputFile->Write();
   outputFile->Close();

   cout<<"finished processing MC "<< endl;
}
