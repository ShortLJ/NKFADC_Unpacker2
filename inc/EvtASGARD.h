
#include "HitClover.h"

#ifndef __EvtASGARD__
#define __EvtASGARD__

using namespace std;

class EvtASGARD
{
	public:
		vector<HitClover> vHitClover;

		EvtASGARD();
		EvtASGARD(vector<HitClover> v_hit_clover);
		EvtASGARD(vector<HitCrystal> v_hit_crystal);
		~EvtASGARD();

		void Clear();
		virtual bool isValid();


	private:

}



#endif //__EvtASGARD__
