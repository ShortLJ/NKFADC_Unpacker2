
#include <vector>

#include "Hit.h"
#include "SigAna.h"

#ifndef __HITCRYSTAL__
#define __HITCRYSTAL__

using namespace std;

class HitCrystal : public Hit
{
	public:
		//uint8_t idx; // crystal idx in a clover: 0,1,2,3
		//float Energy;
		//uint64_t coarse_time;
		//uint16_t fine_time;

		uint8_t detID; // idx+4*cloverID
		uint8_t cloverID;
		float E_seg_sum;
		float E_fv_avg;
		uint8_t primary = -1;
		float dcEnergy=-1;

		vector<SigAna> vSigAnaFV;
		vector<SigAna> vSigAnaSeg;

		HitCrystal();
		HitCrystal(uint8_t detID, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg);
		HitCrystal(uint8_t iclover, uint8_t icrystal, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg);
		~HitCrystal();

		float DopplerCorrE(double beta_r, double beta_theta=0, double beta_phi=0);

	private:
		bool isValid();
		void ProcessHit();
		//void Clear();

		void Process_Esum();
		void Process_Primary();


};



#endif //__HITCRYSTAL__
