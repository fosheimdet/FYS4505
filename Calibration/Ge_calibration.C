

void func(){

fstream in;
in.open("EvsH.txt");

TGraph* g = new TGraph(11);

double energy, channel;

for(int i = 0; i<11; i++){
  in>>energy>>channel;
  g->SetPoint(i,channel,energy);
}




TF1* f = new TF1("f", "pol1(0)",0,3000);
f->SetParameters(-100,0.5);

TCanvas* c1 = new TCanvas("c1", "", 600, 400);
g->Draw("a*");
g->Fit(f,"PMER+");
f->Draw("same");
double params[2];
f->GetParameters(&params[0]);
cout << params[0] <<endl;
cout << params[1] << endl;

}
