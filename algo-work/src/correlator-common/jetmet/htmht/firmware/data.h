#ifndef ALGO_DATA_H
#define ALGO_DATA_H

#include "ap_int.h"
#include "ap_fixed.h"
#include "../../../dataformats/puppi.h"
#include "../../../dataformats/jets.h"

typedef l1ct::pt_t pt_t;             // Type for pt/ht 1 unit = 0.25 GeV; max = 16 TeV
typedef l1ct::glbeta_t etaphi_t;     // Type for eta & phi

typedef ap_fixed<12,3> radians_t;
typedef ap_fixed<14,2> cossin_t;

static constexpr int N_TABLE = 2048;

namespace Scales {
  const ap_fixed<12,-4> scale_degToRad = M_PI/180.;
};  // namespace Scales

typedef l1ct::Jet Jet;


// Class for the data type returned
class HT {
public:
    pt_t hwHt  = 0.;
    pt_t hwMHt = 0.;
};

inline void clear(HT &p) {
    p.hwHt = 0.;
    p.hwMHt = 0.;
}


#define NJETS 10


#endif