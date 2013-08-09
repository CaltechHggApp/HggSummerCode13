#include "../include/MakeSpinSPlot.h"
#include <iostream>

using namespace std;

MakeSpinSPlot::MakeSpinSPlot(RooAbsData *data){
  __dataSet = data;
  __nSpec=0;
  __covMatrix=0;

}

void MakeSpinSPlot::addSpecies(TString name, RooAbsPdf* pdf, double expYield){
  __pdfs.push_back(pdf);
  __speciesNames.push_back(name);
  __expectedYields.push_back(expYield);
  __nSpec++;
  //these need to be kept in sync
}

void MakeSpinSPlot::createObservables(){
   for(std::vector<RooRealVar*>::iterator varIt = __variables.begin();
       varIt!=__variables.end(); varIt++){
      __observables.add(*(*varIt));
   }
}

void MakeSpinSPlot::computeCovMatrix(){
  __covMatrix = new TMatrixD(__nSpec,__nSpec);

  std::cout << "Entries: "<< __dataSet->sumEntries() <<std::endl;
  std::cout << "Covariance Matrix ^-1:" << std::endl;
  Long64_t iEntry=-1;
  const RooArgSet *set;
  //loop over dataset
  while( (set=__dataSet->get(++iEntry)) ){
    //update variables 
    for(std::vector<RooRealVar*>::iterator varIt = __variables.begin();
	varIt!=__variables.end(); varIt++){
      (*varIt)->setVal( ((RooRealVar*)set->find((*varIt)->GetName()))->getVal() );
    }
//    cout<<"mass is "<<__variables[0]->getVal()<<"  "<<__pdfs.at(0)->getVal(RooArgSet(*__variables[0]))<<"  "<<__pdfs.at(1)->getVal(RooArgSet(*__variables[0]))<<endl; 
    //compute the matrix
    for(int iRow = 0; iRow<__nSpec;iRow++){
      for(int iCol = 0; iCol<__nSpec;iCol++){
	double den = TMath::Power(computeDenom(),2);
	double num = __pdfs.at(iRow)->getVal(__observables) * __pdfs.at(iCol)->getVal(__observables);
//	cout<<"mass is "<<__variables[0]->getVal()<<"  "<<num<<endl;
//	cout<<num/den<<endl;
	(*__covMatrix)[iRow][iCol] += num/den;
      }
    }
  }//while

//  __covMatrix->Invert();

    for(int iRow = 0; iRow<__nSpec;iRow++){
      for(int iCol = 0; iCol<__nSpec;iCol++){
	std::cout << (*__covMatrix)[iRow][iCol] << " ";
      }
      std::cout << std::endl;
    }
    __covMatrix->Invert();

    cout<<"\nCovariance matrix"<<endl;
    for(int iRow = 0; iRow<__nSpec;iRow++){
       for(int iCol = 0; iCol<__nSpec;iCol++){
	  std::cout << (*__covMatrix)[iRow][iCol] << " ";
       }
       std::cout<< std::endl;
    }
    std::cout<<endl;

}

double MakeSpinSPlot::computeDenom(){ //compute the denominator for the covariance matrix
  //this is used in multiple places
  std::vector<RooAbsPdf*>::iterator pdfIt = __pdfs.begin();
  std::vector<double>::iterator expIt = __expectedYields.begin();

  double denom=0;
  for(; pdfIt != __pdfs.end(); pdfIt++, expIt++){
     denom+= (*expIt) * ((*pdfIt)->getVal(__observables));
  }
  return denom;
}

void MakeSpinSPlot::computeSWeight(){

  //create SWeight variables
  __sWeightVars = new RooArgSet();
  std::vector<TString>::const_iterator specIt = __speciesNames.begin();
  for(; specIt != __speciesNames.end(); specIt++){
    __sWeightVars->add( *(new RooRealVar( Form("%s_sw",specIt->Data()),"",-1e+6,1e+6)) );
  }
  
  __sWeightDataSet = new RooDataSet("SWeightDataSet","",*__sWeightVars);
  
  Long64_t iEntry=-1;

  const RooArgSet *set;
  //loop
  while( (set=__dataSet->get(++iEntry)) ){
    //update variables
    for(std::vector<RooRealVar*>::iterator varIt = __variables.begin();
	varIt!=__variables.end(); varIt++){
      (*varIt)->setVal( ((RooRealVar*)set->find((*varIt)->GetName()))->getVal() );
    }
    specIt = __speciesNames.begin();

    for(; specIt != __speciesNames.end(); specIt++){ // loop over the species
      int iRow = specIt - __speciesNames.begin();
       
      double num=0;
      std::vector<RooAbsPdf*>::iterator pdfIt = __pdfs.begin();
      for(; pdfIt != __pdfs.end(); pdfIt++){
	int iCol = pdfIt - __pdfs.begin();
	
	num += (*__covMatrix)[iRow][iCol] * ((*pdfIt)->getVal(__observables));
//	cout<<(*__covMatrix)[iRow][iCol]<<"   "<<(*pdfIt)->getVal(__observables)<<endl;
     }
      double denom = computeDenom();
//      cout<<num<<endl;
      ((RooRealVar*)__sWeightVars->find( Form( "%s_sw", specIt->Data()) ))->setVal(num/denom);
    }//end loop over species
//    cout<<"mass is "<<__variables[0]->getVal()<<"     pdf is "<<__pdfs[0]->getVal(__observables)<<endl;
    __sWeightDataSet->add(*__sWeightVars);
//    cout<<"mass is "<<__variables[0]->getVal()<<" sig sw is "<<__sWeightDataSet->get(iEntry)->getRealValue("signal_sw")<<" bkg sw is "<<__sWeightDataSet->get(iEntry)->getRealValue("background_sw")<<endl;
  }  //end while loop


   cout<<"number of variables is "<<__variables.size()<<endl;
   cout<<"number of species is "<<__pdfs.size()<<endl;
   RooPlot* frame1 = __variables[0]->frame();
   __pdfs[1]->plotOn(frame1);
   TCanvas c1;
   frame1->Draw();
//   c1.SaveAs("sigPdf.pdf");
   __variables[0]->setVal(125);
   cout<<"pdf at mgg=125 is "<<__pdfs[1]->getVal(*__variables[0])<<endl; 

}

void MakeSpinSPlot::calculate(){
  if(__nSpec<0) return;
  createObservables();
  computeCovMatrix();
  computeSWeight();
}

