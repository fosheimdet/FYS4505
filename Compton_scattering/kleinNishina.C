#include "TH1D.h"
#include "TMath.h"
#include "TLatex.h"

//Data aquisition times for the various files:
int t_bgr = 200;

int t_guessed0 = 101;
int t_35 = 352; int t_35NR = 341;
int t_65 = 380; int t_65NR = 383;
int t_95 = 336; int t_95NR = 343;
int t_120 = 254; int t_120NR = 254;



// double gain = 1.85137;
// double offset = -10.071824;

double gain = 1.8599269;
double offset = -16.475274;

 void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
 	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void func(){

ifstream inBgr;

ifstream inGuessed0;

ifstream in35, in35_noRod;
ifstream in65, in65_noRod;
ifstream in95, in95_noRod;
ifstream in120, in120_noRod;

inBgr.open("background.asc");

inGuessed0.open("guessed_zero.asc");
in35.open("35_degrees.asc"); in35_noRod.open("35_degrees_no_rod.asc");
in65.open("65_degrees.asc"); in65_noRod.open("65_degrees_no_rod.asc");
in95.open("95_degrees.asc"); in95_noRod.open("95_degrees_no_rod.asc");
in120.open("120_degrees.asc"); in120_noRod.open("120_degrees_no_rod.asc");
double offsetTheta = 6.57;
//inBgr.open("0_degrees_no_rod.asc");

double x,y,z,w; //Variables to store the columns of the input file streams

// double xmin = offset -gain/2; //Minimum energy of our histograms
// double xmax = offset + gain/2 + gain*1024; //Maximum energy of our histograms

double xmin = offset; //Minimum energy of our histograms
double xmax = offset + gain*1024; //Maximum energy of our histograms


TH1D* h_bgr = new TH1D("h_bgr", "h_bgr", 1024, xmin, xmax);

TH1D* h_guessed0 = new TH1D("h_guessed0", "", 1024, xmin, xmax);
TH1D* h_35 = new TH1D("h_35", "", 1024, xmin, xmax);
TH1D* h_65 = new TH1D("h_65", "", 1024, xmin, xmax);
TH1D* h_95 = new TH1D("h_95", "", 1024, xmin, xmax);
TH1D* h_120 = new TH1D("h_120", "", 1024, xmin, xmax);



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
  in35_noRod >> z >> w;
  double a = h_bgr->GetBinContent(i);
  h_35->SetBinContent(x,y-w*t_35/double(t_35NR)-a*t_35/double(t_bgr)); //Scattering with rod (y) - scattering without rod (w) - background
}

for(int i = 0; i<(pow(2,10)); i++){
  in65 >> x >> y;
  in65_noRod >> z >> w;
  double a = h_bgr->GetBinContent(i);
  h_65->SetBinContent(x,y-w*t_65/double(t_65NR)-a*t_65/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in95 >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_95->SetBinContent(x,y-w*t_95/double(t_95NR)-a*t_95/double(t_bgr));
}

for(int i = 0; i<(pow(2,10)); i++){
  in120 >> x >> y;
  in65_noRod >> z >> w;
  double a = h_bgr->GetBinContent(i);
  h_120->SetBinContent(x,y-w*t_120/t_120NR-a*t_120/double(t_bgr));
}

TCanvas* c2 = new TCanvas("c2", "c2", 600, 400);
c2->SetGrid();

TGraph* g = new TGraph(4);
double rate0 = h_guessed0->Integral()/t_guessed0;
double rate35 = h_35->Integral()/t_35;
double rate65 = h_65->Integral()/t_65;
double rate95 = h_95->Integral()/t_95;
double rate120 = h_120->Integral()/t_120;

// g->SetPoint(0,0,rate0);
g->SetPoint(0,35-offsetTheta,rate35);
g->SetPoint(1,65-offsetTheta,rate65);
g->SetPoint(2,95-offsetTheta,rate95);
g->SetPoint(3,120-offsetTheta, rate120);
g->SetMarkerColor(kRed);
g->Draw("a*");
AddText( 0.93, 0.045, "angle, #theta[degrees]" ,0.04,0.,"right");
AddText( 0.25, 0.93, "Counting rate, dN/dt" ,0.04,0.,"right");

cout << "int 0: " << rate0 << endl;
cout <<"int35: " <<rate35 << endl;
cout << "int65: " << rate65 << endl;
cout << "int95: " << rate95 << endl;
cout << "int120: " << rate120 << endl;


double eps = 0.5;
// TF1* Ef = new TF1("Ef", "[0]/(1+([0]/[1])*(1-Cos(x)))", 0, Pi());
TF1* Ef = new TF1("Ef", "([0]/(1+([0]/[1])*(1-TMath::Cos(x))))", 0+eps,TMath::Pi()-eps);
Ef->SetParameters(661.7*1.6e-19,511*1.6e-19);


 TF1* dSigmadTheta = new TF1("dsigmadTheta", "pow([2], 2)*0.5*pow(Ef/[0],2)*(Ef/[0]+[0]/Ef-pow(TMath::Sin(x),2))", 0+eps, TMath::Pi()-eps);
//TF1* dSigmadTheta = new TF1("dsigmadTheta", "pow([2], 2)*0.5*pow(([0]/(1+([0]/[1])*(1-TMath::Cos(x))))/[0],2)*(([0]/(1+([0]/[1])*(1-TMath::Cos(x))))/[0]+[0]/([0]/(1+([0]/[1])*(1-TMath::Cos(x))))-pow(TMath::Sin(x),2))", 0+eps, TMath::Pi()-eps);
//TF1* dSigmadTheta = new TF1("dsigmadTheta", "pow(1,2)*0.5*pow(Ef/662.,2)*Ef", 1+eps, TMath::Pi()-eps);
//dSigmadTheta->SetParameter(2,1);
dSigmadTheta->SetParameter(2,2.8179e-15);
TCanvas* c3 = new TCanvas("c3", "c3", 600, 400);
c3->cd();
dSigmadTheta->Draw();
//Ef->Draw();



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
