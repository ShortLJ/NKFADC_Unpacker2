
#include <vector>

#include "Hit.h"
#include "HitCrystal.h"
#include "SigAna.h"

#ifndef __HITCLOVER__
#define __HITCLOVER__

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
		HitClover(uint8_t iclover, vector<HitCrystal> &v_hit_crystal);
		HitClover(uint8_t iclover, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg);
		~HitClover();

	protected:
		void Clear();

	private:
		bool isValid();
		void ProcessHit();

		float AddBack();
};

#endif // __HITCLOVER__
