
#include "TH1D.h"
#include "TMath.h"
#include "TLatex.h"

//Data aquisition times for the various files:
int t_bgr = 200;

int t_zeroNoRod = 101;
int t_negative = 100;
int t_positive = 100;

 void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
 	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void func(){

ifstream inBgr;


ifstream inNegative;
ifstream inGuessed0;
ifstream inPositive;

inBgr.open("background.asc");

inNegative.open("negative_angle.asc");
inGuessed0.open("guessed_zero.asc");
inPositive.open("positive_angle.asc");


double x,y;

TH1D* h_bgr = new TH1D("h_bgr", "h_bgr", 1024, 0, 1024);

TH1D* negative = new TH1D("negative", "negative", 1024, 0, 1024);
TH1D* h_guessed0 = new TH1D("h_guessed0", "h_guessed0", 1024, 0, 1024);
TH1D* positive = new TH1D("positive", "positive", 1024, 0, 1024);


for(int i = 0; i<(pow(2,10)); i++){
  inBgr >> x >> y;
  h_bgr->SetBinContent(x,y);
}

for(int i = 0; i<pow(2,10); i++){
  inNegative >> x >> y;
  double a = h_bgr->GetBinContent(i);
  negative->SetBinContent(i, y - a*t_negative/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  inGuessed0 >> x >> y;
  h_guessed0->SetBinContent(x,y);
  double a = h_bgr->GetBinContent(i);
  h_guessed0->SetBinContent(x,y-a*t_zeroNoRod/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  inPositive >> x >> y;
  double a = h_bgr->GetBinContent(i);
  positive->SetBinContent(x,y-a*t_positive/double(t_bgr));
}


// in35_degrees >> k >> l;
// h_35->SetBinContent(k,l);
int rebinFactor = 3;

negative->SetLineColor(kGreen-2);
negative->Rebin(rebinFactor);
h_guessed0->SetLineColor(kRed);
h_guessed0->Rebin(rebinFactor);
positive->SetLineColor(kBlue-2);
positive->Rebin(rebinFactor);

h_bgr->SetLineColor(kBlack);
h_bgr->Rebin(rebinFactor);

TCanvas* c = new TCanvas("c", "c", 600, 400);
c->SetGrid();

gStyle->SetOptStat(0);

negative->Draw();
positive->Draw("same");
h_guessed0->Draw("same");


h_bgr->Draw("same");






TLegend *leg = new TLegend(0.6, 0.7, 0.88, 0.88);
leg->SetHeader("Without rod, rebinFactor = 3", "c");

leg->AddEntry(negative,"Negative angle", "l");
leg->AddEntry(h_guessed0,"Guessed zero", "l");
leg->AddEntry(positive, "Positive angle", "l");

leg->AddEntry(h_bgr, "Background", "l");
//   leg->AddEntry(fit, "Fit function", "l");
 leg->Draw("same");

 AddText( 0.9, 0.15, "Channel number, H" ,0.04,0.,"right");
 AddText( 0.15, 0.9, "dN/dH" ,0.04,0.,"right");
  // AddText( 0.5, 0.9, "" ,0.05,0.,"right");

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
