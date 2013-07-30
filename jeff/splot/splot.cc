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

#include "TMatrixT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TError.h"

#include "MakeSpinSPlot.C"

using namespace std;
using namespace RooFit;

void plot(RooWorkspace*);
void weightData(RooWorkspace*);

void makeToyData()
{
   RooRealVar mass("mass","mass",110,150);
   RooRealVar cosT("cosT","cosT",0.,1.);

//signal
   RooRealVar invM_sig("invM_sig","mean",125,120,130);
   RooRealVar sigma_sig("sigma_sig","sigma",2,0,10);
   RooGaussian sigModel("sigModel","sig",mass,invM_sig,sigma_sig);
   
   RooRealVar a1("a1","a1",0,-1,1);
   RooPolynomial cosT_sig_model("cosT_sig_model","",cosT,RooArgList(a1));

//bkg
//   RooRealVar m0("m0","",0,-500,500);
//   RooRealVar m1("m1","", 0,-100,100);
//  RooPolynomial bkgModel("bkgModel","", mass,RooArgList(m0,m1));

   RooRealVar dec("dec","",-.05,-5,5);
   RooExponential bkgModel("bkgModel","",mass,dec);

   RooRealVar b0("b0","",-2,-5,5);//x^1
   RooRealVar b1("b1","b1",2,-1,2);//x^2
   RooPolynomial cosT_bkg_model("cosT_bkg_model","",cosT,RooArgList(b0,b1));

//combined model
   RooRealVar sigYield("sigYield","sigYield",1000,0.,10000);
   RooRealVar bkgYield("bkgYield","bkgYield",10000,0,1000000);


   RooArgSet set;
   set.add(mass);
   set.add(cosT);
   RooDataSet *data = new RooDataSet("data","some data",set);
  
   RooDataSet *sig_mass_data = sigModel.generate(mass,1000);
   RooDataSet *sig_cosT_data = cosT_sig_model.generate(cosT,1000);
   for(int i=0;i<1000;i++)
   {
      mass.setVal(sig_mass_data->get(i)->getRealValue("mass"));
      cosT.setVal(sig_cosT_data->get(i)->getRealValue("cosT"));
      data->add(set);
   }

   RooDataSet *bkg_mass_data = bkgModel.generate(mass,10000);
   RooDataSet *bkg_cosT_data = cosT_bkg_model.generate(cosT,10000);
   for(int i=0;i<10000;i++)
   {
      mass.setVal(bkg_mass_data->get(i)->getRealValue("mass"));
      cosT.setVal(bkg_cosT_data->get(i)->getRealValue("cosT"));
      data->add(set);
   }

   RooWorkspace *ws = new RooWorkspace("ws");
   ws->import(*data);
   ws->import(bkgModel);
   ws->import(sigModel);
   ws->import(sigYield);
   ws->import(bkgYield);

   weightData(ws);

}

void weightData(RooWorkspace* ws)
{
   RooRealVar *mass = ws->var("mass");
   RooRealVar *cosT = ws->var("cosT");
   RooDataSet *data = (RooDataSet*)ws->data("data");

   RooAbsPdf *bkgModel = ws->pdf("bkgModel");
   RooAbsPdf *sigModel = ws->pdf("sigModel");   

//determine yield
   RooRealVar *invM_sig = ws->var("invM_sig");
   RooRealVar *sigma_sig = ws->var("sigma_sig");
//   RooRealVar *m0 = ws->var("m0");
//   RooRealVar *m1 = ws->var("m1");
   RooRealVar *dec=ws->var("dec");


   RooRealVar *sigYield = ws->var("sigYield");
   RooRealVar *bkgYield = ws->var("bkgYield");

   RooAddPdf model("model","",RooArgList(*sigModel,*bkgModel),RooArgList(*sigYield,*bkgYield));

   RooRealVar b0("b0","",-5,5);
   RooRealVar b1("b1","b1",-1,2);
   RooPolynomial cosT_bkg_model("cosT_bkg_model","",*cosT,RooArgList(b0,b1));

   model.fitTo(*data,Extended(),PrintLevel(-1));
   cout<<"sigYield "<<sigYield->getVal()<<endl;
   cout<<"bkgYield "<<bkgYield->getVal()<<endl;

   MakeSpinSPlot splotter(data);
   splotter.addSpecies("signal",sigModel,sigYield->getVal());
   splotter.addSpecies("background",bkgModel,bkgYield->getVal());
   splotter.addVariable(mass);
   splotter.calculate();
   RooDataSet *sweights = splotter.getSWeightDataSet();
   sweights->SetName("sweights");

   RooRealVar *weight = new RooRealVar("weight","",-5.,5.);
   RooArgSet set;
   set.add(*mass);
   set.add(*cosT);
   set.add(*weight);

   RooDataSet *sig = new RooDataSet("sig","",set,"weight");

   for(int i=0;i<11000;i++)
   {
      double weight_double=0;
      weight_double += sweights->get(i)->getRealValue("signal_sw");
//      weight_double += sweights->get(i)->getRealValue("background_sw");

      mass->setVal(data->get(i)->getRealValue("mass"));
      cosT->setVal(data->get(i)->getRealValue("cosT"));
      if(mass->getVal()<130 && mass->getVal() > 120){
//	  cout<<" "<<sweights->get(i)->getRealValue("signal_sw")<<" "<<sweights->get(i)->getRealValue("background_sw")<<" "<<sweights->get(i)->getRealValue("signal_sw")+sweights->get(i)->getRealValue("background_sw")<<endl;
      }
      sig->add(set,weight_double);
   }

   ws->import(*sig);
   ws->Print();
   plot(ws);
}


void plot(RooWorkspace* ws)
{
   RooRealVar *mass = ws->var("mass");
   RooRealVar *cosT = ws->var("cosT");
   RooDataSet *data = (RooDataSet*)ws->data("sig");

   RooPlot* frame = cosT->frame(Bins(40));
   data->reduce("cosT")->plotOn(frame);
   data->statOn(frame,Layout(0.55,0.99,0.8));
   TCanvas c1;
   frame->Draw();
   c1.SaveAs("plot.pdf");
}


int main()
{
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);
   makeToyData();
   return 1;
}
