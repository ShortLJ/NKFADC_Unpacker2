
#include "HitFV.h"
#include "HitSeg.h"

#ifndef __HITCRYSTAL__
#define __HITCRYSTAL__

using namespace std;

class HitCrystal
{
	public:
		uint8_t idet;
		uint8_t iclover;
		uint8_t icrystal;
		// double E_seg_sum;

		vector<HitFV> vHitFV;
		vector<HitSeg> vHitSeg;

		HitCrystal();
		HitCrystal(uint8_t idet, vector<Sig> v_sig_fv, vector<Sig> v_sig_seg);
		~HitCrystal();

	private:
		bool isValid();
		//void Clear();


}



#endif //__HITCRYSTAL__
