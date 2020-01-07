#include <iostream>
#include <math.h>
#include "seno.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

Seno::Seno(const std::string &param)
 : adsr(SamplingRate, param) {
 bActive = false;
 x.resize(BSIZE);
 
 KeyValue kv(param);
 int N;

 if (!kv.to_int("N",N))
 N = 40;

 tbl.resize(N);
 float phase=0, step=2*M_PI/(float)N;
 index = 0;
 for (int i=0; i < N ; ++i) {
 tbl[i] = sin(phase);
 phase += step;
 }
}


void Seno::command(long cmd, long note, long vel) {
 if (cmd == 9) {
 bActive = true;
 adsr.start();
 step = 0;
 index = 0;
 findex = 0;
 accstep = 0;
 N=100;
 f0=440.0*pow(2,(((float)note-69.0)/12.0));
 nota=f0/SamplingRate;
 step=nota*N;
 A=vel/127.;
 } 
  else if (cmd == 8) {
    adsr.stop();
  }
  else if (cmd == 0) {
    adsr.end();
  }
}


const vector<float> & Seno::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

 for (unsigned int i=0; i < x.size() ; ++i) {
  findex += step;
  if(findex>=tbl.size()){
    findex = findex - tbl.size(); 
  }
  prev = (int)findex;
  weight = findex-prev;
  if(prev==tbl.size()-1){
    next=0;
  }
  else next = prev+1;
  x[i] = weight*A*tbl[prev]+(1-weight)*A*tbl[next];  
  //x[i] = (A*tbl[prev]+A*tbl[next])/2;  
  }

  adsr(x);
  return x;
}