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
#include <string>

using namespace std;
using namespace RooFit;

class MakeToys
{
public:
   int nBins;
   int nToys;
   int cheat;
   int qqbar_percent;
   double lumi;
   string etaMax;
   string EB_res;
   string EE_res;

   RooWorkspace *ws;
   double acceptance_x_efficiency;

   RooRealVar *mass;
   RooRealVar *cosT;

   RooDataSet *MC0;
   RooDataSet *MC2gg;
   RooDataSet *MC2qq;


   MakeToys();
   ~MakeToys();
   void setNBins(int nbins);
   void setNToys(int n) {nToys = n;}
   void setCheat(int TrueFalse) {cheat = TrueFalse;}
   void set_qqbar_percent(double percent) {qqbar_percent = percent; }
   void setLumi(double luminosity) {lumi = luminosity;}
   void setMaxEta(string eta){etaMax=eta;}
   void setResolution(string EB, string EE) {EB_res=EB; EE_res=EE; }
   void makePdfs();
   void createPoint();
   void savePoint(double*);
   void importMC();
   RooDataSet* applyCuts(RooDataSet *originalData);
   double lumi_to_nsignal(double lumi);
};

#endif
