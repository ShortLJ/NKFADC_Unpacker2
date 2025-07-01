
#ifndef __HITCLOVER__
#define __HITCLOVER__

#include "Hit.h"

using namespace std;

class HitClover : public Hit
{
	public:
		//uint8_t idx; // clover number : detID>>2
		//float Energy; // addback E in a clover
		//uint64_t coarse_time;
		//uint16_t fine_time;

		vector<HitCrystal> vHitCrystal;

		HitClover();
		HitClover(uint8_t iclover, vector<HitCrystal> v_hit_crystal);
		HitClover(uint8_t iclover, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg);
		~HitClover();

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		float AddBack();
}


#endif // __HITCLOVER__
