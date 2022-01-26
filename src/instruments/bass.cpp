#include <iostream>
#include <math.h>
#include "bass.h"
#include "keyvalue.h"

#include <stdlib.h>
#include <fstream>

using namespace upc;
using namespace std;


bass::bass(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);
  /*
    You can use the class keyvalue to parse "param" and configure your instrument.
    Take a Look at keyvalue.h    
  */
  KeyValue kv(param);
  float a,d,s,r;

  //Paràmetres ADSR
  if (!kv.to_float("ADSR_A", a)) 
    a = 1.0; //default value

  if (!kv.to_float("ADSR_D", d)) 
    d = 2.0; //default value

  if (!kv.to_float("ADSR_S", s)) 
    s = 4.0; //default value

  if (!kv.to_float("ADSR_R", r)) 
    r = 0.0; //default value

  float aux;
  ifstream myReadFile("bass.txt");
  if (!myReadFile) {
    cerr << "Unable to open file datafile.txt";
    exit(1);   // call system to stop
  } 
  while (!myReadFile.eof()) {
      myReadFile >> aux;
      tbl.push_back(aux);
  }
  myReadFile.close();
  
}

float step_bass = 0;
float index_bass = 0;

void bass::command(long cmd, long note, long vel) {
  if (cmd == 9) {		//'Key' pressed: attack begins
    bActive = true;
    adsr.start();
    index_bass = 0;
	  A = vel / 127.;
    
  }
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
  float F0 =  (2*M_PI/40*1046)*(pow(2,(note-69)/12.)*440)/SamplingRate;
  step_bass=F0;
}


const vector<float> & bass::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

  for (unsigned int i=0; i<x.size(); ++i) {
    
    if(index_bass>=tbl.size()){
      index_bass= index_bass- tbl.size(); 
    }
    x[i] = A * tbl[(int)index_bass];
    index_bass += step_bass;
    if (index_bass == tbl.size())
      index_bass = 0;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}

