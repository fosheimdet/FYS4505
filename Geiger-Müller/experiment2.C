void AddText( Double_t txt_x = 0.50, Double_t txt_y = 0.50, const char * txt = "dummy", Double_t txt_size = 0.045,
	      Double_t txt_angle = 0., const char * Alignment = "left", Int_t UseNormalizedSize = 1, Int_t txt_color =1 );


// ==============================================================================
// =========================MAIN FUNCTION=======================================
// ==============================================================================
void func(bool raw){ //pick raw data or adjusted
  fstream bam1, bam2, bam3;

  bam1.open("bam1.txt");
  bam2.open("bam2.txt");
  bam3.open("bam3.txt");

double bam1Arr[21][2], bam2Arr[21][2], bam3Arr[21][2]; //Arrays to hold t and r


int bgr60 = 35; //Background measurement for experiment 2 after 60 sec.

double r_bgr = 35/60.;

TGraph* g1 = new TGraph(21);
TGraph* g2 = new TGraph(21);
TGraph* g3 = new TGraph(21);

//==============================================================================
//=========Set TGraphs according to either raw or adjusted datasamples =========
double t1,r1,t2,r2,t3,r3; //Raw data

if(raw){
	for(int i = 0; i<21; i++){
	bam1>>t1>>r1;
	bam2>>t2>>r2;
	bam3>>t3>>r3;
	g1->SetPoint(i,t1,r1-r_bgr*t1);
	g2->SetPoint(i,t2,r2-r_bgr*t2);
	g3->SetPoint(i,t3,r3-r_bgr*t3);
}//end for loop
}else{
	for(int i = 0; i<21; i++){
		bam1>>t1>>r1;
		bam2>>t2>>r2;
		bam3>>t3>>r3;
		bam1Arr[i][0] = t1; bam1Arr[i][1] = r1;
		bam2Arr[i][0] = t2; bam2Arr[i][1] = r2;
		bam3Arr[i][0] = t3; bam3Arr[i][1] = r3;

	}//end for loop

	// double r1m = bam1Arr[21][1];
	// double r2m = bam2Arr[21][1];
	// double r3m = bam3Arr[21][1];
	for(int i = 0; i<21; i++){
		t1 = bam1Arr[i][0]; r1=bam1Arr[20][1]-bam1Arr[i][1];
		t2 = bam2Arr[i][0]; r2=bam2Arr[20][1]-bam2Arr[i][1];
		t3 = bam3Arr[i][0]; r3=bam3Arr[20][1]-bam3Arr[i][1];

		cout << "bam1Arr[21][1]: " << bam1Arr[20][1] << endl;
		// cout << "t1: " << t1 << "  r1: " <<r1 << endl;

		g1->SetPoint(i, t1, r1); //Total count minus estimated background contribution
		g2->SetPoint(i, t2, r2);
		g3->SetPoint(i, t3, r3);
	}

}//end if loop

g1->SetMarkerColor(kBlue+1);
g2->SetMarkerColor(kRed+1);
g3->SetMarkerColor(kGreen+2);

g1->SetNameTitle("Sample 1", "Sample 1"); // First argument is the name, second is the title
g2->SetNameTitle("Sample 2", "Sample 2");
g3->SetNameTitle("Sample 3", "Sample 3");

TF1* f;
TF1* f1;
TF1* f2;
TF1* f3;
if(raw == true){
	f = new TF1("f", "[0]*(1-exp(-[1]*x))", 0, 410);
}else{
	f1 = new TF1("f1", "[0]*(exp(-[1]*x)-1)-4122", 0, 410);
	f2 = new TF1("f2", "[0]*(exp(-[1]*x)-1)-2605", 0, 410);
	f3 = new TF1("f3", "[0]*(exp(-[1]*x)-1)-6666", 0, 410);
}
// }else{
// 	f = new TF1("f", "[0]*()")
// }



// TF1* f = new TF1("")
if(raw==true){
	f->SetParameters(5000., 0.004);
}else{
	f1->SetParameters(2300.,0.004,5);
	f2->SetParameters(2300.,0.004,5);
	f3->SetParameters(2300.,0.004,5);
}





TCanvas* c1 = new TCanvas("c", "", 800, 600);
c1->SetRightMargin(0.25);
c1->SetGrid();
g3->Draw("a*");
g1->Draw("same *");
g2->Draw("same *");

double parameters[3];
double parameters1[3], parameters2[3], parameters3[3];
double avrgRate; //Average of the three found decay rates

gPad->BuildLegend();
double lambda,lambda1,lambda2,lambda3;
//==============================================================================
//===================Fitting g1 (first data sample)=============================
//==============================================================================
if(raw==true){
	f->SetLineColor(kBlue+1);
	f->SetParNames("N_{1}(0)", "#lambda_{1}");
	g1->Fit(f,"PMER+");
	//Note that the this finds the optimal values for the parameters of f
	//and then plots f with these parameters. Changes in color, line width etc. in the fit
	//curve must therefore be made on f before calling the fit function
	gStyle->SetOptFit();
	f->GetParameters(&parameters[0]);
	lambda = parameters[1];
	// avrgRate+=parameters[1];
}else{
	f1->SetLineColor(kBlue+1);
	f1->SetParNames("N_{1}(0)", "#lambda_{1}");
	g1->Fit(f1,"PMER+");
	f1->Draw("same");
	//Note that the this finds the optimal values for the parameters of f
	//and then plots f with these parameters. Changes in color, line width etc. in the fit
	//curve must therefore be made on f before calling the fit function
	gStyle->SetOptFit();
	// f1->GetParameters(&parameters1[0]);
	// lambda1 = parameters1[1];
	// avrgRate1+=parameters1[1];
}



//==============================================================================
//===================Fitting g2 (second data sample)============================
//==============================================================================
//f->SetParameters(2300., 0.004,5); //For some reason to reset the parameters to my estimate of the red line
if(raw==true){
	f->SetLineColor(kRed+1);
	f->SetParNames("N_{2}(0)", "#lambda_{2}","b_{2}");
	g2->Fit(f,"PMER+");
	f->Draw("same");
	gStyle->SetOptFit();

	f->GetParameters(&parameters[0]);
  lambda= parameters[1];

	// avrgRate2+=parameters2[1];
}else{
	f2->SetLineColor(kRed+1);
	f2->SetParNames("N_{2}(0)", "#lambda_{2}","b_{2}");
	g2->Fit(f2,"PMER+");
	f2->Draw("same");
	// f2->Draw("same");
	gStyle->SetOptFit();
	//
	// f2->GetParameters(&parameters2[0]);
	// lambda2= parameters2[1];
	//
	// avrgRate+=parameters[1];
}


//==============================================================================
//===================Fitting g3 (third data sample)=============================
//==============================================================================
if(raw==true){
	f->SetLineColor(kGreen+2);
	f->SetParNames("N_{3}(0)", "#lambda_{3}");
	g3->Fit(f,"PMER+");
	gStyle->SetOptFit();

	f->GetParameters(&parameters[0]);
	lambda=parameters[1];
	avrgRate+=parameters[1];
}else{
	f3->SetLineColor(kGreen+2);
	f3->SetParNames("N_{3}(0)", "#lambda_{3}");
	f3->Draw("same"); 
	g3->Fit(f3,"PMER+");

	gStyle->SetOptFit();

	// f3->GetParameters(&parameters3[0]);
	// lambda3=parameters3[1];
	// avrgRate+=parameters3[1];
}

//==============================================================================

// avrgRate/=3.;
double t_half1 = log(2)/lambda1; double t_half2 = log(2)/lambda2; double t_half3 = log(2)/lambda3;


cout << "lambda1: " << lambda1 << "  t_half1: " << t_half1 << endl;
cout << "lambda2: " << lambda2 << "  t_half2: " << t_half2 << endl;
cout << "lambda3: " << lambda3 << "  t_half3: " << t_half3 << endl;
// cout<< "avrg lambda: " <<avrgRate << endl;

//AddText( 0.9, 0.1, Form("#bar{#lambda}=%5.6f",avrgRate) ,0.038,0.,"right"); // Y-axis
AddText( 0.82, 0.08, "Time[s]" ,0.03,0.,"right"); // X-label
if(raw==true){
	AddText( 0.2, 0.94, "#decays in total" ,0.03,0.,"right");
}else{
	AddText( 0.11, 0.94, "N(t)" ,0.03,0.,"right");
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
