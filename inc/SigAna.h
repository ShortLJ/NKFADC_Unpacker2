#include "Global.h"
#include "Sig.h"

#ifndef __SIGANA__
#define __SIGANA__

extern uint8_t	map_type	[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_det		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_idx		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};

extern float Ecal_par0		[N_SID][N_BRD][N_CHA];
extern float Ecal_par1		[N_SID][N_BRD][N_CHA];
extern float Ecal_par2		[N_SID][N_BRD][N_CHA];

using namespace std;

class SigAna : public Sig
{
	public:
		uint8_t type;
		uint8_t det;
		uint8_t idx;

		float Energy; // 32 bit: 1bit sign, 8bit exponent, 23bit fraction > 16 bit ADC

		SigAna(){	};
		SigAna(Sig sig);
		SigAna(uint8_t type, uint8_t det, uint8_t idx, Sig sig);
		~SigAna(){	};


	public:
		bool isValid();
		void ProcessSigAna();
		//void Clear();
};



#endif // __SIGANA__



