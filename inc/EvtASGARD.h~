
#ifdef CLOVERLAYER
	#include "HitClover.h"
#else // CLOVERLAYER
	#include "HitCrystal.h"
#endif // CLOVERLAYER


#ifndef __EvtASGARD__
#define __EvtASGARD__

using namespace std;

class EvtASGARD
{
	public:
#ifdef CLOVERLAYER
		vector<HitClover> vClover;
#else // CLOVERLAYER
		vector<HitCrystal> vCrystal; 
#endif // CLOVERLAYER

		EvtASGARD();
#ifdef CLOVERLAYER
		EvtASGARD(vector<HitClover> v_clover);
		EvtASGARD(vector<HitCrystal> v_crystal);
#else // CLOVERLAYER
		EvtASGARD(vector<HitCrystal> v_crystal);
		EvtASGARD(vector<Sig> v_sig_fv, vector<Sig> v_sig_seg);
#endif // CLOVERLAYER
		~EvtASGARD();

		void Clear();
		virtual bool isValid();


	private:

}



#endif //__EvtASGARD__
