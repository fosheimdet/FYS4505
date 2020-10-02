#include "Riostream.h"
#include "Math/IFunction.h"

void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

void func(){
TNtuple energy("energy","energy","channel:counts_e:counts_bg");
energy.ReadFile("data_file-1.txt");
//TNtuple bg("bg","bg","channel_bg:counts_bg");
//bg.ReadFile("background.txt");
TFile f("137Cs_1us_shaping.root","RECREATE");

TCanvas *c1 = new TCanvas("","",700,800);
c1->ToggleEventStatus();
c1->Divide(1,3);
c1->cd(1);
// c1->SetGrid();
    energy.SetLineColor(kBlue);
    energy.SetLineWidth(2);
	//	energy.Draw("counts_e");
    energy.Draw("counts_e:6.644839+1.220401*channel>>h1(1000,0,1000,5000,0,12000)","","L");
	//	energy.Draw("counts_e", "L");

AddText( 0.85, 0.83, "Signal+background" ,0.050,0.,"right");
AddText( 0.95, 0.03, "Energy[Kev]" ,0.060,0.,"right");
AddText( 0.12, 0.96, "dN/dE" ,0.060,0.,"right");
//#frac{dN}{dE}
//TCanvas* c2 = new TCanvas("c2", "", 600, 400);
c1->cd(2);
// c2->SetGrid();
energy.SetLineColor(kRed+2);
energy.Draw("counts_bg:6.644839+1.220401*channel>>h2(1000,0,1000,5000,0,4000)","","L");
AddText( 0.85, 0.83, "Background only" ,0.050,0.,"right");

// TCanvas* c3 = new TCanvas("c3", "", 600, 400);
c1->cd(3);
//c3->SetGrid();
energy.SetLineColor(kGreen+2);
energy.Draw("(counts_e-counts_bg):6.644839+1.220401*channel>>h3(1000,0,1000,5000,0,12000)","","L");
AddText(0.85, 0.83, "Signal only" ,0.050,0.,"right");
//h3->Fit("gaus","","",500,700);


//AddText( 0.92, 0.03, "E[KeV]" ,0.030,0.,"right"); // Y-axis

    f.cd();
    energy.Write();
    //bg.Write();
    f.Close();

    return 0;
}

//calibration coefficients 6.644839 +x*1.220401 for 137Cs

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
