// Takes a root file with some histograms at 8tev, and
// another file with histos at 14tev and creates the file
// histos12.root with those histograms at 12TeV.
// To run do: root create12tev
// Requires histos8.root and histos14.root to be in same folder.

TH1F* interpolate_one(TH1F* hist1, TH1F* hist2)
{
   float energy1 = 8.;
   float energy2 = 14.;
   float energy3 = 12.;

   TString name = hist1->GetName();
   int nbins = hist1->GetNbinsX();
   float xlow = hist1->GetXaxis()->GetXmin();
   float xup = hist1->GetXaxis()->GetXmax();

   TH1F* hist3 = new TH1F(name, "", nbins, xlow, xup);

   for(int i=1; i<= nbins;i++)
   {
      float value1 = hist1->GetBinContent(i);
      float value2 = hist2->GetBinContent(i);
      float value3 = value1 + (value2 - value1) * (energy3 - energy1) / (energy2 - energy1);
      hist3->SetBinContent(i, value3);
   }
   return hist3;
}

void interpolate()
{
   TFile *f8 = new TFile("histos8.root");
   TFile *f14 = new TFile("histos14.root");
   TFile *f12 = new TFile("histos12.root", "recreate");

   TIter next8(f8->GetListOfKeys());
   TIter next14(f14->GetListOfKeys());
   TKey *key8;
   TKey *key14;
   while((key8 = (TKey*)next8()))
   {
      key14 = (TKey*)next14();
      TH1F *h8 = (TH1*)key8->ReadObj();
      TH1F *h14 = (TH1*)key14->ReadObj();

      TH1F* h12 = interpolate_one(h8, h14);
      h12->Write();
   }
}
