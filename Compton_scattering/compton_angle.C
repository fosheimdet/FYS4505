
using namespace TMath;



void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
       Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

////////////////////////////////////////////////////////////////////////////////
///////////////////////// Main Function ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void func(){

// TF1* f = new TF1("f", "662*1.3*(1-Cos(x))/(1+1.3*(1-Cos(x)))", 0,Pi());
TF1* f = new TF1("f", "661.7/(1+(661.7/511)*(1-Cos(x)))", 0,Pi());

TCanvas* c = new TCanvas("c", "", 600, 400);
c->SetGrid();
f->SetLineColor(kBlue);
f->Draw();

double convFac = 2*Pi()/360.; //Conversion factor from degrees to radians

TGraph* g = new TGraph(5);
g->SetPoint(0,0,667.2);
g->SetPoint(1,35*convFac,536.9);
g->SetPoint(2,65*convFac,389.1);
g->SetPoint(3,95*convFac,279.1);
g->SetPoint(4,120*convFac,222.4);
g->Draw("* same");

AddText( 0.9, 0.045, "Radians" ,0.04,0.,"right");
AddText( 0.15, 0.93, "Energy[KeV]" ,0.04,0.,"right");
// AddText( 0.65, 0.96, "With rod, calibrated" ,0.05,0.,"right");

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
