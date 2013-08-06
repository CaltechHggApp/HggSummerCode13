#include "../include/class2.h"

void Class2::fitMC()
{
   TFile *f = new TFile("./lib/histos8.root");
   TTree *tree = (TTree*)f->Get("tree");
   RooDataSet *MC = new RooDataSet("MC","",RooArgSet(*mass,*maxEta),Import(*tree));
   int nEntries=MC->numEntries();

   RooDataSet *data_in_range;
   for(std::vector< pair<float,float> >::iterator it=etaRange.begin(); it!=etaRange.end(); it++)
   {
      data_in_range = new RooDataSet("data_in_range","",*mass);
      float lowBound = it->first;
      float highBound = it->second;

      for(int i=0; i<nEntries;i++)
      {
	 double max_eta=MC->get(i)->getRealValue("maxEta");
	 mass->setVal(MC->get(i)->getRealValue("mass"));
	 if(max_eta > lowBound && max_eta < highBound)
	 {
	    data_in_range->add(*mass);
	 }
      }
      model_sig_mass->fitTo(*data_in_range,PrintLevel(-1));
      signalWidth.push_back(sigma_sig_mass->getVal());
      signalEfficiency.push_back((float)data_in_range->numEntries() / (float)nEntries);
      delete data_in_range;
   }
}


void Class2::generate()
{
   RooArgSet set;
   set.add(*mass);
   set.add(*cosT);

   toyData = new RooDataSet("toyData","",set);

   int create_n;
   RooDataSet *genData_sig_mass;
   RooDataSet *genData_sig_cosT;

   for(vector< pair<float,float> >::iterator it = etaRange.begin(); it!= etaRange.end(); it++)
   {
      sigma_sig_mass->setVal(signalWidth[it-etaRange.begin()]);
      create_n = (int)round(nsignal * signalEfficiency[it-etaRange.begin()]);
      genData_sig_mass = model_sig_mass->generate(*mass,create_n);
      genData_sig_cosT = model_sig_cosT->generate(*cosT,create_n);
      cout<<create_n<<endl;
      for(int i=0; i<create_n; i++)
      {
	 mass->setVal(genData_sig_mass->get(i)->getRealValue("mass"));
	 cosT->setVal(genData_sig_cosT->get(i)->getRealValue("cosT"));
	 toyData->add(set);
      }
   }

   RooDataSet *genData_bkg_mass = model_bkg_mass->generate(*mass,nbackground);
   RooDataSet *genData_bkg_cosT = model_bkg_cosT->generate(*cosT,nbackground);

   for(int i=0; i<nbackground; i++)
   {
      mass->setVal(genData_bkg_mass->get(i)->getRealValue("mass"));
      cosT->setVal(genData_bkg_cosT->get(i)->getRealValue("cosT"));
      toyData->add(set);
   }

}

void Class2::extract()
{
   RooRealVar *sigYield = new RooRealVar("sigYield","",0,5 * nsignal);
   RooRealVar *bkgYield = new RooRealVar("bkgYield","",0,5 * nbackground);
   RooAddPdf *model_mass = new RooAddPdf("model_mass","",RooArgList(*model_sig_mass,*model_bkg_mass),RooArgList(*sigYield,*bkgYield));
   model_mass->fitTo(*toyData,PrintLevel(-1));

   cout<<"\nSignal yield is "<<sigYield->getVal()<<endl;
   cout<<"Bkg yield is    "<<bkgYield->getVal()<<"\n"<<endl;

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

   extractedData = new RooDataSet("extractedData","",set,"weight");

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
