#include "analyzerSpin.h"
#include "librarySpin.h"

using namespace std;

HggSpin::HggSpin(TTree *tree)
{
   if(tree == 0)
   {
      cout<<"No input tree"<<endl;
   }
   if(strcmp(tree->GetName(),"ntp1") == 0)
   {
      Init_vecbos(tree);
   }
   if(strcmp(tree->GetName(),"Events") == 0)
   {
      Init_bacon(tree);
   }
}


HggSpin::~HggSpin()
{
   delete chain;
}

void HggSpin::Init_vecbos(TTree *tree)
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


void HggSpin::Init_bacon(TTree *tree)
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



void HggSpin::setResolution(float barrel, float endcap)
{
   EB_res = barrel;
   EE_res = endcap;
}


void HggSpin::Loop(TString filename)
{
   TFile *outputFile = new TFile(filename, "recreate");
   TTree *tree = new TTree("tree","");
   Float_t mass;
   Float_t cosT;
   Float_t maxEta;
   Float_t pt1;
   Float_t pt2;
   tree->Branch("mass",&mass,"mass/F");
   tree->Branch("cosT",&cosT,"cosT/F");
   tree->Branch("maxEta",&maxEta,"maxEta/F");
   tree->Branch("pt1",&pt1,"pt1/F");
   tree->Branch("pt2",&pt2,"pt2/F");
   
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
         if(!( (motherID[j] != 25) || (motherID[j] != 39)) ) continue;

         Float_t thisPt = smear(pMc[j], etaMc[j], EB_res, EE_res) * sin(thetaMc[j]);
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

      if(top_pt[1] == 0.) continue; // there aren't two photons in event
 
      TLorentzVector gamma1, gamma2, higgs;
      gamma1.SetPtEtaPhiM(top_pt[0], top_eta[0], top_phi[0], 0);
      gamma2.SetPtEtaPhiM(top_pt[1], top_eta[1], top_phi[1], 0);
      higgs = gamma1 + gamma2;

      mass = higgs.M();
      cosT = calculateAngle_cs(gamma1,gamma2);
      maxEta = max(fabs(gamma1.Eta()), fabs(gamma2.Eta()));
      pt1=gamma1.Pt();
      pt2=gamma2.Pt();
      tree->Fill();
   } // end loop over events

   outputFile->cd();
   outputFile->Write();
   outputFile->Close();

   cout<<"finished processing MC "<< endl;
}
