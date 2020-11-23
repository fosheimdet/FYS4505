
#define LOW 150
#define HIGH 2900
#define WIDTH 10

void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );


// ==============================================================================
// =========================MAIN FUNCTION=======================================
// ==============================================================================
void func()
{
    TFile *f1 = new TFile("152Eu.root");//Read the file
    TCanvas *c1 = new TCanvas("c1","c1",600,400);
    c1->SetGrid();
    c1->ToggleEventStatus();
    TH1D *h1 = (TH1D*) f1->Get("hist");

    h1->GetXaxis()->SetTitle("Channel number, H");
    h1->GetYaxis()->SetTitle("dN/dH");

    TSpectrum *s = new TSpectrum();
    Int_t peaks;
    //Float_t *channel; //For Root 5
    Double_t *channel;  //For Root 6


    h1->GetXaxis()->SetRangeUser(LOW,HIGH);//Use an appropiate range that contains eleven 152Eu  peaks
    h1->Draw();
    AddText( 0.9, 0.15, "Energy[KeV]" ,0.04,0.,"right");
    AddText( 0.15, 0.9, "dN/dE" ,0.04,0.,"right");
    AddText( 0.5, 0.5, "dN/dE" ,0.04,0.,"right");

    peaks = s->Search(h1,WIDTH,"",1.5);//Use an appropiate width

    if (peaks != 11) {
        cout << "There should be eleven peaks!" << endl;
    } else {
        channel = s->GetPositionX();
        cout << "Channels: "<< channel[0] << ", " << channel[1]  <<  ", " << channel[2] <<  ", " << channel[3] <<  ", " << channel[4] <<  ", " << channel[5] << ", " <<  channel[6] << ", " <<  channel[7] << ", " <<  channel[8] << ", " <<  channel[9] << ", " <<  channel[10] <<

        endl;
        //cout << "Calibration coefficients: " << endl;
        //cout << "Gain: "<< setprecision(5) << (1402.0110 - 121.7830)/(channel[1]-channel[0]) << endl;
        //cout << "Shift: "<< setprecision(6) << (channel[1]*1408.0110 - channel[0]*121.7830)/(channel[1]-channel[0])<< endl;
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
