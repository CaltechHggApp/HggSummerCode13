#ifndef splotter_h
#define splotter_h

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

using namespace std;
using namespace RooFit;

class plotter
{
public:
   double etaMax;
   TString MC_filename;
   TString plot_dir;
   double plotLumi;
   int nBins;

   RooWorkspace *ws;
   double acceptance_x_efficiency;

   RooRealVar *mass;
   RooRealVar *cosT;

   plotter();
   ~plotter();
   void setMaxEta(float eta){etaMax=eta;}
   void setMCFilename(TString filename) {MC_filename = filename;}
   void setNBins(int nbins){nBins = nbins;}
   void setLumi(double luminosity) {plotLumi = luminosity;}
   void setPlotDirectory(TString dir) {plot_dir = dir;}
   void makePdfs();
   void make_plot_of_toy();
   void calculate();
   void make_plot_lumi();
   void make_plot_sigma();



   RooDataSet *MC0;
   RooDataSet *MC2;
   vector<double> lumi;
   vector<double> pvalueMean;
   vector<double> pvalueSigma;

   void readMC();
   RooDataSet* applyCuts(RooDataSet *originalData);
   double lumi_to_nsignal(double lumi);
};

#endif
