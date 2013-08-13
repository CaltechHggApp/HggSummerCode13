#ifndef class2_h
#define class2_h

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

class Class2 {
public:
   RooWorkspace *ws;
   RooRealVar *mass;
   RooRealVar *cosT;

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

   Class2();
   ~Class2();
   void setNBins(int nbins) { cosT->setBins(nbins);}
   void setNSignal(double);
   void setPdfs(RooWorkspace *workspace) {ws = workspace;}
   void prepare_gen();
   void generate_toy();
   void calculate_yield();
   void extract_signal();
   void plot(TString);
   double getPvalue();

};

#endif
