#include <iostream>
#include <string>

#include "ReadConfig.hh"
#include "AnalyzeMC.h"
#include "spinLibrary.h"

using namespace std;

int main()
{
   // process config file
   ReadConfig cfgReader("config.cfg");
   int high_statistics = strcmp("false",cfgReader.getParameter("high_statistics").c_str() );

   string labels_temp = cfgReader.getParameter("resolution_labels");
   vector<string> labels = cfgReader.tokenizeString(labels_temp,",");

   string EB_res_temp = cfgReader.getParameter("EB_resolutions");
   vector<string> EB_res_temp2 = cfgReader.tokenizeString(EB_res_temp,",");
   vector<double> EB_resolutions;
   for(vector<string>::iterator it = EB_res_temp2.begin(); it!=EB_res_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      EB_resolutions.push_back(num);
   }

   string EE_res_temp = cfgReader.getParameter("EE_resolutions");
   vector<string> EE_res_temp2 = cfgReader.tokenizeString(EE_res_temp,",");
   vector<double> EE_resolutions;
   for(vector<string>::iterator it = EE_res_temp2.begin(); it!=EE_res_temp2.end(); it++)
   {
      double num = atof(it->c_str());
      EE_resolutions.push_back(num);
   }




   for(int i=0; i< labels.size(); i++)
   { 
      cout<<"Starting resolution setting "<<i<<endl;
      AnalyzeMC analyzer;
      TString label = labels[i];
      double EB_res = EB_resolutions[i];
      double EE_res = EE_resolutions[i];

      // Run over spin 0 data
      cout<<"Running over spin0 MC"<<endl;
      analyzer.setData(makeTree("0",high_statistics));
      analyzer.setResolution(EB_res, EE_res);
      analyzer.Loop("./tempData/MC_0_" + label + ".root");
      cout<<endl;

      // Run over spin 2gg data
      cout<<"Running over spin2gg MC"<<endl;
      analyzer.setData(makeTree("2gg",high_statistics));
      analyzer.setResolution(EB_res, EE_res);
      analyzer.Loop("./tempData/MC_2gg_" + label + ".root");
      cout<<endl;

      // Run over spin 2qq data
      cout<<"Running over spin2qq MC"<<endl;
      analyzer.setData(makeTree("2qq",high_statistics));
      analyzer.setResolution(EB_res, EE_res);
      analyzer.Loop("./tempData/MC_2qq_" + label + ".root");
      cout<<endl;
   }

   return 0;
}
