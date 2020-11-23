
void calibrate_152Eu ()
{
    TFile *f1 = new TFile("152Eu.root");//Read the file
    TCanvas *c1 = new TCanvas("LaBr detector","LaBr detector",900,700);
    c1->SetGrid(); 
    c1->ToggleEventStatus();
    TH1D *h1 = (TH1D*) f1->Get("LABRSP_sum");
    TH1D *h_calibrated = new TH1D("h_calibrated","Gamma Energy",4096,0,4095);

    Double_t Gain =0.3952;//Change
    Double_t Shift =  -4.25857;//Change

    for (Int_t i =0; i<h1->GetNbinsX(); i++) {
        h_calibrated->SetBinContent((Int_t)(i*Gain+Shift),h1->GetBinContent(i));
    }

    h_calibrated->Draw();

    TFile *outputFile = new TFile("152Eu_calibrated.root","recreate");
    h_calibrated->Write("h_152Eu");
    outputFile->Close();

}
