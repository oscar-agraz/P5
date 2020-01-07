#include <iostream>
#include <math.h>
#include "instrumentFM.h"
#include "keyvalue.h"

#include <stdlib.h>

using namespace upc;
using namespace std;

InstrumentFM::InstrumentFM(const std::string &param)
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


void InstrumentFM::command(long cmd, long note, long vel) {
 if (cmd == 9) {
 bActive = true;
 adsr.start();
 step = 0;
 index = 0;
 findex = 0;
 accstep = 0;
 I=0.5; 
 N1=100;
 N2=20;
 //N2=140;
 N=100;
 f0=440.0*pow(2,(((float)note-69.0)/12.0));
 fm=N2*f0/N1;
 nota=f0/SamplingRate;
 step1=nota*N1;
 step2=nota*N2;
 A=vel/127.;
 } 
  else if (cmd == 8) {	//'Key' released: sustain ends, release begins
    adsr.stop();
  }
  else if (cmd == 0) {	//Sound extinguished without waiting for release to end
    adsr.end();
  }
}


const vector<float> & InstrumentFM::synthesize() {
  if (not adsr.active()) {
    x.assign(x.size(), 0);
    bActive = false;
    return x;
  }
  else if (not bActive)
    return x;

 for (unsigned int i=0; i < x.size() ; ++i) { 
  findex += step1;
  index2 += step2;
  if(findex>=tbl.size()){
    findex = findex - tbl.size(); 
  }
    if(index2>=tbl.size()){
    index2 = index2 - tbl.size(); 
  }
  prev = (int)findex;
  weight = findex-prev;
  if(prev==tbl.size()-1){
    next=0;
    //findex = findex - tbl.size();
  }
  else next = prev+1;

  //prev2 = (int)index2;
  //if(prev2==tbl.size()-1){
  //  next2=0;
    //findex = findex - tbl.size();
  //}
  //else next2 = prev2+1;

  x[i] = weight*A*tbl[prev+I*tbl[index2]]+(1-weight)*A*tbl[next+I*tbl[index2]];  
  //x[i] = weight*A*tbl[prev+I*tbl[prev2]]+(1-weight)*A*tbl[next+I*tbl[next2]];  
  }

  adsr(x);

  return x;
}