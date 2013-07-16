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

#include "analyzerSpin.h"
#include "librarySpin.h"

using namespace std;

void HggSpin::Loop(TString filename)
{
   TFile *outputFile = new TFile(filename,"recreate");


   TH1F *pt = new TH1F("pt", "p_{T}",50,0,110);
   TH1F *eta = new TH1F("eta", "#eta", 50, -10, 10);
   TH1F *phi = new TH1F("phi", "#phi", 50, -3.5, 3.5);
   TH1F *cosTheta = new TH1F("cosTheta", "|cos#theta|", 22, -0.05, 1.05);
   TH1F *mgg = new TH1F("mgg", "m_{#gamma#gamma}", 50, 115, 135);
   TH1F *maxEta = new TH1F("maxEta", "|#eta|_{max}", 52, -.2, 5);
   TH1F *ptgg = new TH1F("ptgg", "p^{T}_{#gamma#gamma}", 50,0,100);
   TH1F *etagg = new TH1F("etagg", "#eta^{T}_{#gamma#gamma}", 41,-.2,8);
   TH1F *maxEta_32_24 = new TH1F("maxEta_32_24", "", 52, -0.2,5);
   TH1F *maxEta_44_16 = new TH1F("maxEta_44_16", "", 52, -0.2,5);
   TH1F *maxEta_60_24 = new TH1F("maxEta_60_24", "", 52, -0.2,5);

   TH1F *cosTheta_32_24_25 = new TH1F("cosTheta_32_24_25","",22,-0.05,1.05);
   TH1F *cosTheta_32_24_30 = new TH1F("cosTheta_32_24_30","",22,-0.05,1.05);
   TH1F *cosTheta_32_24_40 = new TH1F("cosTheta_32_24_40","",22,-0.05,1.05);
   TH1F *cosTheta_32_24_14 = new TH1F("cosTheta_32_24_14","",22,-0.05,1.05);
   TH1F *cosTheta_44_16_25 = new TH1F("cosTheta_44_16_25","",22,-0.05,1.05);
   TH1F *cosTheta_44_16_30 = new TH1F("cosTheta_44_16_30","",22,-0.05,1.05);
   TH1F *cosTheta_44_16_40 = new TH1F("cosTheta_44_16_40","",22,-0.05,1.05);
   TH1F *cosTheta_44_16_14 = new TH1F("cosTheta_44_16_14","",22,-0.05,1.05);
   TH1F *cosTheta_60_24_25 = new TH1F("cosTheta_60_24_25","",22,-0.05,1.05);
   TH1F *cosTheta_60_24_30 = new TH1F("cosTheta_60_24_30","",22,-0.05,1.05);
   TH1F *cosTheta_60_24_40 = new TH1F("cosTheta_60_24_40","",22,-0.05,1.05);
   TH1F *cosTheta_60_24_14 = new TH1F("cosTheta_60_24_14","",22,-0.05,1.05);


   Long_t nEvents = chain->GetEntries();
   cout<<"nEvents is "<<nEvents<<endl;
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

         pt->Fill(thisPt);
         eta->Fill(thisEta);
         phi->Fill(thisPhi);

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

      if(top_pt[1] < 0.01) continue; // there aren't two photons in event
//      if(fabs(top_eta[0]) > 2.4) continue;
//      if(fabs(top_eta[1]) > 2.4) continue; 
 
      TLorentzVector gamma1, gamma2, higgs;
      gamma1.SetPtEtaPhiM(top_pt[0], top_eta[0], top_phi[0], 0);
      gamma2.SetPtEtaPhiM(top_pt[1], top_eta[1], top_phi[1], 0);
      higgs = gamma1 + gamma2;

      Float_t cos_theta = calculateHelicity(gamma1, gamma2);
      Float_t max_eta = max(fabs(gamma1.Eta()), fabs(gamma2.Eta()));

      cosTheta->Fill(cos_theta);
      maxEta->Fill(max_eta);
      mgg->Fill(higgs.M());
      ptgg->Fill(higgs.Pt());
      etagg->Fill(higgs.Eta());

         if(gamma1.Pt() > 32 && gamma2.Pt() > 24)
         {
            maxEta_32_24->Fill(max_eta);
            if(max_eta < 1.4) cosTheta_32_24_14->Fill(cos_theta);
            if(max_eta < 2.5) cosTheta_32_24_25->Fill(cos_theta);
            if(max_eta < 3.0) cosTheta_32_24_30->Fill(cos_theta);
            if(max_eta < 4.0) cosTheta_32_24_40->Fill(cos_theta);
         }
         if(gamma1.Pt() > 44 && gamma2.Pt() > 16)
         {
            maxEta_44_16->Fill(max_eta);
            if(max_eta < 1.4) cosTheta_44_16_14->Fill(cos_theta);
            if(max_eta < 2.5) cosTheta_44_16_25->Fill(cos_theta);
            if(max_eta < 3.0) cosTheta_44_16_30->Fill(cos_theta);
            if(max_eta < 4.0) cosTheta_44_16_40->Fill(cos_theta);
         }
         if(gamma1.Pt() > 60 && gamma2.Pt() > 24)
         {
            maxEta_60_24->Fill(max_eta);
            if(max_eta < 1.4) cosTheta_60_24_14->Fill(cos_theta);
            if(max_eta < 2.5) cosTheta_60_24_25->Fill(cos_theta);
            if(max_eta < 3.0) cosTheta_60_24_30->Fill(cos_theta);
            if(max_eta < 4.0) cosTheta_60_24_40->Fill(cos_theta);
         }
   } // end loop over events                                                                 

   outputFile->cd();
   outputFile->Write();
   outputFile->Close();

   cout<<"histograms written to " << filename << endl;
}
