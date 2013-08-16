#include <iostream>

#include "plotter.h"
#include "ReadConfig.hh"

using namespace std;
using namespace RooFit;

int main()
{
   ReadConfig cfgReader("config.cfg");

   int single = atoi(cfgReader.getParameter("make_single_plot").c_str());
   int multi = atoi(cfgReader.getParameter("make_multi_plot").c_str());

   float maxEta = atof(cfgReader.getParameter("maximum_eta").c_str());
   float lumi = atof(cfgReader.getParameter("luminosity").c_str());
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());
   int nToys = atoi(cfgReader.getParameter("number_of_toys").c_str());
   string mc_filename = cfgReader.getParameter("MC_filename");
   string luminosities = cfgReader.getParameter("luminosities");
   vector<string> luminositiesVec = cfgReader.tokenizeString(luminosities,",");
   string plot_filename = cfgReader.getParameter("plot_filename");


   cout<<"maxEta is "<<maxEta<<endl;
   cout<<"lumi is "<<lumi<<endl;
   cout<<"nBins is "<<nBins<<endl;
   cout<<"nToys is "<<nToys<<endl;



   vector<double> lumis;
   for(vector<string>::iterator it = luminositiesVec.begin(); it!=luminositiesVec.end(); it++)
   {
      double num = atof(it->c_str());
      lumis.push_back(num);
   }

   // Set seed
   (RooRandom::randomGenerator())->SetSeed(0);
   // Don't print dumb warnings
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

   plotter walter;
   // Make plots for a single toy
   walter.setMaxEta(maxEta);
   walter.setMCFilename(mc_filename);
   walter.setLumi(lumi);
   walter.setLumis(lumis);
   walter.setNBins(nBins);
   walter.setNToys(nToys);

   walter.makePdfs();
   if(single) walter.make_plot_of_toy();


   // Make plots for many toys
   if(multi)
   {
      walter.calculate();
      walter.make_plot_lumi(plot_filename);
   }

   return 0;
}
