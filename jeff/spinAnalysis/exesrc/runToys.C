#include <iostream>

#include "MakeToys.h"
#include "ReadConfig.hh"
#include "TStopwatch.h"

using namespace std;
using namespace RooFit;

int main()
{
   TStopwatch stopwatch;
   stopwatch.Start();

   // process config file
   ReadConfig cfgReader("config.cfg");
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());
   int nToys = atoi(cfgReader.getParameter("number_of_toys").c_str());
   int cheat = strcmp("false",cfgReader.getParameter("cheat").c_str() );
   string resLabel_for_etaPlot = cfgReader.getParameter("resLabel_for_etaPlot");
   string etaLabel_for_resPlot = cfgReader.getParameter("etaLabel_for_resPlot");  

   string eta_labels_temp = cfgReader.getParameter("eta_labels");
   vector<string> eta_labels = cfgReader.tokenizeString(eta_labels_temp,",");

   string res_labels_temp = cfgReader.getParameter("resolution_labels");
   vector<string> res_labels = cfgReader.tokenizeString(res_labels_temp,",");

   string lumis_temp = cfgReader.getParameter("luminosities");
   vector<string> lumis_temp2 = cfgReader.tokenizeString(lumis_temp,",");
   vector<double> lumis;
   for(vector<string>::iterator it = lumis_temp2.begin(); it!=lumis_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      lumis.push_back(num);
   }

   string etas_temp = cfgReader.getParameter("eta_limits");
   vector<string> etas_temp2 = cfgReader.tokenizeString(etas_temp,",");
   vector<double> etas;
   for(vector<string>::iterator it = etas_temp2.begin(); it!=etas_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      etas.push_back(num);
   }

   


   // Set seed
   (RooRandom::randomGenerator())->SetSeed(0);
   // Don't print dumb warnings
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

   MakeToys walter;
   walter.setLumis(lumis);
   walter.setNBins(nBins);
   walter.setNToys(nToys);
   walter.setCheat(cheat);


   // for eta plot
   walter.setMC0Filename( Form("./tempData/MC_0_%s.root",resLabel_for_etaPlot.c_str()) );
   walter.setMC2Filename( Form("./tempData/MC_2gg_%s.root",resLabel_for_etaPlot.c_str()) );
   for(int i=0; i<eta_labels.size(); i++)
   {
      string eta_label = eta_labels[i];
      walter.setMaxEta(etas[i]);
      cout<<"Running toys for eta "<<eta_label<<endl;

      walter.makePdfs();
      walter.calculate();
      walter.make_plot_lumi( Form("./tempData/toys_2gg_%s_%s.root",resLabel_for_etaPlot.c_str(),eta_label.c_str()) );
      cout<<endl;
   }


   // for resolution plot
   walter.setMaxEta(2.5); //flag!
   for(int i=0; i<res_labels.size(); i++)
   {
      string res_label = res_labels[i];
      if(!strcmp(res_label.c_str(),resLabel_for_etaPlot.c_str())) continue; //this case is covered above
      cout<<"Running toys for res "<<res_label<<endl;

      walter.setMC0Filename( Form("./tempData/MC_0_%s.root",res_label.c_str()) );
      walter.setMC2Filename( Form("./tempData/MC_2gg_%s.root",res_label.c_str()) );
      walter.makePdfs();
      walter.calculate();
      walter.make_plot_lumi( Form("./tempData/toys_2gg_%s_%s.root",res_label.c_str(),etaLabel_for_resPlot.c_str()) );
      cout<<endl;
   }


   // for gg vs. qqbar
   walter.setMC0Filename(Form("./tempData/MC_0_%s.root",resLabel_for_etaPlot.c_str()) );
   walter.setMC2Filename(Form("./tempData/MC_2qq_%s.root",resLabel_for_etaPlot.c_str()) );
   walter.setMaxEta(2.5);//flag
   walter.makePdfs();
   walter.calculate();
   walter.make_plot_lumi( Form("./tempData/toys_2qq_%s_%s.root",resLabel_for_etaPlot.c_str(),etaLabel_for_resPlot.c_str()) );
   cout<<endl;


   stopwatch.Stop();
   cout<<"Real Time: "<<stopwatch.RealTime()/60.0<<" minutes"<<endl;
   cout<<"CPU Time: "<<stopwatch.CpuTime()/60.0<<" minutes"<<endl;
   return 0;
}
