#include <iostream>
#include <string>

#include "ReadConfig.hh"
#include "analyzerSpin.h"
#include "librarySpin.h"

using namespace std;

int main()
{
   ReadConfig cfgReader("config.cfg");
   TString s_allMC = cfgReader.getParameter("high_statistics").c_str();
   TString s_EB_res = cfgReader.getParameter("EB_resolution").c_str();
   TString s_EE_res = cfgReader.getParameter("EE_resolution").c_str();
   TString filename = cfgReader.getParameter("MC_output_filename").c_str();

   int allMC = atoi(s_allMC);
   float EB_res = atof(s_EB_res);
   float EE_res = atof(s_EE_res);
   string bob("42");

   // Run over spin 0 data
   cout<<"Running over spin0 MC"<<endl;
   TTree *tree0 = makeTree(0, allMC);
   HggSpin analyzer0(tree0);
   analyzer0.setResolution(EB_res, EE_res);
   analyzer0.Loop("./lib/0_" + filename);

   // Run over spin 2 data
   cout<<"Running over spin2 MC"<<endl;
   TTree *tree2 = makeTree(2,allMC);
   HggSpin analyzer2(tree2);
   analyzer2.setResolution(EB_res, EE_res);
   analyzer2.Loop("./lib/2_" + filename);

   return 0;
}
