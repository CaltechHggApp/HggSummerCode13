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

const int nsignal = 1000;
const int nbackground = 10000;

class Class2 {
public:

   RooWorkspace *ws;

   RooRealVar *mass;
   RooRealVar *mean_sig_mass;
   RooRealVar *sigma_sig_mass;

   RooRealVar *cosT;
   RooDataSet *genData_sig_cosT;

   RooRealVar *c_bkg_mass;

   RooRealVar *p1_bkg_cosT;
   RooRealVar *p2_bkg_cosT;

   RooRealVar *maxEta;

/*
   vector< pair<float,float> > etaRange;
   vector<double> signalWidth;
   vector<double> signalEfficiency;
*/
   float signalWidth;
   float signalEfficiency;

   float signalYield;
   float backgroundYield;
   double nSignal_gen;
   double nBackground_gen;

   RooDataSet *toyData;
   RooDataSet *extractedData;

   Class2();
   ~Class2();
//   void setEtaRanges();
   void setNBins(int nbins) { cosT->setBins(nbins);}
   void setNSignal(float, float);
   void create_signal_pdfs();
   void create_spin2_pdf();
   void generate();
   void determineYield();
   void extractSignal();
   void plot(TString);

};

Class2::Class2()
{
   ws = new RooWorkspace("ws","");
   mass = new RooRealVar("mass","",110,150);
   mass->setBins(30);
   mean_sig_mass = new RooRealVar("mean_sig_mass","",125,80,160);
   sigma_sig_mass = new RooRealVar("sigma_sig_mass","",2,0,10);

   cosT = new RooRealVar("cosT","",0.,1.);
   cosT->setBins(10);
   
   c_bkg_mass = new RooRealVar("c_bkg_mass","",-.05,-5,5);

   p1_bkg_cosT = new RooRealVar("p1_bkg_cosT","",0,-100,100);
   p2_bkg_cosT = new RooRealVar("p2_bkg_cosT","",1,-100,100);

   maxEta = new RooRealVar("maxEta","",0.,50.);

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
   delete maxEta;
//   delete model_sig_mass;
//   delete model_bkg_mass;
//   delete model_bkg_cosT;
//   delete model_sig0_cosT;
//   delete model_sig2_cosT;
   delete toyData;
   delete extractedData;
   delete ws;

}


/*
void Class2::setEtaRanges()
{
   pair <float,float> range1;
   range1.first=0.;
   range1.second=.8;

   pair <float,float> range2;
   range2.first=.8;
   range2.second=1.44;

   pair <float,float> range3;
   range3.first=1.57;
   range3.second=2.0;

   pair <float,float> range4;
   range4.first=2.0;
   range4.second=2.6;

   pair <float,float> range5;
   range5.first=2.6;
   range5.second=3.0;

   pair <float,float> range6;
   range6.first=3.0;
   range6.second=15.0;

   etaRange.push_back(range1);
   etaRange.push_back(range2);
   etaRange.push_back(range3);
   etaRange.push_back(range4);
   etaRange.push_back(range5);
   etaRange.push_back(range6);
}
*/
