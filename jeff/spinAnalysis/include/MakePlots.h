#ifndef MakePlots_h
#define MakePlots_h

#include "TFile.h"
#include "TCanvas.h"
#include "TGraphAsymmErrors.h"
#include "TLine.h"
#include "TLatex.h"
#include "TAxis.h"
#include "TLegend.h"
#include "TPad.h"
#include "iostream"

using namespace std;

void plot_eta(vector<double>, vector<string>); // pval vs lumi. different color dots for different maxEta of the ECAL
void plot_res(); // pval vs lumi. different color dots for different resolutions of the ECAL
void plot_gg_qq(); // pval vs lumi. differnet color dots for gluon fusion and q qbar production

#endif
