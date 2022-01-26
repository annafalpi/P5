#include <iostream>
#include <math.h>
#include "violin.h"
#include "keyvalue.h"

#include <stdlib.h>
#include <fstream>

using namespace upc;
using namespace std;

  float fase_violin;
 
violin::violin(const std::string &param) 
  : adsr(SamplingRate, param) {
  bActive = false;
  x.resize(BSIZE);
  fase_violin = 0; //modificat

  KeyValue kv(param);
  float a,d,s,r;

  int N;

  if (!kv.to_int("N",N))
    N = 40; //default value

  //Paràmetres ADSR
  if (!kv.to_float("ADSR_A", a)) 
    a = 1.0; //default value

  if (!kv.to_float("ADSR_D", d)) 
    d = 2.0; //default value

  if (!kv.to_float("ADSR_S", s)) 
    s = 4.0; //default value

  if (!kv.to_float("ADSR_R", r)) 
    r = 0.0; //default value

  tbl.resize(N);
  
  float aux;
  ifstream myReadFile("violin_synth.txt");
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
float F0_violin;
float step_violin;
float index_violin;

void violin::command(long cmd, long note, long vel) {
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
  //phase_m=0;
  F0_violin = (2*M_PI/40*523.251)*(pow(2,(note-69)/12.)*440)/SamplingRate;
  step_violin=F0_violin;
}


const vector<float> & violin::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

   for (unsigned int i=0; i<x.size(); ++i) {
    if(index_violin>=tbl.size()){
      index_violin = index_violin - tbl.size(); 
    }
    index_violin += step_violin;
    //index_violin++;
    float coef_inter = index_violin - (int)index_violin; 
    int index_violin_next = 0;
    if((int)index_violin +1 == tbl.size()){
      index_violin_next = 0;
    } else{
      index_violin_next = (int)index_violin +1;  
    }
      
    x[i] = coef_inter * A * tbl[(int)index_violin] + (1-coef_inter)* A * tbl[index_violin_next];
    
    if (index_violin == tbl.size())
        index_violin = 0;
  }
  adsr(x); //apply envelope to x and update internal status of ADSR

  return x;
}
