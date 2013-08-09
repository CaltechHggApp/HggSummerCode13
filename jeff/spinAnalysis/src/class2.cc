#include "../include/class2.h"
#include "MakeSpinSPlot.C"


void Class2::setNSignal(float lumi)
{
   double crossSection = 49.85e-12;
   double BR = 2.28e-3;
   nSignal_gen = lumi * 1.0e15 * crossSection * BR;
   nBackground_gen = round(10 * nSignal_gen);
   cout<<"-number of signal at this lumi: "<<nSignal_gen<<endl;
}


void Class2::apply_cuts(float etaCut)
{
   RooRealVar pt1("pt1","",0,500);
   RooRealVar pt2("pt2","",0,500);
   RooRealVar maxEta("maxEta","",0,50);
   const RooArgSet *event;
   int nEntries;

   // spin 0
   TFile *f = new TFile("./lib/MC_output_0.root");
   TTree *tree = (TTree*)f->Get("tree");

   RooDataSet MC0("MC0","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*tree));
   spin0Signal = new RooDataSet("spin0Signal","",RooArgSet(*mass,*cosT));

   nEntries=MC0.numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = MC0.get(i);
      if(event->getRealValue("pt1")<24) continue;
      if(event->getRealValue("pt2")<16) continue;
      double max_eta = event->getRealValue("maxEta");
      if (max_eta>etaCut) continue;

      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      spin0Signal->add(RooArgSet(*mass,*cosT));
   }
   signalEfficiency = (float)spin0Signal->numEntries() / (float)nEntries;

   // spin 2
   f = new TFile("./lib/MC_output_2.root");
   tree = (TTree*)f->Get("tree");

   RooDataSet MC2("MC2","",RooArgSet(*mass,*cosT,maxEta,pt1,pt2),Import(*tree));
   spin2Signal = new RooDataSet("spin2Signal","",RooArgSet(*mass,*cosT));

   nEntries=MC2.numEntries();
   for(int i=0; i<nEntries; i++)
   {
      event = MC2.get(i);
      if(event->getRealValue("pt1")<24) continue;
      if(event->getRealValue("pt2")<16) continue;
      double max_eta = event->getRealValue("maxEta");
      if (max_eta>etaCut) continue;

      mass->setVal(event->getRealValue("mass"));
      cosT->setVal(event->getRealValue("cosT"));
      spin2Signal->add(RooArgSet(*mass,*cosT));
   }
}

void Class2::create_pdfs()
{
   // signal pdf's
   RooGaussian model_sig_mass("model_sig_mass","",*mass,*mean_sig_mass,*sigma_sig_mass);
   model_sig_mass.fitTo(*(spin0Signal->reduce("mass")),PrintLevel(-1));
   signalWidth = sigma_sig_mass->getVal();

   RooDataHist hist0("hist0","",*cosT, *(spin0Signal->reduce("cosT")));
   RooHistPdf model_sig0_cosT("model_sig0_cosT","",*cosT,hist0);

   RooDataHist hist2("hist2","",*cosT, *(spin2Signal->reduce("cosT")));
   RooHistPdf model_sig2_cosT("model_sig2_cosT","",*cosT,hist2);


   // background pdf's
   RooExponential model_bkg_mass("model_bkg_mass","",*mass,*c_bkg_mass);
   RooPolynomial model_bkg_cosT("model_bkg_cosT","",*cosT,RooArgList(*p1_bkg_cosT,*p2_bkg_cosT));

   ws->import(model_sig_mass);
   ws->import(model_sig0_cosT);
   ws->import(model_sig2_cosT);
   ws->import(model_bkg_mass);
   ws->import(model_bkg_cosT);
}


void Class2::generate_toy()
{
   RooArgSet event;
   event.add(*mass);
   event.add(*cosT);

   toyData = new RooDataSet("toyData","",event);

   int create_n = (int)round(nSignal_gen * signalEfficiency);
   cout<<"-signal acceptance x efficiency is "<<signalEfficiency<<endl;
   RooDataSet *genData_sig_mass;
   RooDataSet *genData_sig_cosT;

   sigma_sig_mass->setVal(signalWidth);
   genData_sig_mass = ws->pdf("model_sig_mass")->generate(*mass,create_n);
   genData_sig_cosT = ws->pdf("model_sig0_cosT")->generate(*cosT,create_n,AutoBinned(false));

   cout<<"-generating "<<create_n<<" signal events"<<endl;
   for(int i=0; i<create_n; i++)
   {
      mass->setVal(genData_sig_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_sig_cosT->get(i)->getRealValue("cosT"));
      toyData->add(event);
   }

   RooDataSet *genData_bkg_mass = ws->pdf("model_bkg_mass")->generate(*mass,nBackground_gen);
   RooDataSet *genData_bkg_cosT = ws->pdf("model_bkg_cosT")->generate(*cosT,nBackground_gen);

   cout<<"-generating "<<nBackground_gen<<" background events"<<endl;
   for(int i=0; i<nBackground_gen; i++)
   {
      mass->setVal(genData_bkg_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_bkg_cosT->get(i)->getRealValue("cosT"));
      toyData->add(event);
   }

}

void Class2::calculate_yield()
{
   RooRealVar tempSigYield("tempSigYield","",0,5 * nSignal_gen);
   RooRealVar tempBkgYield("tempBkgYield","",0,5 * nBackground_gen);
   RooAddPdf model_mass("model_mass","",RooArgList( *(ws->pdf("model_sig_mass")), *(ws->pdf("model_bkg_mass")) ),RooArgList(tempSigYield,tempBkgYield));
   model_mass.fitTo(*toyData,PrintLevel(-1));
   signalYield = round(tempSigYield.getVal());
   backgroundYield = round(tempBkgYield.getVal());

   cout<<"\n-signal yield is "<<signalYield<<endl;
   cout<<"-background yield is    "<<backgroundYield<<"\n"<<endl;
}

void Class2::extract_signal()
{
   MakeSpinSPlot splotter(toyData);
   splotter.addSpecies("signal",ws->pdf("model_sig_mass"),signalYield);
   splotter.addSpecies("background",ws->pdf("model_bkg_mass"),backgroundYield);
   splotter.addVariable(ws->var("mass"));
   splotter.calculate();
   RooDataSet *sweights = splotter.getSWeightDataSet();
   sweights->SetName("sweights");

   RooRealVar weight("weight","",-5.,5.);
   RooArgSet event;
   event.add(*mass);
   event.add(*cosT);
   event.add(weight);

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


void Class2::plot(TString plot_dir)
{

   RooPlot* frame1 = cosT->frame();
   extractedData->reduce("cosT")->plotOn(frame1);
   extractedData->statOn(frame1,Layout(0.55,0.99,0.8));
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


double Class2::getPvalue()
{
   RooPlot* frame1 = cosT->frame();
   RooAbsData *data = extractedData->reduce("cosT");
   RooDataHist hist3("hist3","",*cosT,*data);
   RooAbsPdf *model_sig2_cosT = ws->pdf("model_sig2_cosT");
   hist3.plotOn(frame1);
   model_sig2_cosT->plotOn(frame1);

   double chi2 = frame1->chiSquare();
   cout<<"\n-chi-squared is "<<chi2<<endl;
   int dof = cosT->getBins();
   double pvalue = TMath::Prob(chi2,dof-1);
   cout<<"-dof is "<<dof<<endl;
   cout<<"-pvalue is "<< pvalue<<endl;
   return pvalue;
}
