

tuple<double,double> resolution(double sigma, double deltaSigma, double E, double deltaE);

void func(){

//Energy and std deviations with uncertainties for the various peaks
//=================================================================

//Cesium
double E_cs = 662.4;
double deltaE_cs = 0.0;
double sigmaE_cs = 21.71;
double deltaSigma_cs = 0.05;

//First compton
double E_compt1 = 181.0;
double deltaE_compt1 = 1.4;
double sigmaE_compt1 = 18.6;
double deltaSigma_compt1 = 2.2;

//Second compton
double E_compt2 = 424.2;
double deltaE_compt2 = 4.1;
double sigmaE_compt2 = 54.23;
double deltaSigma_compt2 = 4.09;

//Get resolutions with uncertainties
//=================================================================

//Cesium
tuple<double, double> cesium = resolution(sigmaE_cs, deltaSigma_cs, E_cs, deltaE_cs);
double Res_cs = get<0>(cesium);
double deltaRes_cs = get<1>(cesium);
//First compton
tuple<double, double> first_compt = resolution(sigmaE_compt1, deltaSigma_compt1, E_compt1, deltaE_compt1);
double Res_compt1 = get<0>(first_compt);
double deltaRes_compt1 = get<1>(first_compt);
//Second compton
tuple<double, double> second_compt = resolution(sigmaE_compt2, deltaSigma_compt2, E_compt2,deltaE_compt2);
double Res_compt2 = get<0>(second_compt);
double deltaRes_compt2 = get<1>(second_compt);

//Print results
cout << "R_cesium(%) = " << Res_cs*100 << " pm " << deltaRes_cs*100 << endl;
cout << "R_firstCompt(%) = " << Res_compt1*100 << " pm " << deltaRes_compt1 << endl;
cout << "R_secondCompt(%) = " << Res_compt2*100 << " pm " << deltaRes_compt2 << endl;

}

//Function for calculating resolution with uncertainty
tuple<double,double> resolution( double sigma, double deltaSigma, double E, double deltaE){

  double k = sqrt(-8*log(0.5));
  double Ratio = k*sigma/E;
  double delta_R_squared = pow(deltaSigma,2)*pow((k/E),2) + pow(deltaE,2)*pow((k*sigma/(E*E)),2);

  double delta_R = sqrt(delta_R_squared);

  // double Ratio_upper = k*(sigma + deltaSigma)/E; //Symmetrical uncertainty interval and linear function. We thus only need one of the uncertainty intervals
  //
  // double delta_Ratio = Ratio_upper-Ratio;

  tuple<double, double> results = make_tuple(Ratio, delta_R);
  return results;
}


// tuple<double,double> resolution( double sigma, double deltaSigma, double E, double deltaE){
//
//   double k = sqrt(-8*log(0.5));
//   double Ratio = k*sigma/E;
//   double Ratio_upper = k*(sigma + deltaSigma)/E; //Symmetrical uncertainty interval and linear function. We thus only need one of the uncertainty intervals
//
//   double delta_Ratio = Ratio_upper-Ratio;
//
//   tuple<double, double> results = make_tuple(Ratio, delta_Ratio);
//   return results;
// }
