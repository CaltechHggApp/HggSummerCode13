#ifndef AnalyzeToy_h
#define AnalyzeToy_h

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

#include "TMatrixT.h"
#include "TMath.h"
#include "TH1F.h"
#include "TStyle.h"
#include "TError.h"
#include "TFile.h"
#include <numeric>
#include <vector>

using namespace std;
using namespace RooFit;

class AnalyzeToy {
public:
   RooWorkspace *ws;
   RooRealVar *mass;
   RooRealVar *cosT;

   int cheat;
   float signalYield;
   float backgroundYield;
   int nSignal_gen;
   int nBackground_gen;
   RooAbsPdf::GenSpec *genSpec_sig_mass;
   RooAbsPdf::GenSpec *genSpec_sig_cosT;
   RooAbsPdf::GenSpec *genSpec_bkg_mass;
   RooAbsPdf::GenSpec *genSpec_bkg_cosT;

   RooDataSet *toyData;
   RooDataSet *extractedData;
   double pvalue;

   AnalyzeToy();
   ~AnalyzeToy();
   void setNBins(int nbins) { cosT->setBins(nbins);}
   void setCheat(int TrueFalse) {cheat = TrueFalse;}
   void setPdfs(RooWorkspace *workspace) {ws = workspace;}
   void setNSignal(double);
   void prepare_gen();
   void run();
   void generate_toy();
   void calculate_yield();
   void extract_signal();
   void plot();
   void calculate_pval();
   double getPvalue() { return pvalue;}
};

#endif
