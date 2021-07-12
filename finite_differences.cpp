#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <fstream>
#include "ConfigFile.tpp"

using namespace std;

int main(int argc, char* argv[]) 
{ 
  cout << " starting reading the config file " << endl;
  string inputPath("configuration.in"); 
  if(argc>1) 
    inputPath = argv[1];

  ConfigFile configFile(inputPath); 

  for(int i(2); i<argc; ++i) 
    configFile.process(argv[i]);

  cout << " finished reading the potential extra arguments " << endl;
  // box:
  double box_size = configFile.get<double>("box_size");

  // Discretization:
  int N = configFile.get<int>("N");
  double dt = configFile.get<double>("dt");
  double tMax = configFile.get<double>("tMax");
  int every = configFile.get<int>("write_every");
  
  double dx = box_size / (N);
  bool U1Forward = configFile.get<bool>("U1Forward");
  bool U2Forward = configFile.get<bool>("U2Forward");

  // vals
  double h0 = configFile.get<double>("h0");
  double eta = configFile.get<double>("eta");
  double amplitude = configFile.get<double>("amplitude");
  double freq = configFile.get<double>("freq");
  double xi = configFile.get<double>("xi");
  // output
  string outputfile = configFile.get<string>("output");
  

  // storage :
  vector<double>  h(N);
  vector<double>  U1(N);
  vector<double>  U2(N);

  vector<double>  htmp(N);
  vector<double>  U1tmp(N);
  vector<double>  U2tmp(N);
  cout << " Done reading values " << endl;
  // initialization
  double x;
  for(int i(0); i<N; ++i){
      x = i * dx;
      h[i] = h0 + h0 * amplitude * sin(freq * 2 * 3.1415926535897 * x / box_size);
  }

  for(int i(0); i<N; ++i){
      if(U1Forward) U1[i] = (h[(N + (i+1)%N) % N] - h[i])/dx;
      else U1[i] = (h[i] - h[(N + (i-1)%N) % N ])/dx;
      
      if(U2Forward) U2[i] = (h[(N + (i+1)%N) % N] - h[i])/dx;
      else U2[i] = (h[i] - h[(N + (i-1)%N) % N ])/dx;
  }




  cout << " done initializing the fields, now preparing output files " << endl;
  
  ofstream U1ofs((outputfile+"_U1.txt").c_str());
  U1ofs.precision(5);
  ofstream U2ofs((outputfile+"_U2.txt").c_str());
  U2ofs.precision(5);
  ofstream hofs((outputfile+"_h.txt").c_str()); 
  hofs.precision(5);
  ofstream tfs("time.txt");
  tfs.precision(5);

  // write initial state to disk 
  for(int i(0); i<N; ++i){
    U1ofs << U1[i] << " ";
    U2ofs << U2[i] << " ";
    hofs  << h[i] << " ";
  }
  U1ofs << endl;
  U2ofs << endl;
  hofs << endl;
  tfs << 0. << endl;
  
  // time evolution:
  double t(0);
  double U1i;
  double U2i;
  double hi;
  double Pii;
  double Di;
  double fh;
  double J;
  double laplh, laplU1, laplU2;
  int counter = 0;
  while(t<tMax){
    for(int i(0); i<N; ++i){
       U1i = U1[i];
       U2i = U2[i];
       hi  = h[i]; 
       Pii = 0.5 * (U1i - U2i);
       Di  = 0.5 * (U1i + U2i); 
       fh  = xi * hi / (1 + xi * hi*hi);
       J   = fh * ( Pii*Pii - Di*Di - 4*hi*fh);

       if(U1Forward)
          U1tmp[i] = U1i + dt * (U1[( N+(i+1)%N ) % N] - U1i) / dx + dt*J;
       else
          U1tmp[i] = U1i + dt * (U1i - U1[ (N+(i-1)%N) % N] ) / dx + dt*J;
       if(U2Forward)
	  U2tmp[i] = U2i - dt * (U2[( N+(i+1)%N ) % N] - U2i) / dx - dt*J;
       else
          U2tmp[i] = U2i - dt * (U2i - U2[ (N+(i-1)%N) % N] ) / dx - dt*J;

       //htmp[i]  = hi + 0.5*(Pii + 0.5*(U1tmp[i]-U2tmp[i]) ) * dt;
       htmp[i]  = hi + Pii * dt;

       // add viscosity if wanted
       if(abs(eta)>1e-15){ 
         laplh  = ( h[( N+(i+1)%N )%N] +  h[( N+(i-1)%N )%N] - 2 * hi ) / pow(dx,2);
         laplU1 = (U1[( N+(i+1)%N )%N] + U1[( N+(i-1)%N )%N] - 2 * U1i) / pow(dx,2);
         laplU2 = (U2[( N+(i+1)%N )%N] + U2[( N+(i-1)%N )%N] - 2 * U2i) / pow(dx,2);
 
	 htmp[i]  = htmp[i]  + eta * dt * laplh;
	 U1tmp[i] = U1tmp[i] + eta * dt * laplU1;
	 U2tmp[i] = U2tmp[i] - eta * dt * laplU2; // see main.pdf end near eq 3.37 (might change) to see why the minus sign.
       }
    }

    t = t + dt;

    // update main fields
    for(int i(0); i<N; ++i){
       U1[i] = U1tmp[i];
       U2[i] = U2tmp[i];
       h[i] = htmp[i];
    }
    // write if reached 'every' mark
    if(counter % every == 0){
      for(int i(0); i<N; ++i){
         U2ofs << U2[i] << " ";
         U1ofs << U1[i] << " ";
         hofs << h[i] << " ";
      }
      U1ofs << endl;
      U2ofs << endl;
      hofs << endl;
      tfs << t << endl;
    }


    counter = counter + 1;
  } 
  return 0;
}

