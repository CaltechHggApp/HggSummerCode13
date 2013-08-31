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

   // Process config file
   ReadConfig cfgReader("config.cfg");
   int nBins = atoi(cfgReader.getParameter("number_of_bins").c_str());
   int nToys = atoi(cfgReader.getParameter("number_of_toys").c_str());
   int cheat = strcmp("false",cfgReader.getParameter("cheat").c_str() );
   int qqbar_percent = atoi(cfgReader.getParameter("qqbar_percent").c_str());
   string EB_res = cfgReader.getParameter("use_EB_res");
   string EE_res = cfgReader.getParameter("use_EE_res");
   string use_eta = cfgReader.getParameter("use_eta");


   string lumis_temp = cfgReader.getParameter("run_toys_at_these_lumi");
   vector<string> lumis_temp2 = cfgReader.tokenizeString(lumis_temp,",");
   vector<double> lumis;
   for(vector<string>::iterator it = lumis_temp2.begin(); it!=lumis_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      lumis.push_back(num);
   }

   string etas_temp = cfgReader.getParameter("eta_limits");
   vector<string> etas = cfgReader.tokenizeString(etas_temp,",");
   string EB_res_temp = cfgReader.getParameter("EB_resolutions");
   vector<string> EB_resolutions = cfgReader.tokenizeString(EB_res_temp,",");
   string EE_res_temp = cfgReader.getParameter("EE_resolutions");
   vector<string> EE_resolutions = cfgReader.tokenizeString(EE_res_temp,",");
   

   // Set seed
   (RooRandom::randomGenerator())->SetSeed(0);
   // Don't print dumb warnings
   RooMsgService::instance().setGlobalKillBelow(RooFit::WARNING);


   MakeToys walter;
   walter.setNBins(nBins);
   walter.setNToys(nToys);
   walter.setCheat(cheat);
   walter.set_qqbar_percent(qqbar_percent);

   for(vector<double>::iterator lumiIt=lumis.begin(); lumiIt!= lumis.end(); lumiIt++)
   {
      walter.setLumi(*lumiIt);

      // Run toys for eta plot
      walter.setResolution(EB_res,EE_res);
      for(int i=0; i<etas.size(); i++)
      {
         cout<<"Running toys for eta "<<etas[i]<<endl;
         walter.setMaxEta(etas[i]);
         walter.createPoint();
         cout<<endl;
      }


      // Run toys for res plot
      walter.setMaxEta(use_eta);   
      for(int i=0; i<EB_resolutions.size(); i++)
      {
         if( !strcmp(EB_resolutions[i].c_str(),EB_res.c_str()) && !strcmp(EE_resolutions[i].c_str(),EE_res.c_str()) ) continue; //this case is covered above
         cout<<"Running toys for res  EB="<<EB_resolutions[i]<<" EE="<<EE_resolutions[i]<<endl;
         walter.setResolution(EB_resolutions[i],EE_resolutions[i]);
         walter.createPoint();
         cout<<endl;
      }

   } // end loop over lumi

   stopwatch.Stop();
   cout<<"Real Time: "<<stopwatch.RealTime()/60.0<<" minutes"<<endl;
   cout<<"CPU Time: "<<stopwatch.CpuTime()/60.0<<" minutes"<<endl;
   return 0;
}
