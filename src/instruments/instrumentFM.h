#ifndef INSTRUMENTFM
#define INSTRUMENTFM

#include <vector>
#include <string>
#include "instrument.h"
#include "envelope_adsr.h"

namespace upc {
  class InstrumentFM: public upc::Instrument {
    EnvelopeADSR adsr;
    unsigned int index, N, prev, next, prev2, next2;
	  float A, step, step1, step2, phase, vel, accstep, weight, findex, index2, note, nota, f0, fm, N1, N2, I;
    std::vector<float> tbl;
  public:
    InstrumentFM(const std::string &param = "");
    void command(long cmd, long note, long velocity=1); 
    const std::vector<float> & synthesize();
    bool is_active() const {return bActive;} 
  };
}

#endif
