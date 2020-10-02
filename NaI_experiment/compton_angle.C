
using namespace TMath;
void func(){
TF1* f = new TF1("f", "662*1.3*(1-Cos(x))/(1+1.3*(1-Cos(x)))", 0,Pi());

TCanvas* c = new TCanvas("c", "", 600, 400);
c->SetGrid(); 
f->Draw();
}
