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
#include <numeric>
#include <vector>


using namespace std;
using namespace RooFit;

class Class2 {
public:
   RooWorkspace *ws;

   RooRealVar *mass;
   RooRealVar *mean_sig_mass;
   RooRealVar *sigma_sig_mass;

   RooRealVar *cosT;

   RooRealVar *c_bkg_mass;

   RooRealVar *p1_bkg_cosT;
   RooRealVar *p2_bkg_cosT;

   float signalWidth;
   float signalEfficiency;

   float signalYield;
   float backgroundYield;
   double nSignal_gen;
   double nBackground_gen;


   RooDataSet *spin0Signal;
   RooDataSet *spin2Signal;
   RooDataSet *toyData;
   RooDataSet *extractedData;

   Class2();
   ~Class2();
   void setNBins(int nbins) { cosT->setBins(nbins);}
   void setNSignal(float);
   void apply_cuts(float);
   void create_pdfs();
   void generate_toy();
   void calculate_yield();
   void extract_signal();
   void plot(TString);
   double getPvalue();

};

Class2::Class2()
{
   ws = new RooWorkspace("ws","");
   mass = new RooRealVar("mass","",110,150);
   mass->setBins(30);
   mean_sig_mass = new RooRealVar("mean_sig_mass","",125,80,160);
   sigma_sig_mass = new RooRealVar("sigma_sig_mass","",2,0,10);

   cosT = new RooRealVar("cosT","",0.,1.);
   cosT->setBins(20);

   c_bkg_mass = new RooRealVar("c_bkg_mass","",-.05,-5,5);

   p1_bkg_cosT = new RooRealVar("p1_bkg_cosT","",0,-100,100);
   p2_bkg_cosT = new RooRealVar("p2_bkg_cosT","",1,-100,100);
}


Class2::~Class2()
{
   delete mass;
   delete mean_sig_mass;
   delete sigma_sig_mass;
   delete cosT;
   delete c_bkg_mass;
   delete p1_bkg_cosT;
   delete p2_bkg_cosT;
   delete spin0Signal;
   delete spin2Signal;
   delete toyData;
   delete extractedData;
   delete ws;

}

