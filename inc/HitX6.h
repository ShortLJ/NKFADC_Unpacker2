

#define X6_RAW_SIGANA 1


#include <vector>

#include "Hit.h"
#include "SigAna.h"

#ifndef __HITPAD__
#define __HITPAD__

extern float strip_pos_cart [Nx6][Nstrip][3];

class HitPad : public Hit
{
	public:
		//uint8_t idx;
		//float Energy;
		//uint64_t coarse_time;
		//uint16_t fine_time;

		SigAna sigPad;

		HitPad(){	}
		HitPad(SigAna &sigana);
};

#endif //__HITPAD__

#ifndef __HITSTRIP__
#define __HITSTRIP__

class HitStrip : public Hit
{
	public:
		//uint8_t idx;
		//float Energy;
		//uint64_t coarse_time;
		//uint16_t fine_time;

		float position;

		SigAna sigStripU;
		SigAna sigStripD;

		HitStrip(){	}
		HitStrip(SigAna &siganaU, SigAna &siganaD) : Hit(siganaU.idx), sigStripU(siganaU), sigStripD(siganaD)
		{	
			ProcessHit();
		}
		void ProcessHit();
		bool isValid()
		{
			return true;
		}
};


#endif //__HITSTRIP__

#ifndef __HITX6__
#define __HITX6__

using namespace std;

class HitX6 : public Hit
{
	public:
		//uint8_t idx;
		//float Energy;
		//uint64_t coarse_time;
		//uint16_t fine_time;
		
		vector<HitPad> vHitPad;
		vector<HitStrip> vHitStrip;

#ifdef HITX6PARTICLE
		vector<HitX6Particle> vHitX6Particle;
#endif // HITX6PARTICLE

		HitX6();
		HitX6(uint8_t det, vector<SigAna> &v_sigana_pad, vector<SigAna> &v_sigana_strpU, vector<SigAna> &v_sigana_strpD);
		~HitX6();

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		uint8_t flag_pad=0;
		uint16_t flag_strip=0;

		uint64_t counts_stripU=0, counts_stripD=0;

};



#endif //__HITX6__


