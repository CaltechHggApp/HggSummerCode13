#include <iostream>

#include "MakePlots.h"
#include "ReadConfig.hh"

using namespace std;

int main()
{
   //process config file
   ReadConfig cfgReader("config.cfg");
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());

   plot_eta();
   plot_res();
   plot_gg_qq();
   return 0;
}
