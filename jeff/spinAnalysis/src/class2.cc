#include "../include/class2.h"
#include "MakeSpinSPlot.C"


void Class2::calculateNSignal(float lumi, float nbkg)
{
   double crossSection = 49.85e-12;
   double BR = 2.28e-3;
   nSignal_gen = 1000;//lumi * 1.0e15 * crossSection * BR;
   nBackground_gen = nbkg;
}


void Class2::create_signal_pdfs()
{
   RooGaussian *model_sig_mass = new RooGaussian("model_sig_mass","",*mass,*mean_sig_mass,*sigma_sig_mass);
   RooExponential *model_bkg_mass = new RooExponential("model_bkg_mass","",*mass,*c_bkg_mass);
   RooPolynomial *model_bkg_cosT = new RooPolynomial("model_bkg_cosT","",*cosT,RooArgList(*p1_bkg_cosT,*p2_bkg_cosT));






   TFile *f = new TFile("./lib/MC_output_0.root");
   TTree *tree = (TTree*)f->Get("tree");

   RooRealVar *pt1 = new RooRealVar("pt1","",0,500);
   RooRealVar *pt2 = new RooRealVar("pt2","",0,500);
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass,*cosT,*maxEta,*pt1,*pt2),Import(*tree));
   RooDataSet *acceptedData = new RooDataSet("acceptedData","",RooArgSet(*mass,*cosT));

   const RooArgSet *event;
   int nEntries=MC->numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = MC->get(i);
      if(event->getRealValue("pt1")<24) continue;
      if(event->getRealValue("pt2")<16) continue;
      double max_eta = event->getRealValue("maxEta");
      if (max_eta>3.0) continue;
      
      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      acceptedData->add(RooArgSet(*mass,*cosT));
   }
    
   model_sig_mass->fitTo(*(acceptedData->reduce("mass")),PrintLevel(-1));
   signalWidth = sigma_sig_mass->getVal();
   signalEfficiency = (float)acceptedData->numEntries() / (float)nEntries;

   RooDataHist hist0("hist0","",*cosT, *(acceptedData->reduce("cosT")));

   model_sig0_cosT = new RooHistPdf("model_sig0_cosT","",*cosT,hist0);



   genData_sig_cosT = new RooDataSet("genData_sig_cosT","",*cosT);
   genData_sig_cosT=model_sig0_cosT->generate(*cosT,nSignal_gen,AutoBinned(false));
   cout<<"generated "<<genData_sig_cosT->numEntries()<<" signal cosT events"<<endl;

   RooPlot* frame = cosT->frame();
   genData_sig_cosT->plotOn(frame);
   TCanvas c1;
   frame->Draw();
   c1.SaveAs("gen_cos_sig.pdf");

/*
   RooPlot* frame2 = cosT->frame();
   model_sig0_cosT->plotOn(frame2);
   TCanvas c2;
   frame2->Draw();
   c2.SaveAs("model_sig0_cosT.pdf");
*/
   ws->import(*model_sig0_cosT);
   ws->import(*model_sig_mass);
   ws->import(*model_bkg_mass);
   ws->import(*model_bkg_cosT);
}

void Class2::create_spin2_pdf()
{
   TFile *f = new TFile("./lib/MC_output_2.root");
   TTree *tree2 = (TTree*)f->Get("tree");

   RooRealVar *pt1 = new RooRealVar("pt1","",0,500);
   RooRealVar *pt2 = new RooRealVar("pt2","",0,500);
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass,*cosT,*maxEta,*pt1,*pt2),Import(*tree2));
   RooDataSet *acceptedData = new RooDataSet("acceptedData","",RooArgSet(*mass,*cosT));

   const RooArgSet *event;
   int nEntries=MC->numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = MC->get(i);
      if(event->getRealValue("pt1")<24) continue;
      if(event->getRealValue("pt2")<16) continue;
      double max_eta = event->getRealValue("maxEta");
      if (max_eta>3.0) continue;

      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      acceptedData->add(RooArgSet(*mass,*cosT));
   }

   RooDataHist hist2("hist2","",*cosT, *(acceptedData->reduce("cosT")));

   model_sig2_cosT = new RooHistPdf("model_sig2_cosT","",*cosT,hist2);
   ws->import(*model_sig2_cosT);
}




void Class2::generate()
{
   RooArgSet event;
   event.add(*mass);
   event.add(*cosT);

   toyData = new RooDataSet("toyData","",event);

   int create_n = (int)round(nSignal_gen);//; * signalEfficiency);
   RooDataSet *genData_sig_mass;

   sigma_sig_mass->setVal(signalWidth);
   genData_sig_mass = ws->pdf("model_sig_mass")->generate(*mass,create_n);
   cout<<"generated "<<genData_sig_mass->numEntries()<<" signal mass events"<<endl;

   cout<<"generating "<<create_n<<" signal events"<<endl;
   for(int i=0; i<create_n; i++)
   {
      mass->setVal(genData_sig_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_sig_cosT->get(i)->getRealValue("cosT"));
      toyData->add(event);
   }

   RooDataSet *genData_bkg_mass = ws->pdf("model_bkg_mass")->generate(*mass,nbackground);
   RooDataSet *genData_bkg_cosT = ws->pdf("model_bkg_cosT")->generate(*cosT,nbackground);

   cout<<"generating background events"<<endl;
   for(int i=0; i<nBackground_gen; i++)
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
   RooAddPdf *model_mass = new RooAddPdf("model_mass","",RooArgList( *(ws->pdf("model_sig_mass")), *(ws->pdf("model_bkg_mass")) ),RooArgList(*(sigYield),*(bkgYield)));
   model_mass->fitTo(*toyData,PrintLevel(-1));
   signalYield = sigYield->getVal();
   backgroundYield = bkgYield->getVal();

   cout<<"\nSignal yield is "<<signalYield<<endl;
   cout<<"Bkg yield is    "<<backgroundYield<<"\n"<<endl;
}

void Class2::extractSignal()
{
   MakeSpinSPlot splotter(toyData);
   splotter.addSpecies("signal",ws->pdf("model_sig_mass"),signalYield);
splotter.addSpecies("background",ws->pdf("model_bkg_mass"),backgroundYield);
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
   RooAbsPdf *modelCos0 = ws->pdf("model_sig0_cosT");

   ws->Print();

   RooPlot* frame = cosT->frame();
   modelCos0->plotOn(frame);
   TCanvas c;
   frame->Draw();
   c.SaveAs("cos0pdf.pdf");



   RooPlot* frame1 = cosT->frame();
   extractedData->reduce("cosT")->plotOn(frame1);
   extractedData->statOn(frame1,Layout(0.55,0.99,0.8));
   modelCos0->plotOn(frame1);
   TCanvas c1;
   frame1->Draw();
   c1.SaveAs(plot_dir+"/extracted_sig_cosT.pdf");

   RooPlot* frame2 = mass->frame();
   extractedData->reduce("mass")->plotOn(frame2);
   extractedData->statOn(frame2,Layout(0.55,0.99,0.8));
   TCanvas c2;
   frame2->Draw();
   c2.SaveAs(plot_dir+"/extracted_sig_mass.pdf");

   RooPlot* frame3 = cosT->frame();
   toyData->reduce("cosT")->plotOn(frame3);
   toyData->statOn(frame3,Layout(0.55,0.99,0.8));
   TCanvas c3;
   frame3->Draw();
   c3.SaveAs(plot_dir+"/toyData_cosT.pdf");

   RooPlot* frame4 = mass->frame();
   toyData->reduce("mass")->plotOn(frame4);
   toyData->statOn(frame4,Layout(0.55,0.99,0.8));
   TCanvas c4;
   frame4->Draw();
   c4.SaveAs(plot_dir+"/toyData_mass.pdf");
}
