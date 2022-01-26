#include <iostream>
#include <math.h>
#include "instrument_FM.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

  float fase;
  float fm, I, N1, N2, fc;
  
instrumentFM::instrumentFM(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);
  fase = 0; //modificat

  KeyValue kv(param);
  float a,d,s,r;

  if (!kv.to_float("N1", N1))
   N1 = 10;

  if (!kv.to_float("N2", N2))
   N2 = 200;

  if (!kv.to_float("I", I))
    I = 1; //default downward variation shift in semitones
  // Pass I in semitones to linear I
   // I = 1. - pow(2, -I / 12.);

  if (!kv.to_float("fm", fm)) // freqüència moduladora
    fm = 10; //default value

  //Paràmetres ADSR
  if (!kv.to_float("ADSR_A", a)) 
    a = 1.0; //default value

  if (!kv.to_float("ADSR_D", d)) 
    d = 2.0; //default value

  if (!kv.to_float("ADSR_S", s)) 
    s = 4.0; //default value

  if (!kv.to_float("ADSR_R", r)) 
    r = 0.0; //default value


  //cout << "N1: " << N1 << " N2: " << N2 << " I: " << I <<" A: " << a <<" D: " << d << " S: " << s << " R: " << r << endl;
}
float F0;
float phase_c = 0;
float phase_m;
float alpha_c, alpha_m;

void instrumentFM::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index = 0;

	A = vel / 127.;
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
  phase_m=0;
  F0 =  (pow(2,(note-69)/12.)*440)/SamplingRate;
  fm = N2*F0/N1;
  alpha_c =  2*M_PI*F0;
  alpha_m =  2*M_PI*fm;
}


const vector<float> & instrumentFM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

 
  for (unsigned int i=0; i<x.size(); ++i) {
    x[i] = A*sin(phase_c + I*sin(phase_m));
    //x[i] = A*sin(phase_m);
    phase_c = phase_c + alpha_c;
    phase_m = phase_m + alpha_m;

    while(phase_c > M_PI) phase_c = phase_c - 2*M_PI;
    while(phase_m > M_PI) phase_m = phase_m - 2*M_PI;

  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
