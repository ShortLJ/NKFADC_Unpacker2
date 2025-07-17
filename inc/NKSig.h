

//#include <stdio.h>
#include <stdint.h>

#include "Sig.h"

#ifndef __NKSIG__
#define __NKSIG__

class NKSig : public Sig
{
	public: 
		NKSig() {	}
		NKSig(uint8_t *data);
		~NKSig() {	}

		Sig GetSig(){Sig ret = *this; return ret;}

	public: 
		void Print(int verbosity=0);

		//bool operator<(const NKSig sig) const
		//{
		//	return this->local_gate_time > sig.local_gate_time;
		//}
	protected:


	private:
		uint8_t data_length;
		uint8_t trigger_type;

		uint32_t tcb_trigger_number;
		//uint16_t tcb_trigger_fine_time;
		//uint64_t tcb_trigger_coarse_time;
		uint64_t tcb_trigger_time;

		//uint8_t sid;
		//uint8_t mid;
		//uint8_t cha;
		bool ADC_type;

		uint32_t local_trigger_pattern;
		uint32_t local_trigger_number;
		//uint16_t local_gate_fine_time;
		//uint64_t local_gate_coarse_time;
		uint64_t local_gate_time;

		//uint16_t peak;
		//uint16_t timing;

		//uint8_t flag; // pile_up flag @ bit1, hit flag @ bit0

};


#endif // __NKSIG__
