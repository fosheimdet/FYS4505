void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );


// ==============================================================================
// =========================MAIN FUNCTION=======================================
// ==============================================================================
void func(bool raw){ //pick raw data or adjusted
fstream geig;
geig.open("Geiger_plateau.txt");

TGraph* g1 = new TGraph(17);
TGraph* g2 = new TGraph(17);
TGraph* g3 = new TGraph(17);
TGraph* g_avrg = new TGraph(17);
TGraph* g_comb = new TGraph(51);
double t,v1,v2,v3;

for(int i = 0; i<17; i++){
	geig>>t>>v1>>v2>>v3;
	g1->SetPoint(i,t,v1);
	g2->SetPoint(i,t,v2);
	g3->SetPoint(i,t,v3);
	g_comb->SetPoint(3*i,t,v1);
	g_comb->SetPoint(3*i+1,t,v2);
	g_comb->SetPoint(3*i+2,t,v3);
	g_avrg->SetPoint(i,t,(v1+v2+v3)/3);
}
// for(int i = 0; i<17; i++){
// 	geig>>t>>v1>>v2>>v3;
// 	g_comb->SetPoint(3*i,t,v1);
// 	g_comb->SetPoint(3*i+1,t,v2);
// 	g_comb->SetPoint(3*i+2,t,v3);
// }

TCanvas* c1 = new TCanvas("c1", "", 600, 400);
c1->SetGrid();

// g1->SetMarkerColor(kBlue+1);
// g1->SetNameTitle("g1", "Measurement 1");
// g2->SetMarkerColor(kRed+1);
// g2->SetNameTitle("g2", "Measurement 2");
// g3->SetMarkerColor(kGreen+2);
// g3->SetNameTitle("g3", "Measurement 3");
// g1->Draw("a*");
// g2->Draw("same *");
// g3->Draw("same *");
g_avrg->SetNameTitle("g_avrg", "Average measurement");
g_avrg->Draw("a*");


// gPad->BuildLegend();

// TCanvas* c2 = new TCanvas("c2", "", 600, 400);
// c2->SetGrid();
// g_comb->Draw("a*");


TF1* f = new TF1("f", "pol0(0)",300,750);
f->SetParameter(0,130);
g3->Fit(f,"PMER+");
f->SetLineColor(kBlue);
f->Draw("same");
f->SetNameTitle("f", "0-th order polynomial fit"); 
gPad->BuildLegend();
//AddText( 0.9, 0.1, Form("#bar{#lambda}=%5.6f",avrgRate) ,0.038,0.,"right"); // Y-axis
AddText( 0.95, 0.05, "Voltage[V]" ,0.04,0.,"right"); // X-label
if(raw==true){
	AddText( 0.25, 0.94, "#counts per 10 sec" ,0.04,0.,"right");
}else{
	AddText( 0.11, 0.94, "N(t)" ,0.04,0.,"right");
}
}
//==============================================================================
//===================END OF MAIN FUNCTION=======================================
//==============================================================================






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
