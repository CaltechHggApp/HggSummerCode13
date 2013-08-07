#include "../include/class2.h"
#include "MakeSpinSplot.C"


void Class2::calculateNSignal(float lumi, float nbkg)
{
   double sigma = 49.85e-12;
   double BR = 2.28e-3;
   nSignal = 1000;//lumi * 1.0e15 * sigma * BR;
   nBkg = nbkg;
//   cout<<"sig and bkg are "<<nSignal<<" "<<nBkg<<endl;
}


void Class2::fitMC()
{
   TFile *f = new TFile("./lib/histos8.root");
   TTree *tree = (TTree*)f->Get("tree");

   RooRealVar *pt1 = new RooRealVar("pt1","",0,500);
   RooRealVar *pt2 = new RooRealVar("pt2","",0,500);
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass,*maxEta,*pt1,*pt2),Import(*tree));
   RooDataSet *acceptedData = new RooDataSet("acceptedData","",*mass);

   const RooArgSet *event;
   int nEntries=MC->numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = MC->get(i);
//      if(event->getRealValue("pt1")<24) continue;
//      if(event->getRealValue("pt2")<16) continue;
      double max_eta = event->getRealValue("maxEta");
      if (max_eta>4.0) continue;
      
      mass->setVal(event->getRealValue("mass"));
      acceptedData->add(*mass);
   }
    
   model_sig_mass->fitTo(*acceptedData,PrintLevel(-1));
   signalWidth = sigma_sig_mass->getVal();
   signalEfficiency = (float)acceptedData->numEntries() / (float)nEntries;
}


void Class2::generate()
{
   RooArgSet event;
   event.add(*mass);
   event.add(*cosT);

   toyData = new RooDataSet("toyData","",event);

   int create_n = (int)round(3 * nSignal * signalEfficiency);
   RooDataSet *genData_sig_mass;
   RooDataSet *genData_sig_cosT;

   sigma_sig_mass->setVal(signalWidth);
   genData_sig_mass = model_sig_mass->generate(*mass,create_n);
   genData_sig_cosT = model_sig_cosT->generate(*cosT,create_n);
   cout<<"generating "<<create_n<<"signal events"<<endl;
   for(int i=0; i<create_n; i++)
   {
      mass->setVal(genData_sig_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_sig_cosT->get(i)->getRealValue("cosT"));
      toyData->add(event);
   }

   RooDataSet *genData_bkg_mass = model_bkg_mass->generate(*mass,nbackground);
   RooDataSet *genData_bkg_cosT = model_bkg_cosT->generate(*cosT,nbackground);

   for(int i=0; i<nBkg; i++)
   {
      mass->setVal(genData_bkg_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_bkg_cosT->get(i)->getRealValue("cosT"));
      toyData->add(event);
   }

}

void Class2::determineYield()
{
   RooRealVar *sigYield = new RooRealVar("sigYield","",0,5 * nsignal);
   RooRealVar *bkgYield = new RooRealVar("bkgYield","",0,5 * nbackground);
   RooAddPdf *model_mass = new RooAddPdf("model_mass","",RooArgList(*model_sig_mass,*model_bkg_mass),RooArgList(*sigYield,*bkgYield));
   model_mass->fitTo(*toyData,PrintLevel(-1));
   signalYield = sigYield->getVal();
   backgroundYield = bkgYield->getVal();

   cout<<"\nSignal yield is "<<signalYield<<endl;
   cout<<"Bkg yield is    "<<backgroundYield<<"\n"<<endl;
}

void Class2::extractSignal()
{
   MakeSpinSPlot splotter(toyData);
   splotter.addSpecies("signal",model_sig_mass,signalYield);
   splotter.addSpecies("background",model_bkg_mass,backgroundYield);
   splotter.addVariable(mass);
   splotter.calculate();
   RooDataSet *sweights = splotter.getSWeightDataSet();
   sweights->SetName("sweights");

   RooRealVar *weight = new RooRealVar("weight","",-5.,5.);
   RooArgSet event;
   event.add(*mass);
   event.add(*cosT);
   event.add(*weight);

   extractedData = new RooDataSet("extractedData","",event,WeightVar("weight"));

   Long64_t nEntries = toyData->numEntries();
   for(int i=0;i<nEntries;i++)
   {
      double weight_double=0;
      weight_double += sweights->get(i)->getRealValue("signal_sw");
//      weight_double += sweights->get(i)->getRealValue("background_sw");

      mass->setVal(toyData->get(i)->getRealValue("mass"));
      cosT->setVal(toyData->get(i)->getRealValue("cosT"));
      extractedData->add(event,weight_double);
   }
}


void Class2::plot(TString plot_dir = "./plots")
{
   RooPlot* frame1 = cosT->frame(Bins(40));
   extractedData->reduce("cosT")->plotOn(frame1);
   extractedData->statOn(frame1,Layout(0.55,0.99,0.8));
   TCanvas c1;
   frame1->Draw();
   c1.SaveAs(plot_dir+"/extracted_sig_cosT.pdf");

   RooPlot* frame2 = mass->frame(Bins(40));
   extractedData->reduce("mass")->plotOn(frame2);
   extractedData->statOn(frame2,Layout(0.55,0.99,0.8));
   TCanvas c2;
   frame2->Draw();
   c2.SaveAs(plot_dir+"/extracted_sig_mass.pdf");

   RooPlot* frame3 = cosT->frame(Bins(40));
   toyData->reduce("cosT")->plotOn(frame3);
   toyData->statOn(frame3,Layout(0.55,0.99,0.8));
   TCanvas c3;
   frame3->Draw();
   c3.SaveAs(plot_dir+"/toyData_cosT.pdf");

   RooPlot* frame4 = mass->frame(Bins(40));
   toyData->reduce("mass")->plotOn(frame4);
   toyData->statOn(frame4,Layout(0.55,0.99,0.8));
   TCanvas c4;
   frame4->Draw();
   c4.SaveAs(plot_dir+"/toyData_mass.pdf");
}
