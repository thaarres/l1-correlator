#include <cstdio>
#include <cstdlib>
#include <iostream>

#include <algorithm>
#include "firmware/ht.h"
#include <math.h>       /* sin */

#define NTEST 10

struct momvector3 {
    float px, py, pz;
};
  
typedef struct momvector3 Vec3;
  
Vec3 momvector3(float pt_in,float eta_in,float phi_in)
{
    Vec3 vec;
    vec.px = pt_in * cos(phi_in); //phi in radians
    vec.py = pt_in * sin(phi_in); //phi in radians
    vec.pz = pt_in * sinh(eta_in); //eta in radians
    return vec;
    
}

// Returns true if x is in range [low..high], else false
bool inRange(pt_t low, pt_t high, pt_t x)
{
    return  ((x-low) <= (high-low));
}


int main() {
    Jet scjets[NJETS];
    HT ht_out;

    int test = 0;
    
    // Reference values
    
    // Open test file and read seedcone jets
    FILE *dump = fopen("seededcone_jets_ttbar.txt","r"); 
    if (!dump) return 3;
    printf("Opened dump file\n");
    
    
    while (!feof(dump) && (++test <= NTEST)) {
        for (unsigned int j = 0; j < NJETS; ++j) clear(scjets[j]);

        int nscjets, ncands, id; float pt_in, eta_in, phi_in;
        if (fscanf(dump, "Event with %d candidates, %d jets in the selected region\n", &ncands, &nscjets) != 2) return 2;
        
        // printf("Reading event with %d jets \n",nscjets);
        
        float sum_pt = 0.;
        float sum_px = 0.;
        float sum_py = 0.;   
          
        for (int i = 0; i < nscjets; ++i) {
          if (fscanf(dump, "   jet  pt %f eta %f phi %f  constituents %d", &pt_in, &eta_in, &phi_in, &id) != 4) return 2;
          if (i < NJETS) {
                sum_pt += pt_in;
                scjets[i].hwPt =  pt_in;
                scjets[i].hwEta = eta_in / l1ct::Scales::ETAPHI_LSB;
                scjets[i].hwPhi = phi_in / l1ct::Scales::ETAPHI_LSB;
                Vec3 jet3mom = momvector3(pt_in,eta_in,phi_in);
                sum_px += jet3mom.px;
                sum_py += jet3mom.py;
            }
        }
        fscanf(dump, "\n");
        
        pt_t ref_ht  = sum_pt;
        pt_t ref_mht = sqrt( pow(sum_px,2)+pow(sum_py,2) );
        
        // run the algorithm
        algo_main(scjets, ht_out);

        // check the output
        bool ok = false;
        
//        for (int i = 0; i < NJETS; ++i) {
//            std::cout << "Jet nr." << i << ": hw pt " << scjets[i].hwPt << " hw eta " << 
//                scjets[i].hwEta << " hw phi " << scjets[i].hwPhi << std::endl;
//        }
        ap_fixed<14, 3> down = 0.95;
        ap_fixed<14, 3> up   = 1.05;
        //std::cout << "Reference HT:  " << ref_ht << "  GeV   HLS HT:  " << ht_out.hwHt << "  GeV " << std::endl;
        //std::cout << "Reference MHT: " << ref_mht << " GeV   HLS MHT: " << ht_out.hwMHt << " GeV " << std::endl;
        std::cout << "TRUTH(HLS):  HT = " << ref_ht << "("<<ht_out.hwHt<<") GeV , MHT = " << ref_mht<< "("<<ht_out.hwMHt<<") GeV" << std::endl;
        if (inRange(ref_ht*down, ref_ht*up, ht_out.hwHt) && inRange(ref_mht*down, ref_mht*up, ht_out.hwMHt)){
          ok = true;
        }
        // if (ref_ht != ht_out.hwHt || ref_mht != ht_out.hwMHt ) {
//           ok = false;
//         }
        
        if (!ok) { printf("MISMATCH\n"); return 1; }
        printf("\n");
        clear(ht_out);
    }
    printf("Passed all %d tests\n", NTEST);
    return 0;
}
