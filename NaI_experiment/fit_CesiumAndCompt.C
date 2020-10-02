

void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );
//============== Function Body ====================
void func(int peak){ //1 for backscatter peak, 2 for compton edge and 3 for cesium peak.

  TFile* input = new TFile("137Cs_1us_shaping.root", "read");

  TNtuple* my_tuple = (TNtuple*)input->Get("energy");
  //Extracts the tuple "energy" stored in the file "137Cs..." and stores
  //it in my_tuple

  int entries = my_tuple->GetEntries();
  cout << entries << endl;

//x, y and z are the variables to which the channel number,
// signal energy and background energy will be read to, respectively

  float x, y, z;
  my_tuple->SetBranchAddress("channel", &x);
  my_tuple->SetBranchAddress("counts_e", &y);
  my_tuple->SetBranchAddress("counts_bg", &z);

// TH1F* h = new TH1F("h", "", entries, 0, entries);
// TH1F* h2 = new TH1F("h2", "", 10, 0, 10);

// Calibration coefficients.
//Converts the channel number axis into the energy axis
float offset=6.644839,slope=1.220401;
// The x-axis (in units of keV), so that E0 is the energy in the
// middle of the first bin and the maximum energy is in the middle of
// the last bin. I am assuming there are 8191 bins, but this should be
// checked for your input file.
float xmin=offset-slope/2,xmax=offset+slope/2+2048*slope;

//TH1F *h = new TH1F("h","Energy spectrum;E (keV);Events",2048,xmin,xmax);
TH1F *h = new TH1F("h","",2048,xmin,xmax);
  for(int i = 0; i<=entries; i++){
    my_tuple->GetEntry(i);
  //  cout << x << " " << y << " " << z <<endl;
    h->SetBinContent(i,y-z); //Signal minus background
    h->SetBinError(i,sqrt(y+z));  //Poisson distributed bins?
  }

h->SetMarkerStyle(20);
h->SetMarkerColor(kBlue);
h->SetMarkerSize(0.5);
h->SetLineColor(kBlue);
if(peak==3){
	h->GetXaxis()->SetRangeUser(0,1000);//Set the range of our histogram and thereby our plot
}else{
	h->GetXaxis()->SetRangeUser(0,600);
}


float peak_min,peak_max;
Double_t par[5];
if(peak==1){
	 peak_min=152,peak_max=220; // Fit range for backscatter peak
}else if(peak==2){
	 peak_min=376.7,peak_max=452; // Fit range for compton edge
}else if(peak==3){
	peak_min = 588, peak_max = 718; //Fit range for cesium peak
}

//float peak_min2=400,peak_max2=450; // Fit range for compton edge
//TF1 *gspol1 = new TF1("gspol1","gausn(0) + pol1(3)",peak_min,peak_max);  //==== What is pol1(3)?
TF1 *gspol1 = new TF1("gspol1","gausn(0) + pol1(3)",peak_min,peak_max);
//gausn is shorthand for (1/(sqrt(2*pi)[2]))*[0]*exp(-0.5*((x-[1])/[2])**2). The argument 0 specifies that we start numbering parameters at 0.
//polN is shorthand for a polynomial of degree N.
//So in our case, pol1 refers to a first order polynomial, i.e. a + bx. The argument specifies that we start numbering its parameters at 3.

//TF1 *gspol2 = new TF1("gspol2","gausn(0) + pol1(3)",peak_min2, peak_max2);


gspol1->GetChisquare();

//=====================Provide initial guess for the shape parameters of the fit curve========================
if(peak==1){
	gspol1->SetParameters(900.0,252.5,12.0,585.4,3.93);
}else if(peak==2){
	gspol1->SetParameters(185.5,424.5,26.6,1941.5,-2.522);
}else if(peak==3){
	gspol1->SetParameters(1.4e5,662,21.00,2500,-3.290);
}

// gspol1->SetParameter(0, 900.0); //Norm
// gspol1->SetParameter(1, 252.5);  // mu
// // cout << (peak_max-peak_min)/2 << endl;
// gspol1->SetParameter(2, 12.0); //sigma
// gspol1->SetParameter(3, 585.4); // a
// gspol1->SetParameter(4, 3.93); // b
//Note that we could also simply have used:  "gspol1->SetParameters( 1.4e+05, 662, 21.00, 2500, -3.290)" to set the parameters
//However, for clarity we did not.

//parameter names for gaussian function
gspol1->SetParName(0, "norm");
gspol1->SetParName(1, "mu");
gspol1->SetParName(2, "sigma");
//parameter names for polynomial function a + bx.
gspol1->SetParName(3, "a");  // a (offset)
gspol1->SetParName(4, "b");  // b (slope)

// h->Fit(gspol1,"WMER+"); //"WMER" "R+"
// gspol1->GetParameters(&par[0]);
TCanvas *c1 = new TCanvas("NaI_energy","NaI_energy",900,700);
c1->cd();
c1->SetLeftMargin(0.15);
c1->SetRightMargin(0.12);
c1->SetGrid();
c1->ToggleEventStatus();


h->Draw("E1 X0");
//"E": Tells THistPainter to draw with error bars. "E1" specifies to do so with perpendicular lines at the edges.
//"X0": When used with one of the "E" options, it suppresses the error bar along X like gStyle->SetErrorX(0) would do.

h->Fit(gspol1,"PMER+");
//Here each of the letters of the second argument specifies a different goption:
//"P": tells the fitter to use Pearson chi2
//"M": stands for "more". Tries to improve fit results. It uses the IMPROVe command of
		//TMinuit. This algorithm attempts to improve the found local minimum by searching for a new one
//"E": Perform better Error estimation using Minos technique
//"R": Use range specified in the function range
//"+": Add this new fitted function to the list of fitted functions (by default, any previous function is deleted)

gspol1->GetParameters(&par[0]); //Get the parameters of fit after gradient descent has been performed

double fitMax = gspol1->GetMaximum(); //Find y-value of fitted peak
double fitMaxX = gspol1->GetMaximumX(); //Find x-value of fitted peak
cout << "maximum of fit: " << fitMax << endl;

// Superimpose the background component (not needed since we subtracted the background?)
TF1 *fbkg = new TF1("fbkg","pol1",peak_min-50,peak_max+50);
fbkg->SetLineColor(kGreen);
fbkg->SetParameter(0, par[3]);
fbkg->SetParameter(1, par[4]);
fbkg->Draw("SAME");
gspol1->Draw("SAME");
cout << par[3] << endl;

//=============Draw vertical line to read off peak x-value=============
//TLine* l = new TLine(fitMaxX, -550, fitMaxX, 1e4+520);
// TLine* l_vert;
// TLine* l_hor;
// if(peak==3){
// 	l_vert = new TLine(fitMaxX,-550,fitMaxX,1e4+520);
// 	l_hor = new TLine(5,fitMax,1000,fitMax);
// }else{
// 	l_vert = new TLine(fitMaxX, 0, fitMaxX, 2380);
// 	l_hor = new TLine(5,fitMax,600,fitMax);
// }
// l_vert->SetLineStyle(2);
// l_hor->SetLineStyle(2);
// l_vert->SetLineWidth(3);
// l_hor->SetLineWidth(3);
// l_vert->SetLineColor(kBlack);
// l_hor->SetLineColor(kBlack);
// l_vert->Draw("same");
// l_hor->Draw("same");
//=====================================================================

gStyle->SetOptFit(); //Gives panel on plot with fit results?
h->SetStats(0); //disables statistics box
//h->SetStats(1); //enables statistics box

//AddText( 0.50-0.01, 0.915, Form("%3.0f",double(6.625)) ,0.030,0.,"right"); // Y-axis
AddText( 0.13, 0.93, "#frac{dN}{dE}" ,0.038,0.,"right"); // Y-axis
AddText( 0.92, 0.03, "E[KeV]" ,0.038,0.,"right"); // Y-axis
// AddText( 0.62, 0.92, Form("%5.1f",fitMaxX),0.035, 0.,"left");
// AddText( 0.89, 0.84, Form("%5.1f",fitMax),0.035, 0.,"left");
//f->Write();

//h->Rebin(20);
// h->Draw();

//======================================================
//======================================================

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
