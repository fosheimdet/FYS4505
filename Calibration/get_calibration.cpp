
#define LOW 2800
#define HIGH 3550
#define WIDTH 10


void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
       Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );

void get_calibration ()
{
    TFile *f1 = new TFile("60Co.root");//Read the file
    TH1D *h1 = (TH1D*) f1->Get("LABRSP_sum");

    TSpectrum *s = new TSpectrum();
    Int_t peaks;
    //Float_t *channel; //For Root 5
    Double_t* channel;  //For Root 6

    TCanvas *c1 = new TCanvas("c1", "fit", 800, 600);
    c1->SetGrid();
    gStyle->SetOptStat(0);

    h1->GetXaxis()->SetRangeUser(LOW,HIGH);//Use an appropiate range that contains only the two 60Co peaks
    h1->SetTitle("");
    h1->Draw();

    peaks = s->Search(h1,WIDTH,"",0.5);//Use an appropiate width

    if (peaks != 2) {
        cout << "There should be two peaks!" << endl;
    } else {
        channel = s->GetPositionX();

        cout << "Channels: "<< channel[0] << ", " << channel[1] << endl;
        cout << "Calibration coefficients: " << endl;
        cout << "Gain: "<< setprecision(5) << (1332.492 - 1173.228)/(channel[1]-channel[0]) << endl;
        cout << "Shift: "<< setprecision(6) << (channel[1]*1173.228 - channel[0]*1332.492)/(channel[1]-channel[0])<< endl;

        TLine* l1 = new TLine(channel[0], 0, channel[0], 3250);
        l1->SetLineWidth(2);
        l1->SetLineStyle(2);
        l1->Draw("same");

        TLine* l2 = new TLine(channel[1],0, channel[1], 3250);
        l2->SetLineWidth(2);
        l2->SetLineStyle(2);
        l2->Draw("same");

        AddText( 0.35, 0.92, Form("H_{0} = %5.1f", channel[0]) ,0.03,0.,"right");
        AddText( 0.78, 0.92, Form("H_{1} = %5.1f", channel[1]) ,0.03,0.,"right");
        AddText( 0.9, 0.045, "Channel number, H" ,0.04,0.,"right");
        AddText( 0.12, 0.93, "dN/dH" ,0.04,0.,"right");

    }




}//end main function




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
