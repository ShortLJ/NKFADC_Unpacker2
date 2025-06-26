
#include "Hit.h"
#include "SigAna.h"

#ifndef __HITCRYSTAL__
#define __HITCRYSTAL__

using namespace std;

class HitCrystal : public Hit
{
	public:
		//uint8_t idx; // 0,1,2,3
		//float Energy;
		//uint64_t coarse_time;
		//uint16_t fine_time;

		uint8_t detID; // idx+4*cloverID
		uint8_t cloverID;
		float E_seg_sum;
		float E_fv_avg;

		vector<SigAna> vSigAnaFV;
		vector<SigAna> vSigAnaSeg;

		HitCrystal();
		HitCrystal(uint8_t detID, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg);
		~HitCrystal();

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		void Process_Esum();


}



#endif //__HITCRYSTAL__
