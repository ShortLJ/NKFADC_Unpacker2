
#include "SigAna.h"

#ifndef __HITCRYSTAL__
#define __HITCRYSTAL__

using namespace std;

class HitCrystal
{
	public:
		uint8_t det;
		uint8_t clover;
		uint8_t crystal;

		float E_seg_sum;
		float E_fv_avg;

		vector<SigAna> vSigAnaFV;
		vector<SigAna> vSigAnaSeg;

		HitCrystal();
		HitCrystal(uint8_t det, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg);
		~HitCrystal();

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		void Process_Esum();


}



#endif //__HITCRYSTAL__
