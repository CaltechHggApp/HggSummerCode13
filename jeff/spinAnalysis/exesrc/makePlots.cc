#include <iostream>

#include "plotter.h"
#include "ReadConfig.hh"

using namespace std;
using namespace RooFit;

int main()
{
   ReadConfig cfgReader("config.cfg");

   float maxEta = atof(cfgReader.getParameter("maximum_eta").c_str());
   float lumi = atof(cfgReader.getParameter("luminosity").c_str());
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());
   TString mc_filename = cfgReader.getParameter("MC_output_filename").c_str();
   TString plot_dir = cfgReader.getParameter("plot_directory").c_str();

   // Set seed
   (RooRandom::randomGenerator())->SetSeed(0);
   // Don't print dumb warnings
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);

   plotter walter;
   // Make plots for a single toy
   walter.setMaxEta(maxEta);
   walter.setMCFilename(mc_filename);
   walter.setPlotDirectory(plot_dir);
   walter.setLumi(lumi);
   walter.setNBins(nBins);

   walter.makePdfs();
   walter.make_plot_of_toy();


   // Make plots for many toys


   return 0;
}
