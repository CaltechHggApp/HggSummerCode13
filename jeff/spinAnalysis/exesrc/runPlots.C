#include <iostream>
#include <stdlib.h>

#include "MakePlots.h"
#include "ReadConfig.hh"

using namespace std;

int main()
{
   //process config file
   ReadConfig cfgReader("config.cfg");
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());


   string etaLumi_temp = cfgReader.getParameter("use_lumi_");
   vector<string> etaLumi_temp2 = cfgReader.tokenizeString(etaLumi_temp,",");
   vector<double> etaLumis;
   for(vector<string>::iterator it = etaLumi_temp2.begin(); it!=etaLumi_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      etaLumis.push_back(num);
   }

   string resLumi_temp = cfgReader.getParameter("use_lumi");
   vector<string> resLumi_temp2 = cfgReader.tokenizeString(resLumi_temp,",");
   vector<double> resLumis;
   for(vector<string>::iterator it = resLumi_temp2.begin(); it!=resLumi_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      resLumis.push_back(num);
   }

   string etas_temp = cfgReader.getParameter("eta_limits");
   vector<string> etas = cfgReader.tokenizeString(etas_temp,",");
   string EB_res_temp = cfgReader.getParameter("EB_resolutions");
   vector<string> EB_resolutions = cfgReader.tokenizeString(EB_res_temp,",");
   string EE_res_temp = cfgReader.getParameter("EE_resolutions");
   vector<string> EE_resolutions = cfgReader.tokenizeString(EE_res_temp,",");

   plot_eta(etaLumis,etas);
//   plot_res(resLumis,EB_resolutions,EE_resolutions);
   return 0;
}
