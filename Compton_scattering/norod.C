
//Data aquisition times for the various files:
int t_bgr = 200;

int t_zeroNR = 101;
int t_guessed0 = 101;
int t_35NR = 341;
int t_65NR = 383;
int t_95NR = 343;
int t_120NR = 254;

double gain = 1.85137;
double offset = -10.071824; 

void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
       Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void func(bool zero){ //Argument specifies whether to include the zero angle in the plot

ifstream inBgr;

ifstream inZeroNR;
ifstream in35NR;
ifstream in65NR;
ifstream in95NR;
ifstream in120NR;

inBgr.open("background.asc");

inZeroNR.open("0_degrees_no_rod.asc");
in35NR.open("35_degrees_no_rod.asc");
in65NR.open("65_degrees_no_rod.asc");
in95NR.open("95_degrees_no_rod.asc");
in120NR.open("120_degrees_no_rod.dat");
//inBgr.open("0_degrees_no_rod.asc");

double x,y;

TH1D* h_bgr = new TH1D("h_bgr", "h_bgr", 1024, 0, 1024);

TH1D* h_zeroNR = new TH1D("h_guessed0", "h_guessed0", 1024, 0, 1024);
TH1D* h_35NR = new TH1D("h_35NR", "h_35NR", 1024, 0, 1024);
TH1D* h_65NR = new TH1D("h_65NR", "h_65NR", 1024, 0, 1024);
TH1D* h_95NR = new TH1D("h_95NR", "h_95NR", 1024, 0, 1024);
TH1D* h_120NR = new TH1D("h_120NR", "h_120NR", 1024, 0, 1024);


for(int i = 0; i<(pow(2,10)); i++){
  inBgr >> x >> y;
  h_bgr->SetBinContent(x,y);
}

for(int i = 0; i<(pow(2,10)); i++){
  inZeroNR >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_zeroNR->SetBinContent(x,y-a*t_zeroNR/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in35NR >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_35NR->SetBinContent(x,y-a*t_35NR/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in65NR >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_65NR->SetBinContent(x,y-a*t_65NR/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in95NR >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_95NR->SetBinContent(x,y-a*t_95NR/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in120NR >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_120NR->SetBinContent(x,y-a*t_120NR/double(t_bgr));
}

int rebinFactor = 3;
h_zeroNR->SetLineColor(kRed);
h_zeroNR->Rebin(rebinFactor);
h_35NR->SetLineColor(kGreen);
h_35NR->Rebin(rebinFactor);
h_65NR->SetLineColor(kMagenta);
h_65NR->Rebin(rebinFactor);
h_95NR->SetLineColor(kTeal);
h_95NR->Rebin(rebinFactor);
h_120NR->SetLineColor(kBlue);
h_120NR->Rebin(rebinFactor);
h_bgr->SetLineColor(kBlack);

TCanvas* c = new TCanvas("c", "c", 600, 400);
c->SetGrid();
gStyle->SetOptStat(0);

if(zero){
  h_zeroNR->Draw();
  h_35NR->Draw("same");
}
else{
  h_35NR->Draw();
}
// h_zeroNR->Draw();
// h_35NR->Draw();
h_65NR->Draw("same");
h_95NR->Draw("same");
h_120NR->Draw("same");
h_bgr->Draw("same");

TLegend* legend = new TLegend(0.5,0.4,0.85,0.8);
legend->SetHeader("No rod, rebin factor = 3");

//legend->AddEntry(h_zeroNR, "Zero degrees", "l");
legend->AddEntry(h_35NR, "35 Degrees", "l");  //The third option specifies the entry as a line
legend->AddEntry(h_65NR, "65 Degrees", "l");
legend->AddEntry(h_95NR, "95 Degrees", "l");
legend->AddEntry(h_120NR, "120 Degrees", "l");
legend->AddEntry(h_bgr, "Background", "l");

legend->Draw("same");

AddText( 0.9, 0.05, "Channel number, H" ,0.04,0.,"right");
AddText( 0.12, 0.93, "dN/dH" ,0.04,0.,"right");
 AddText( 0.7, 0.93, "Without rod, uncalibrated" ,0.05,0.,"right");

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
