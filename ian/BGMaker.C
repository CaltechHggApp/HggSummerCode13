#include "MakeSpinFits.h"
#include "RooTrace.h"

using namespace std;

void MakeBackground(){
  RooDataSet Background_Combined("Background_Combined");
  for(std::vector<TString>::iterator lblit = lblvec.begin(); lblit != lblvec.end(); ++lblit){
    Background_Combined.append(*lblit + "_Combined")
  }
