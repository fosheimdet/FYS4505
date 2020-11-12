
double t_Cs = 103.;
double t_Na = 375.;

double t_bgr = 200.;


void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
       Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void func(int peak){ // 0 for no fits, 1 to fit first Na peak and 2 to fit second Na peak

ifstream inCe;
ifstream inNa;
ifstream in_bgr;


inCe.open("Cs137.asc");
inNa.open("Na22.asc");

in_bgr.open("background.asc");

double x,y;

TH1D* h_Cs = new TH1D("h_Cs", "h_Cs", 1024, 0, 1024);
TH1D* h_Na = new TH1D("", "", 1024, 0, 1024);
TH1D* h_bgr = new TH1D("h_bgr", "h_bgr", 1024, 0, 1024);


for(int i = 0; i<1024; i++){
in_bgr >> x >> y;
h_bgr->SetBinContent(x,y);
}

for(int i = 0; i<1024; i++){
  inCe >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_Cs->SetBinContent(x,y-a*t_Cs/t_bgr); //Subtract scaled background from measurement
}

for(int i = 0; i<1024; i++){
  inNa >> x >> y;
  double a = h_bgr->GetBinContent(i);
  h_Na->SetBinContent(x,y-a*t_Na/t_bgr); //Subtract scaled background from measurement
}

int scaleFactor = 3;

TCanvas* c = new TCanvas("c", "c", 600, 400);
c->SetGrid();

if(peak==0){
  gStyle->SetOptStat(0); //Remove statistics box
}else{
  gStyle->SetOptFit(); //Set statistics box to display relevant fit results
}

h_Na->SetLineColor(kBlue);
h_Cs->SetLineColor(kGreen-2);
h_bgr->SetLineColor(kBlack);

h_Na->Draw();
h_Cs->Draw("same");
h_bgr->Draw("same");

//Only make legend if we aren't fitting:
if(peak == 0){
  TLegend* legend = new TLegend(0.5,0.55,0.85,0.8);
  legend->SetHeader("Na 22 and Cs 137 spectra, no rod", "c"); //Last argument specifies that we want to centre the text
  legend->AddEntry(h_Na, "Na 22", "l"); // Last arguemnt specifies to display as a line in the legend
  legend->AddEntry(h_Cs, "Cs 137", "l");
  legend->AddEntry(h_bgr, "Background", "l");

  legend->Draw("same");
}


////////////////////////////////////////////////////////////////////////////////
////////////////////// Fitting first peak //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
if(peak ==1){
  TF1* NaFit1 = new TF1("NaFit1", "gausn(0)", 275,295);
  NaFit1->SetParameters(15000,283,25);

  h_Na->Fit(NaFit1, "PMER+");

  NaFit1->Draw("same");

}

////////////////////////////////////////////////////////////////////////////////
////////////////////// Fitting second peak /////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

if(peak == 2){
  TF1* NaFit2 = new TF1("NaFit2", "gausn(0)", 671, 720);
  NaFit2->SetParameters(1500, 700, 10);

  h_Na->Fit(NaFit2, "PMER+");
  NaFit2->Draw("same");

}


// double gain = 1.85137;
// double offset = -10.071824;


AddText( 0.9, 0.045, "Channel number, H" ,0.04,0.,"right");
AddText( 0.12, 0.93, "dN/dH" ,0.04,0.,"right");
if(peak == 0){
  AddText( 0.7, 0.97, "Na and Cs spectra, ucalibrated" ,0.05,0.,"right");
}else if(peak ==1){
  AddText( 0.60, 0.97, "Fit of first Na peak" ,0.05,0.,"right");

}else if(peak ==2){
  AddText( 0.6, 0.97, "Fit of second Na peak" ,0.05,0.,"right");
}

//2.83146e+02, 6.93869e+02

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
