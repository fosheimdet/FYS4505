


void func(int rebinFactor){    //rebinFactor for the two histograms
    TCanvas *c1 = new TCanvas("LaBr detector","LaBr detector",900,700);
    c1->SetGrid();
    c1->ToggleEventStatus();
    c1->SetLogy();
    gStyle->SetOptStat(0);
    TFile *f1 = new TFile("./Calibration_LaBr_152Eu/152Eu_calibrated.root");//Read the file for LaBr detector
    TH1D *h1 = (TH1D*) f1->Get("h_152Eu");
    h1->SetTitle(""); h1->GetXaxis()->SetTitle("E [KeV]"); h1->GetYaxis()->SetTitle("dN/dE"); 
    h1->Rebin(rebinFactor);

    TFile *f2 = new TFile("./Calibration_Ge_152Eu/152Eu_calibrated.root");//Read the file for Ge detector
    TH1D *h2 = (TH1D*) f2->Get("h_152Eu");
    h2->Rebin(rebinFactor);

h1->SetLineColor(2);///LaBr detector
h2->SetLineColor(4);///Ge detector

    h1->Draw("l");
    h2->Draw("same");

    TLegend* leg = new TLegend(0.65,0.65, 0.85, 0.8);
    leg->SetHeader("152Eu spectra", "c");
    leg->AddEntry(h1, "LaBr_{3} detector", "l");
    leg->AddEntry(h2, "Ge detector", "l");
    leg->Draw("same");
}
