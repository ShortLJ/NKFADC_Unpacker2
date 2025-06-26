

#define X6_RAW_SIGANA 1


#include "Hit.h"
#include "SigAna.h"

#ifndef __HITX6__
#define __HITX6__

using namespace std;

class HitX6
{
	public:
		uint8_t det;
		
#ifdef X6_RAW_SIGANA
		vector<SigAna> vSigAnaOhmic;
		vector<SigAna> vSigAnaStripU;
		vector<SigAna> vSigAnaStripD;
#endif //X6_RAW_SIGANA

		HitX6();
		HitX6(uint8_t det, vector<SigAna> v_sigana_ohmic, vector<SigAna> v_sigana_strpU, vector<SigAna> v_sigana_strpD);
		~HitX6();

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		void SetValues(Int_t _X6ID, Int_t _stripID, Int_t _padID, Int_t _stripValU, Int_t _stripValD, Int_t _padVal);

}



#endif //__HITX6__
