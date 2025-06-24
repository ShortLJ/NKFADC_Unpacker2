
#include "Global.h"

#include <stdio.h>
#include <stdint.h>

#ifndef __SIG__
#define __SIG__

class Sig
{
	public: 
		uint32_t trigger_number;
		uint64_t coarse_time;

		uint8_t sid;
		uint8_t brd;
		uint8_t cha;

		uint16_t ADC;
		uint16_t fine_time;

		uint8_t flag; // pile_up flag @ bit1, hit flag @ bit0

#ifdef WAVEFORM
		vector<uint16_t> waveform;
#endif //WAVEFORM

		Sig() {	}
		//Sig(uint8_t *data);
		~Sig() {	}

	public: 
		void PrintSummary();

		bool operator<(const Sig sig) const
		{
			return this->coarse_time > sig.coarse_time;
		}
	protected:


	private:

};


#endif // __NKSIG__
