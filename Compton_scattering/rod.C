
#include "TH1D.h"
#include "TMath.h"
#include "TLatex.h"

//Data aquisition times for the various files:
int t_bgr = 200;

int t_guessed0 = 101;
int t_35 = 352;
int t_65 = 380;
int t_95 = 336;
int t_120 = 254;

 void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
 	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void func(){

ifstream inBgr;

ifstream inGuessed0;

ifstream in35;
ifstream in65;
ifstream in95;
ifstream in120;

inBgr.open("background.asc");

inGuessed0.open("guessed_zero.asc");
in35.open("35_degrees.asc");
in65.open("65_degrees.asc");
in95.open("95_degrees.asc");
in120.open("120_degrees.asc");
//inBgr.open("0_degrees_no_rod.asc");

double x,y;

TH1D* h_bgr = new TH1D("h_bgr", "h_bgr", 1024, 0, 1024);

TH1D* h_guessed0 = new TH1D("h_guessed0", "h_guessed0", 1024, 0, 1024);
TH1D* h_35 = new TH1D("h_35", "h_35", 1024, 0, 1024);
TH1D* h_65 = new TH1D("h_65", "h_65", 1024, 0, 1024);
TH1D* h_95 = new TH1D("h_95", "h_95", 1024, 0, 1024);
TH1D* h_120 = new TH1D("h_120", "h_120", 1024, 0, 1024);

for(int i = 0; i<(pow(2,10)); i++){
  inBgr >> x >> y;
  h_bgr->SetBinContent(x,y);
}

for(int i = 0; i<(pow(2,10)); i++){
  inGuessed0 >> x >> y;
  h_guessed0->SetBinContent(x,y);
  double a = h_bgr->GetBinContent(i);
  h_guessed0->SetBinContent(x,y-a*t_guessed0/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in35 >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_35->SetBinContent(x,y-a*t_35/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in65 >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_65->SetBinContent(x,y-a*t_65/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in95 >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_95->SetBinContent(x,y-a*t_95/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in120 >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_120->SetBinContent(x,y-a*t_120/double(t_bgr));
}

// in35_degrees >> k >> l;
// h_35->SetBinContent(k,l);
int rebinFactor = 3;
h_guessed0->SetLineColor(kRed);
h_guessed0->Rebin(rebinFactor);
h_35->SetLineColor(kGreen);
h_35->Rebin(rebinFactor);
h_65->SetLineColor(kMagenta);
h_65->Rebin(rebinFactor);
h_95->SetLineColor(kTeal);
h_95->Rebin(rebinFactor);
h_120->SetLineColor(kBlue);
h_120->Rebin(rebinFactor);
h_bgr->SetLineColor(kBlack);


TCanvas* c = new TCanvas("c", "c", 600, 400);
c->SetGrid();
gStyle->SetOptStat(0);


h_guessed0->Draw();
h_35->Draw("same");
h_65->Draw("same");
h_95->Draw("same");
h_120->Draw("same");
h_bgr->Draw("same");

//TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88);
TLegend* leg = new TLegend(0.5,0.4,0.85,0.8);
leg->SetHeader("With rod, rebinFactor = 3", "c");


leg->AddEntry(h_guessed0,"Guessed zero", "l");
leg->AddEntry(h_35, "35 Degrees", "l");
leg->AddEntry(h_65, "65 Degrees", "l");
leg->AddEntry(h_95,"95 Degrees", "l");
leg->AddEntry(h_120,"120 Degrees", "l");
leg->AddEntry(h_bgr, "Background", "l");
//   leg->AddEntry(fit, "Fit function", "l");
 leg->Draw("same");

 AddText( 0.9, 0.05, "Channel number, H" ,0.04,0.,"right");
 AddText( 0.12, 0.93, "dN/dH" ,0.04,0.,"right");
  AddText( 0.7, 0.93, "With rod, uncalibrated" ,0.05,0.,"right");

 // AddText( 0.9, 0.15, "Channel number, H" ,0.04,0.,"right");
 // AddText( 0.15, 0.9, "dN/dH" ,0.04,0.,"right");
 //  AddText( 0.5, 0.9, "With rod, uncalibrated" ,0.05,0.,"right");

}


//=======================================================================================================================
void AddText( Double_t txt_x, Double_t txt_y, const char * txt, Double_t txt_size,
              Double_t txt_angle, const char * Alignment, Int_t UseNormalizedSize, Int_t txt_color)
//=======================================================================================================================

{
  Int_t txt_align = 12;
  if ( !strcmp(Alignment, "left"))   { txt_align = 12; } // left
  if ( !strcmp(Alignment, "right"))  { txt_align = 32; } // right
  if ( !strcmp(Alignment, "center")) { txt_align = 22; } // center

  TLatex* t1 = new TLatex( txt_x, txt_y, txt);
  if(UseNormalizedSize) {t1->SetNDC(kTRUE);} // <- use NDC coordinate
  t1->SetTextSize(txt_size);
  t1->SetTextAlign(txt_align);
  t1->SetTextAngle(txt_angle);
  t1->SetTextColor(txt_color);
  t1->Draw();

} // end AddText()
