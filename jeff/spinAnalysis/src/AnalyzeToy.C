#include "AnalyzeToy.h"
#include "MakeSpinSPlot.h"


void AnalyzeToy::setNSignal(double nsig)
{
   nSignal_gen = round(nsig);
   nBackground_gen = round(20 * nSignal_gen);
}


void AnalyzeToy::prepare_gen()
{
   genSpec_sig_mass = ws->pdf("model_sig_mass")->prepareMultiGen(*mass, NumEvents(nSignal_gen));
   genSpec_bkg_mass = ws->pdf("model_bkg_mass")->prepareMultiGen(*mass, NumEvents(nBackground_gen));
   genSpec_bkg_cosT = ws->pdf("model_bkg_cosT")->prepareMultiGen(*cosT, NumEvents(nBackground_gen));
}

void AnalyzeToy::generate_toy()
{
   RooDataSet *toySignal;
   toySignal = ws->pdf("model_sig_mass")->generate(*genSpec_sig_mass);
   toySignal->merge(ws->pdf("model_sig0_cosT")->generate(*cosT,nSignal_gen,AutoBinned(false))); 

   RooDataSet *toyBkg;
   toyBkg= ws->pdf("model_bkg_mass")->generate(*genSpec_bkg_mass);
   toyBkg->merge(ws->pdf("model_bkg_cosT")->generate(*genSpec_bkg_cosT));

   toyData = new RooDataSet("toyData","",RooArgSet(*mass, *cosT));
   toyData->append(*toySignal);
   toyData->append(*toyBkg);

   delete toySignal;
   delete toyBkg;
}

void AnalyzeToy::calculate_yield()
{
   if(cheat)
   {
      signalYield = nSignal_gen;
      backgroundYield = nBackground_gen;
   }
   else
   {
      RooRealVar tempSigYield("tempSigYield","",0,5 * nSignal_gen);
      RooRealVar tempBkgYield("tempBkgYield","",0,5 * nBackground_gen);
      RooAddPdf model_mass("model_mass","",RooArgList( *(ws->pdf("model_sig_mass")), *(ws->pdf("model_bkg_mass")) ),RooArgList(tempSigYield,tempBkgYield));
      model_mass.fitTo(*toyData,PrintLevel(-1));
      signalYield = round(tempSigYield.getVal());
      backgroundYield = round(tempBkgYield.getVal());
   }
}

void AnalyzeToy::extract_signal()
{
   calculate_yield();
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


void AnalyzeToy::plot()
{
   RooPlot* frame1 = cosT->frame();
   extractedData->reduce("cosT")->plotOn(frame1);
//   extractedData->statOn(frame1,Layout(0.55,0.99,0.8));
   RooAbsPdf *model_sig2_cosT = ws->pdf("model_sig2_cosT");
   model_sig2_cosT->plotOn(frame1);
   TCanvas c1;
   frame1->Draw();
   c1.SaveAs("./plots/extracted_sig_cosT.pdf");

/*
   RooPlot* frame2 = mass->frame();
   extractedData->reduce("mass")->plotOn(frame2);
   extractedData->statOn(frame2,Layout(0.55,0.99,0.8));
   TCanvas c2;
   frame2->Draw();
   c2.SaveAs(plot_dir+"/extracted_sig_mass.pdf");
*/

   RooPlot* frame3 = cosT->frame();
   toyData->reduce("cosT")->plotOn(frame3);
//   toyData->statOn(frame3,Layout(0.55,0.99,0.8));
   TCanvas c3;
   frame3->Draw();
   c3.SaveAs("./plots/toyData_cosT.pdf");

   RooPlot* frame4 = mass->frame();
   toyData->reduce("mass")->plotOn(frame4);
//   toyData->statOn(frame4,Layout(0.55,0.99,0.8));
   TCanvas c4;
   frame4->Draw();
   c4.SaveAs("./plots/toyData_mass.pdf");
}


double AnalyzeToy::getPvalue()
{
   RooPlot* frame1 = cosT->frame();
   RooAbsData *data = extractedData->reduce("cosT");
   RooDataHist hist3("hist3","",*cosT,*data);
   RooAbsPdf *model_sig2_cosT = ws->pdf("model_sig2_cosT");
   hist3.plotOn(frame1);
   model_sig2_cosT->plotOn(frame1);

   int dof = cosT->getBins() - 1;
   double chi2 = frame1->chiSquare() * dof;
//   cout<<"-chi squared is "<<chi2<<endl;
   double pvalue = TMath::Prob(chi2,dof);
//   cout<<"-dof is"<<dof<<endl;
//   cout<<"-pvalue is "<< pvalue<<endl;
   return pvalue;
}


AnalyzeToy::AnalyzeToy()
{
   mass = new RooRealVar("mass","",110,140);
   cosT = new RooRealVar("cosT","",0.,1.);
   mass->setBins(30);
}


AnalyzeToy::~AnalyzeToy()
{
   delete mass;
   delete cosT;
   delete toyData;
   delete extractedData;
}
