#include "spinLibrary.h"

using namespace std;


float smear(float energy, float eta, float EB_res, float EE_res)
{
   TRandom3 random(0);
   if(fabs(eta) < 1.4)
   {
      return energy * (1 + random.Gaus(0,EB_res));
   }
   if(fabs(eta) > 1.4)
   {
      return energy * (1 + random.Gaus(0,EE_res));
   }
}


Float_t calculateHelicity(TLorentzVector gamma1, TLorentzVector gamma2)
{
   TLorentzVector higgs = gamma1 + gamma2;
   TVector3 boost = -1 * higgs.BoostVector();
   gamma1.Boost(boost);
   Float_t cos_theta = (higgs.Vect() * gamma1.Vect()) / (higgs.Vect().Mag() * gamma1.Vect().\
							 Mag());
   return fabs(cos_theta);
}

Float_t calculateAngle_cs(TLorentzVector gamma1, TLorentzVector gamma2)
{
   TLorentzVector higgs = gamma1 + gamma2;
   TVector3 boost = -1 * higgs.BoostVector();
   TLorentzVector b1; b1.SetPxPyPzE(0,0,4000,4000);
   TLorentzVector b2; b2.SetPxPyPzE(0,0,-4000,4000);

   b1.Boost(boost);
   b2.Boost(boost);
   gamma1.Boost(boost);

   TVector3 gamma1_unit = gamma1.Vect().Unit();
   TVector3 b1_unit = b1.Vect().Unit();
   TVector3 b2_unit = b2.Vect().Unit();
   TVector3 directionCS = (b1_unit - b2_unit).Unit();

   return fabs(cos(directionCS.Angle(gamma1_unit)));
}




TTree* makeTree(int spin, int all)
{
   int energy_0 = 14;
   int energy_2 = 8;
   int local = 0;

   TChain *chain = new TChain();
   if(spin == 0 && energy_0 ==8)
   {
      chain->SetName("ntp1");

      TString directory = "/mnt/tier2/store/user/amott/Vecbos2012/MC/V00-5_3_X/GluGluToHToGG_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/";

      chain->Add(directory + "default_MC_10_1_nt4.root");
      if (all && !local)
      {
         chain->Add(directory + "default_MC_11_1_UJo.root");
         chain->Add(directory + "default_MC_1_1_ZVS.root");
         chain->Add(directory + "default_MC_12_1_iqQ.root");
         chain->Add(directory + "default_MC_13_1_cOM.root");
         chain->Add(directory + "default_MC_14_1_40A.root");
         chain->Add(directory + "default_MC_15_1_MXM.root");
         chain->Add(directory + "default_MC_16_1_LCO.root");
         chain->Add(directory + "default_MC_17_1_t2A.root");
         chain->Add(directory + "default_MC_18_1_GhC.root");
         chain->Add(directory + "default_MC_19_1_DtR.root");
         chain->Add(directory + "default_MC_20_1_Q9L.root");
         chain->Add(directory + "default_MC_21_1_nzp.root");
         chain->Add(directory + "default_MC_2_1_646.root");
         chain->Add(directory + "default_MC_22_1_R24.root");
         chain->Add(directory + "default_MC_3_1_USZ.root");
         chain->Add(directory + "default_MC_4_1_pdP.root");
         chain->Add(directory + "default_MC_5_1_9kX.root");
         chain->Add(directory + "default_MC_6_1_JbX.root");
         chain->Add(directory + "default_MC_7_1_48v.root");
         chain->Add(directory + "default_MC_8_1_oj2.root");
         chain->Add(directory + "default_MC_9_1_wzC.root");
      }
   }

   if(spin == 0 && energy_0 ==14)
   {
      chain->SetName("Events");

      TString directory = "/mnt/tier2/store/user/amott/LiteFormat2013/MC/V03-6_1_2_SLHC4_patch2/GluGluToHToGG_M-125_14TeV-powheg-pythia6/Summer12-PU50_POSTLS161_V12-v1//";
      if(local) chain->Add("~/Documents/MC/BACONNtuple_26_1_JKG.root");

      if(!local) chain->Add(directory + "BACONNtuple_10_1_lLB.root");
      if(all && !local)
      {
         chain->Add(directory + "BACONNtuple_11_2_VzI.root");
         chain->Add(directory + "BACONNtuple_12_2_zbd.root");
         chain->Add(directory + "BACONNtuple_13_1_f5m.root");
         chain->Add(directory + "BACONNtuple_14_2_o7c.root");
         chain->Add(directory + "BACONNtuple_15_4_1Fh.root");
         chain->Add(directory + "BACONNtuple_16_1_e2U.root");
         chain->Add(directory + "BACONNtuple_17_2_KgZ.root");
         chain->Add(directory + "BACONNtuple_18_1_3zF.root");
         chain->Add(directory + "BACONNtuple_19_2_OAU.root");
         chain->Add(directory + "BACONNtuple_1_1_Wl2.root");
         chain->Add(directory + "BACONNtuple_20_1_K9G.root");
         chain->Add(directory + "BACONNtuple_21_1_xWh.root");
         chain->Add(directory + "BACONNtuple_22_2_F8u.root");
         chain->Add(directory + "BACONNtuple_23_3_X7A.root");
         chain->Add(directory + "BACONNtuple_24_2_W94.root");
         chain->Add(directory + "BACONNtuple_25_1_1TD.root");
         chain->Add(directory + "BACONNtuple_26_1_JKG.root");
         chain->Add(directory + "BACONNtuple_27_3_QCz.root");
         chain->Add(directory + "BACONNtuple_28_2_4OQ.root");
         chain->Add(directory + "BACONNtuple_2_1_WhQ.root");
         chain->Add(directory + "BACONNtuple_3_1_Yj3.root");
         chain->Add(directory + "BACONNtuple_4_1_Fhp.root");
         chain->Add(directory + "BACONNtuple_5_1_LU8.root");
         chain->Add(directory + "BACONNtuple_6_1_dDw.root");
         chain->Add(directory + "BACONNtuple_7_1_hy6.root");
         chain->Add(directory + "BACONNtuple_8_1_No3.root");
         chain->Add(directory + "BACONNtuple_9_3_Ixn.root");
      }
   }


   if(spin == 2 && energy_2 ==8)
   {
      chain->SetName("ntp1");

      TString directory = "/mnt/tier2/store/user/apresyan/Vecbos2012/MC/V06-5_3_X/Graviton2PMGluGluToHToGG_M-125_8TeV-jhu-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/";
      if(local) chain->Add("~/Documents/MC/default_MC_27_1_fzS.root");

      if(!local) chain->Add(directory + "default_MC_10_1_ZiX.root");
      if (all && !local)
      {
         chain->Add(directory + "default_MC_11_1_0fA.root");
         chain->Add(directory + "default_MC_12_1_BpX.root");
         chain->Add(directory + "default_MC_13_1_qCK.root");
         chain->Add(directory + "default_MC_14_1_Rvl.root");
         chain->Add(directory + "default_MC_15_1_tE7.root");
         chain->Add(directory + "default_MC_16_1_et9.root");
         chain->Add(directory + "default_MC_17_1_XyL.root");
         chain->Add(directory + "default_MC_18_1_4Pf.root");
         chain->Add(directory + "default_MC_19_1_NjC.root");
         chain->Add(directory + "default_MC_1_1_33z.root");
         chain->Add(directory + "default_MC_20_1_0Fd.root");
         chain->Add(directory + "default_MC_21_1_Dpj.root");
         chain->Add(directory + "default_MC_22_1_SGf.root");
         chain->Add(directory + "default_MC_23_1_IsH.root");
         chain->Add(directory + "default_MC_24_1_EJQ.root");
         chain->Add(directory + "default_MC_25_1_jGg.root");
         chain->Add(directory + "default_MC_26_1_a3T.root");
         chain->Add(directory + "default_MC_27_1_fzS.root");
         chain->Add(directory + "default_MC_28_1_LI2.root");
         chain->Add(directory + "default_MC_29_1_4P0.root");
         chain->Add(directory + "default_MC_2_1_qCO.root");
         chain->Add(directory + "default_MC_30_1_13f.root");
         chain->Add(directory + "default_MC_31_1_iMx.root");
         chain->Add(directory + "default_MC_3_1_3Ha.root");
         chain->Add(directory + "default_MC_4_1_lm4.root");
         chain->Add(directory + "default_MC_5_1_rUJ.root");
         chain->Add(directory + "default_MC_6_1_OWc.root");
         chain->Add(directory + "default_MC_7_1_3m7.root");
         chain->Add(directory + "default_MC_8_1_Gum.root");
         chain->Add(directory + "default_MC_9_1_VGH.root");
      }
   }
   return chain;
}
