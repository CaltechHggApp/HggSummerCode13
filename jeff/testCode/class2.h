#include "TCanvas.h"
#include <RooWorkspace.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooPolynomial.h>
#include <RooProdPdf.h>
#include <RooExponential.h>
#include <RooGlobalFunc.h>
#include <RooAbsPdf.h>
#include <RooAddPdf.h>
#include <RooAbsData.h>
#include <RooPlot.h>
#include "RooStats/SPlot.h"
#include "RooKeysPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooBernstein.h"
#include "RooLinkedListIter.h"
#include "RooCBShape.h"
#include "RooSimultaneous.h"
#include "RooExtendPdf.h"
#include "RooRandom.h"

#include "TMatrixT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TError.h"
#include <numeric>

//#include "MakeSpinSPlot.C"

using namespace std;
using namespace RooFit;

class Class2 {
public:

   RooRealVar *mass;
   RooRealVar *mean_sig_mass;
   RooRealVar *sigma_sig_mass;
   RooGaussian *model_sig_mass;

   RooRealVar *cos;
   RooRealVar *p1_sig_cos;
   RooPolynomial *model_sig_cos;

   RooRealVar *c_bkg_mass;
   RooExponential *model_bkg_mass;

   RooRealVar *p1_bkg_cos;
   RooRealVar *p2_bkg_cos;
   RooPolynomial *model_bkg_cos;


   float p_0_8;
   float sig_0_8;
   float p_8_14;
   float sig_8_14;
   float p_157_20;
   float sig_157_20;
   float p_20_26;
   float sig_20_26;
   float p_26_30;
   float sig_26_30;
   float p_30_40;
   float sig_30_40;


   RooDataSet* toyData;

   Class2();
   ~Class2();
   void fitMC();
//   void generate();
//   void plot();

};

Class2::Class2()
{
   mass = new RooRealVar("mass","",110,150);
   mean_sig_mass = new RooRealVar("mean_sig_mass","",125,80,160);
   sigma_sig_mass = new RooRealVar("sigma_sig_mass","",2,0,10);
   model_sig_mass = new RooGaussian("model_sig_mass","",*mass,*mean_sig_mass,*sigma_sig_mass);

   cos = new RooRealVar("cos","",0.,1.);
   p1_sig_cos = new RooRealVar("p1",0,-2,2);
   model_sig_cos = new RooPolynomial ("model_sig_cos","",*cos,RooArgList(*p1_sig_cos));
   
   c_bkg_mass = new RooRealVar("c_bkg_mass","",-.05,-5,5);
   model_bkg_mass = new RooExponential("model_bkg_mass","",*mass,*c_bkg_mass);

   p1_bkg_cos = new RooRealVar("p1_bkg_cos","",-2,-5,5);
   p2_bkg_cos = new RooRealVar("p2_bkg_cos","",2,-1,2);
   model_bkg_cos = new RooPolynomial("model_bkg_cos","",*cos,RooArgList(*p1_bkg_cos,*p2_bkg_cos));



}


Class2::~Class2()
{
   delete mass;
   delete mean_sig_mass;
   delete sigma_sig_mass;
   delete model_sig_mass;
   delete cos;
   delete p1_sig_cos;
   delete c_bkg_mass;
   delete model_bkg_mass;
   delete p1_bkg_cos;
   delete p2_bkg_cos;
   delete model_bkg_cos;
}


void Class2::fitMC()
{
   TFile *f = new TFile("histos8.root");
   TTree *tree = (TTree*)f->Get("tree");
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass),Import(*tree));
   int nEntries=MC->numEntries();
   cout<<"enties is "<<nEntries<<endl;
   Float_t masss;
   tree->SetBranchAddress("mass",&masss);
//   tree->Print();
   for(int i=0;i<10;i++)
   {
      MC->get(i);
      cout<<mass->getVal()<<endl;
   }
         
}
