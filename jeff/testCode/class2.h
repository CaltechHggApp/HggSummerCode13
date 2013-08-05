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

#include "MakeSpinSPlot.C"

using namespace std;
using namespace RooFit;

const int nsignal = 1000;
const int nbackground = 10000;

class Class2 {
public:

   RooRealVar *mass;
   RooRealVar *mean_sig_mass;
   RooRealVar *sigma_sig_mass;
   RooGaussian *model_sig_mass;

   RooRealVar *cosT;
   RooRealVar *p1_sig_cosT;
   RooPolynomial *model_sig_cosT;

   RooRealVar *c_bkg_mass;
   RooExponential *model_bkg_mass;

   RooRealVar *p1_bkg_cosT;
   RooRealVar *p2_bkg_cosT;
   RooPolynomial *model_bkg_cosT;

   RooRealVar *maxEta;


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


   RooDataSet *toyData;
   RooDataSet *extractedData;

   Class2();
   ~Class2();
   void fitMC();
   void generate();
   void extract();
   void plot();

};

Class2::Class2()
{
   mass = new RooRealVar("mass","",110,150);
   mean_sig_mass = new RooRealVar("mean_sig_mass","",125,80,160);
   sigma_sig_mass = new RooRealVar("sigma_sig_mass","",2,0,10);
   model_sig_mass = new RooGaussian("model_sig_mass","",*mass,*mean_sig_mass,*sigma_sig_mass);

   cosT = new RooRealVar("cosT","",0.,1.);
   p1_sig_cosT = new RooRealVar("p1",0,-2,2);
   model_sig_cosT = new RooPolynomial ("model_sig_cosT","",*cosT,RooArgList(*p1_sig_cosT));
   
   c_bkg_mass = new RooRealVar("c_bkg_mass","",-.05,-5,5);
   model_bkg_mass = new RooExponential("model_bkg_mass","",*mass,*c_bkg_mass);

   p1_bkg_cosT = new RooRealVar("p1_bkg_cosT","",-2,-5,5);
   p2_bkg_cosT = new RooRealVar("p2_bkg_cosT","",2,-1,2);
   model_bkg_cosT = new RooPolynomial("model_bkg_cosT","",*cosT,RooArgList(*p1_bkg_cosT,*p2_bkg_cosT));

   maxEta = new RooRealVar("maxEta","",0.,50.);

}


Class2::~Class2()
{
   delete mass;
   delete mean_sig_mass;
   delete sigma_sig_mass;
   delete model_sig_mass;
   delete cosT;
   delete p1_sig_cosT;
   delete c_bkg_mass;
   delete model_bkg_mass;
   delete p1_bkg_cosT;
   delete p2_bkg_cosT;
   delete model_bkg_cosT;
   delete maxEta;
}


void Class2::fitMC()
{
   TFile *f = new TFile("histos8.root");
   TTree *tree = (TTree*)f->Get("tree");
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass,*maxEta),Import(*tree));
   int nEntries=MC->numEntries();

   int n_0_8=0;
   RooDataSet *data_0_8 = new RooDataSet("data_0_8","", *mass);
   for(int i=0; i<nEntries;i++)
   {
      double max_eta=MC->get(i)->getRealValue("maxEta");
      mass->setVal(MC->get(i)->getRealValue("mass"));
      if(max_eta < .8)
      {
         n_0_8++;
         data_0_8->add(*mass);
      }
   }

   model_sig_mass->fitTo(*data_0_8,PrintLevel(-1));
   sig_0_8 = sigma_sig_mass->getVal();
   p_0_8 = (float)n_0_8 / (float)nEntries;


   int n_8_14=0;


}


void Class2::generate()
{
   RooArgSet set;
   set.add(*mass);
   set.add(*cosT);

   toyData = new RooDataSet("toyData","",set);

   sigma_sig_mass->setVal(sig_0_8);
   RooDataSet *genData_sig_mass = model_sig_mass->generate(*mass,nsignal);
   RooDataSet *genData_sig_cosT = model_sig_cosT->generate(*cosT,nsignal);

   for(int i=0;i<nsignal; i++)
   {
      mass->setVal(genData_sig_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_sig_cosT->get(i)->getRealValue("cosT"));
      toyData->add(set);
   }
   
   RooDataSet *genData_bkg_mass = model_bkg_mass->generate(*mass,nbackground);
   RooDataSet *genData_bkg_cosT = model_bkg_cosT->generate(*cosT,nbackground);


   for(int i=0; i<nbackground; i++)
   {
      mass->setVal(genData_bkg_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_bkg_cosT->get(i)->getRealValue("cosT"));
      toyData->add(set);
//      cout<<"mass is "<<mass->getVal()<<"   cosT is "<<cosT->getVal()<<endl;
   }
}


void Class2::extract()
{
   RooRealVar *sigYield = new RooRealVar("sigYield","",0,10000);
   RooRealVar *bkgYield = new RooRealVar("bkgYield","",0,100000);
   RooAddPdf *model_mass = new RooAddPdf("model_mass","",RooArgList(*model_sig_mass,*model_bkg_mass),RooArgList(*sigYield,*bkgYield));
   model_mass->fitTo(*toyData,PrintLevel(-1));

   MakeSpinSPlot splotter(toyData);
   splotter.addSpecies("signal",model_sig_mass,sigYield->getVal());
   splotter.addSpecies("background",model_bkg_mass,bkgYield->getVal());
   splotter.addVariable(mass);
   splotter.calculate();
   RooDataSet *sweights = splotter.getSWeightDataSet();
   sweights->SetName("sweights");

   RooRealVar *weight = new RooRealVar("weight","",-5.,5.);
   RooArgSet set;
   set.add(*mass);
   set.add(*cosT);
   set.add(*weight);

   extractedData = new RooDataSet("sig","",set,"weight");

   Long64_t nEntries = toyData->numEntries();
   for(int i=0;i<nEntries;i++)
   {
      double weight_double=0;
      weight_double += sweights->get(i)->getRealValue("signal_sw");
//      weight_double += sweights->get(i)->getRealValue("background_sw");

      mass->setVal(toyData->get(i)->getRealValue("mass"));
      cosT->setVal(toyData->get(i)->getRealValue("cosT"));
      extractedData->add(set,weight_double);
   }

}


void Class2::plot()
{
   RooPlot* frame = cosT->frame(Bins(40));
   extractedData->reduce("cosT")->plotOn(frame);
   extractedData->statOn(frame,Layout(0.55,0.99,0.8));
   TCanvas c1;
   frame->Draw();
   c1.SaveAs("plot.pdf");
}
