#ifndef MakeToys_h
#define MakeToys_h

#include "TCanvas.h"
#include <RooWorkspace.h>
#include <RooRealVar.h>
#include <RooDataSet.h>
#include <RooGaussian.h>
#include <RooPolynomial.h>
#include <RooProdPdf.h>
#include <RooExponential.h>
#include <RooGlobalFunc.h>
#include <RooAbsPdf.h>
#include <RooAddPdf.h>
#include <RooAbsData.h>
#include <RooPlot.h>
#include "RooStats/SPlot.h"
#include "RooKeysPdf.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooBernstein.h"
#include "RooLinkedListIter.h"
#include "RooCBShape.h"
#include "RooSimultaneous.h"
#include "RooExtendPdf.h"
#include "RooRandom.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"

#include "TFile.h"
#include "TMatrixT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TError.h"
#include <numeric>
#include <vector>
#include "TGraphErrors.h"

using namespace std;
using namespace RooFit;

class MakeToys
{
public:
   double etaMax;
   TString MC_filename;
   double plotLumi;
   int nBins;
   int nToys;
   int cheat;

   RooWorkspace *ws;
   double acceptance_x_efficiency;

   RooRealVar *mass;
   RooRealVar *cosT;

   RooDataSet *MC0;
   RooDataSet *MC2;
   vector<double> lumi;
   vector<double> pvalueMedian;
   vector<double> pvalue1stQuartile;
   vector<double> pvalue3rdQuartile;


   MakeToys();
   ~MakeToys();
   void setMaxEta(float eta){etaMax=eta;}
   void setMCFilename(TString filename) {MC_filename = filename;}
   void setNBins(int nbins){nBins = nbins;}
   void setLumi(double luminosity) {plotLumi = luminosity;}
   void setCheat(int TrueFalse) {cheat = TrueFalse;}
   void setNToys(int n) {nToys = n;}
   void setLumis(vector<double> numbers) {lumi = numbers;}
   void makePdfs();
   void make_plot_of_toy();
   void calculate();
   void make_plot_lumi(TString filename);
   void make_plot_sigma();
   void readMC();
   RooDataSet* applyCuts(RooDataSet *originalData);
   double lumi_to_nsignal(double lumi);
};

#endif
