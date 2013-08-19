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




TTree* makeTree(TString spin, int high_statistics)
{
   int energy_0 = 14;
   int energy_2 = 8;

   TChain *chain = new TChain();
   if(!strcmp(spin,"0") && energy_0 ==8)
   {
      chain->SetName("ntp1");

      TString directory = "/mnt/tier2/store/user/amott/Vecbos2012/MC/V00-5_3_X/GluGluToHToGG_M-125_8TeV-powheg-pythia6/Summer12_DR53X-PU_S10_START53_V7A-v1/";

      chain->Add(directory + "default_MC_10_1_nt4.root");
      if (high_statistics)
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

   if(!strcmp(spin,"0") && energy_0 ==14)
   {
      chain->SetName("Events");

      TString directory = "/mnt/tier2/store/user/amott/LiteFormat2013/MC/V03-6_1_2_SLHC4_patch2/GluGluToHToGG_M-125_14TeV-powheg-pythia6/Summer12-PU50_POSTLS161_V12-v1//";

      chain->Add(directory + "BACONNtuple_10_1_lLB.root");
      if(high_statistics)
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


   if(!strcmp(spin,"2gg"))
   {
      chain->SetName("ntp1");

      TString directory = "/mnt/tier2/store/user/apresyan/Vecbos2012/MC/V06-5_3_X/Graviton2PMGluGluToHToGG_M-125_8TeV-jhu-pythia6/Summer12_DR53X-PU_S10_START53_V7C-v1/";

      chain->Add(directory + "default_MC_10_1_ZiX.root");
      if (high_statistics)
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

   if(!strcmp(spin,"2qq"))
   {
      chain->SetName("ntp1");

      TString directory = "/mnt/tier2/store/user/apresyan/Vecbos2012/MC/V06-5_3_X/RSGravGG_q\
q_kMpl001_M125_TuneZ2star_8TeV_pythia6/";

      chain->Add(directory + "default_MC_10_1_WCB.root");

      if(high_statistics)
      {
         chain->Add(directory + "default_MC_11_1_SV1.root");
         chain->Add(directory + "default_MC_12_1_UF0.root");
         chain->Add(directory + "default_MC_13_1_t4d.root");
         chain->Add(directory + "default_MC_14_1_DNT.root");
         chain->Add(directory + "default_MC_15_1_mDy.root");
         chain->Add(directory + "default_MC_16_1_HUR.root");
         chain->Add(directory + "default_MC_17_1_8SQ.root");
         chain->Add(directory + "default_MC_18_1_doF.root");
         chain->Add(directory + "default_MC_19_1_Udz.root");
         chain->Add(directory + "default_MC_1_1_xPN.root");
         chain->Add(directory + "default_MC_20_1_9Ox.root");
         chain->Add(directory + "default_MC_21_1_YxA.root");
         chain->Add(directory + "default_MC_22_1_5F2.root");
         chain->Add(directory + "default_MC_23_1_VxZ.root");
         chain->Add(directory + "default_MC_24_1_Xqp.root");
         chain->Add(directory + "default_MC_25_1_YhN.root");
         chain->Add(directory + "default_MC_26_1_HHN.root");
         chain->Add(directory + "default_MC_27_1_NjJ.root");
         chain->Add(directory + "default_MC_28_1_OHD.root");
         chain->Add(directory + "default_MC_29_1_Nos.root");
         chain->Add(directory + "default_MC_2_1_hot.root");
         chain->Add(directory + "default_MC_30_1_jmB.root");
         chain->Add(directory + "default_MC_31_1_Me3.root");
         chain->Add(directory + "default_MC_32_1_Cil.root");
         chain->Add(directory + "default_MC_33_1_YKx.root");
         chain->Add(directory + "default_MC_34_1_ITZ.root");
         chain->Add(directory + "default_MC_35_1_IoJ.root");
         chain->Add(directory + "default_MC_36_1_BqD.root");
         chain->Add(directory + "default_MC_37_1_ShK.root");
         chain->Add(directory + "default_MC_38_1_Kwf.root");
         chain->Add(directory + "default_MC_39_1_dKF.root");
         chain->Add(directory + "default_MC_3_1_Z8a.root");
         chain->Add(directory + "default_MC_40_1_uUp.root");
         chain->Add(directory + "default_MC_41_1_bJu.root");
         chain->Add(directory + "default_MC_42_1_hBu.root");
         chain->Add(directory + "default_MC_43_1_C1x.root");
         chain->Add(directory + "default_MC_44_1_3Fu.root");
         chain->Add(directory + "default_MC_45_1_VWm.root");
         chain->Add(directory + "default_MC_46_1_xxN.root");
         chain->Add(directory + "default_MC_47_1_BIK.root");
         chain->Add(directory + "default_MC_48_1_CXp.root");
         chain->Add(directory + "default_MC_49_1_S3G.root");
         chain->Add(directory + "default_MC_4_1_qFU.root");
         chain->Add(directory + "default_MC_50_1_cS4.root");
         chain->Add(directory + "default_MC_51_1_qR7.root");
         chain->Add(directory + "default_MC_52_1_LAZ.root");
         chain->Add(directory + "default_MC_53_1_xmk.root");
         chain->Add(directory + "default_MC_54_1_ZNv.root");
         chain->Add(directory + "default_MC_55_1_XYX.root");
         chain->Add(directory + "default_MC_56_1_1wC.root");
         chain->Add(directory + "default_MC_57_1_mY4.root");
         chain->Add(directory + "default_MC_59_1_sXr.root");
         chain->Add(directory + "default_MC_5_1_NK1.root");
         chain->Add(directory + "default_MC_60_1_aPC.root");
         chain->Add(directory + "default_MC_61_1_n25.root");
         chain->Add(directory + "default_MC_62_1_YX3.root");
         chain->Add(directory + "default_MC_63_1_QRD.root");
         chain->Add(directory + "default_MC_64_1_Bph.root");
         chain->Add(directory + "default_MC_65_1_wvo.root");
         chain->Add(directory + "default_MC_66_1_Jmi.root");
         chain->Add(directory + "default_MC_67_1_7Z3.root");
         chain->Add(directory + "default_MC_68_1_eaY.root");
         chain->Add(directory + "default_MC_69_1_2gt.root");
         chain->Add(directory + "default_MC_6_1_1e8.root");
         chain->Add(directory + "default_MC_70_1_Tux.root");
         chain->Add(directory + "default_MC_71_1_NnS.root");
         chain->Add(directory + "default_MC_72_1_0rA.root");
         chain->Add(directory + "default_MC_73_1_SoW.root");
         chain->Add(directory + "default_MC_74_1_nf9.root");
         chain->Add(directory + "default_MC_75_1_vOR.root");
         chain->Add(directory + "default_MC_76_1_NIm.root");
         chain->Add(directory + "default_MC_77_1_DZM.root");
         chain->Add(directory + "default_MC_78_1_zz7.root");
         chain->Add(directory + "default_MC_79_1_hEx.root");
         chain->Add(directory + "default_MC_7_1_tuT.root");
         chain->Add(directory + "default_MC_80_1_utj.root");
         chain->Add(directory + "default_MC_81_1_1B9.root");
         chain->Add(directory + "default_MC_82_1_92K.root");
         chain->Add(directory + "default_MC_83_1_9g3.root");
         chain->Add(directory + "default_MC_8_1_dhg.root");
         chain->Add(directory + "default_MC_9_1_X6v.root");
      }
   }

   return chain;
}
