#include <iostream>

#include "MakeToys.h"
#include "ReadConfig.hh"

using namespace std;
using namespace RooFit;

int main()
{
   // process config file
   ReadConfig cfgReader("config.cfg");
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());
   int nToys = atoi(cfgReader.getParameter("number_of_toys").c_str());
   int cheat = strcmp("false",cfgReader.getParameter("cheat").c_str() );
   TString res_label = cfgReader.getParameter("use_res_label");

   string labels_temp = cfgReader.getParameter("eta_labels");
   vector<string> labels = cfgReader.tokenizeString(labels_temp,",");

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

   for(int i=0; i<labels.size(); i++)
   {
      string label = labels[i];
      walter.setMaxEta(etas[i]);

      walter.setMCFilename("./tempData/2gg_" + res_label + ".root");
      walter.makePdfs();
      walter.calculate();
      walter.make_plot_lumi( Form("./tempData/eta%s_gg.root",label.c_str()) );

      walter.setMCFilename("./tempData/2qq_" + res_label + ".root");
      walter.makePdfs();
      walter.calculate();
      walter.make_plot_lumi( Form("./tempData/eta%s_qq.root",label.c_str()) );
   }

   return 0;
}
