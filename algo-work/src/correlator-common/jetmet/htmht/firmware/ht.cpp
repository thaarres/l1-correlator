#include "ht.h"
#include "TreeReduce.h"
#include <cmath>
#include <cassert>
#include <hls_math.h>
#include <iostream>
#include "hls_math.h"
#include "ap_fixed.h"

// Option 1: Direct logic, estimate iteratively
void sine(radians_t x, ap_fixed<11,2> &s)
{
   s = hls::sin(x);
}

void cosine(radians_t x, ap_fixed<11,2> &c)
{
   c = hls::cos(x);
}


// Option 2: Store sin/cos as look-up tables
// Sine table
template<class data_T, class table_T, int N>
void init_sinphi_table(table_T table_out[N]){
    for(int i = 0; i < N; i++){
        float x = i*(M_PI/180.)/4.;
        table_T sin_x = sin(x);
        table_out[i] = sin_x;
    }
}
template<class in_t, class table_t, int N>
table_t sine_with_conversion(etaphi_t hwPhi){
    table_t sin_table[N];
    init_sinphi_table<in_t, table_t, N>(sin_table);
    table_t out = sin_table[hwPhi];
    return out; 
}
// Cosine table
template<class data_T, class table_T, int N>
void init_cosphi_table(table_T table_out[N]){
    for(int i = 0; i < N; i++){
        float x = i*(M_PI/180.)/4.;
        table_T cos_x = cos(x);
        table_out[i] = cos_x;
    }
}
template<class in_t, class table_t, int N>
table_t cosine_with_conversion(etaphi_t hwPhi){
    table_t cos_table[N];
    init_cosphi_table<in_t, table_t, N>(cos_table);
    table_t out = cos_table[hwPhi];
    return out; 
}


pt_t mht_compute_lut(Jet jets[NJETS]){
   
  #pragma HLS pipeline
  ap_fixed<16,13> sum_px = 0;
  ap_fixed<16,13> sum_py = 0;

  for(int i = 0; i < NJETS; i++){
      #pragma HLS unroll
    
      cossin_t sinphi;
      cossin_t cosphi;
      bool sign = jets[i].hwPhi.sign();
      etaphi_t absPhi = hls::abs(jets[i].hwPhi);
      sinphi  = sine_with_conversion  <etaphi_t, cossin_t, N_TABLE>(absPhi);
      sinphi = (sign > 0) ? (cossin_t) (-sign*sinphi) : sinphi ; //Flip sign bit if hwPt is negative, sin(-x)=-sin(x)
      cosphi  = cosine_with_conversion  <etaphi_t, cossin_t, N_TABLE>(absPhi);// Do nothing, cos(-θ) = cos θ,
    
      sum_py +=  jets[i].hwPt * sinphi;
      sum_px +=  jets[i].hwPt * cosphi;
  }
  
return hls::sqrt( (sum_px*sum_px)+(sum_py*sum_py));

  
}
pt_t mht_compute(Jet jets[NJETS]){

    #pragma HLS pipeline
    ap_fixed<16,13> sum_px = 0;
    ap_fixed<16,13> sum_py = 0;
    for(int i = 0; i < NJETS; i++){
        #pragma HLS unroll
      
        ap_fixed<11,9> phi;
        phi.V = jets[i].hwPhi.V;
        
        radians_t phiradians;
        phiradians = Scales::scale_degToRad*phi;
        
        ap_fixed<11,2> sinphi;
        ap_fixed<11,2> cosphi;
        sine(phiradians,sinphi);
        cosine(phiradians,cosphi);
        
        sum_py +=  jets[i].hwPt * sinphi;
        sum_px +=  jets[i].hwPt * cosphi;
      }

    return (ap_fixed<14,12>)hls::sqrt( (sum_px*sum_px)+(sum_py*sum_py));
}


pt_t ht_compute(Jet jets[NJETS]){
    #pragma HLS pipeline
    pt_t sum_hwPt = 0.;
    
    for(int i = 0; i < NJETS; i++){
            #pragma HLS unroll
            sum_hwPt     += jets[i].hwPt;
        }
    return sum_hwPt;
}

void algo_main(Jet jets[NJETS], HT &ht ){
    #pragma HLS pipeline
    #pragma HLS array_partition variable=jets complete
    #pragma HLS data_pack variable=jets
  
    pt_t ht_tmp  = ht_compute(jets);
    // pt_t mht_tmp = mht_compute(jets); // Based on the hls math library sin/cos functions
    pt_t mht_tmp = mht_compute_lut(jets); // Precomputing and saving sin/cos to look-up tables
    
    ht.hwHt  = ht_tmp;
    ht.hwMHt = mht_tmp;
    
    
}
