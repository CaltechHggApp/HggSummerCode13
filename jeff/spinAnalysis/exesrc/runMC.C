#include <iostream>
#include <string>

#include "ReadConfig.hh"
#include "AnalyzeMC.h"
#include "spinLibrary.h"

using namespace std;

int main()
{
   // Process config file
   ReadConfig cfgReader("config.cfg");
   int high_statistics = strcmp("false",cfgReader.getParameter("high_statistics").c_str() );

   string EB_res_temp = cfgReader.getParameter("EB_resolutions");
   vector<string> EB_resolutions = cfgReader.tokenizeString(EB_res_temp,",");

   string EE_res_temp = cfgReader.getParameter("EE_resolutions");
   vector<string> EE_resolutions = cfgReader.tokenizeString(EE_res_temp,",");


   AnalyzeMC analyzer;
   for(int i=0; i< EB_resolutions.size(); i++)  // loop over resolution settings
   { 
      double EB_res = atof(EB_resolutions[i].c_str());
      double EE_res = atof(EE_resolutions[i].c_str());
      cout<<"Starting resolution setting EB="<<EB_res<<" EE="<<EE_res<<endl;
      analyzer.setResolution(EB_res, EE_res);


      // Run over spin 0 data
      cout<<"Running over spin0 MC"<<endl;
      analyzer.setData(makeTree("0",high_statistics));
      analyzer.Loop( Form("./tempData/MC_0_%sEB_%sEE.root",EB_resolutions[i].c_str(),EE_resolutions[i].c_str()) );
      cout<<endl;

      // Run over spin 2gg data
      cout<<"Running over spin2gg MC"<<endl;
      analyzer.setData(makeTree("2gg",high_statistics));
      analyzer.Loop( Form("./tempData/MC_2gg_%sEB_%sEE.root",EB_resolutions[i].c_str(),EE_resolutions[i].c_str()) );
      cout<<endl;

      // Run over spin 2qq data
      cout<<"Running over spin2qq MC"<<endl;
      analyzer.setData(makeTree("2qq",high_statistics));
      analyzer.Loop( Form("./tempData/MC_2qq_%sEB_%sEE.root",EB_resolutions[i].c_str(),EE_resolutions[i].c_str()) );
      cout<<endl;
   }

   return 0;
}
