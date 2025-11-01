


#include "SigAna.h"


#ifndef __HIT__
#define __HIT__

class Hit
{
	public:
		uint8_t idx;
		float Energy;
		uint64_t coarse_time;
		uint16_t fine_time;

		Hit();
		Hit(SigAna &sigana);
		Hit(uint8_t idx, float Energy, uint64_t coarse_time, uint16_t fine_time);
		Hit(uint8_t idx);
		~Hit();

	protected:
		void Clear();

	public:
		virtual bool isValid();
		virtual void ProcessHit();
		virtual void Print();

};


#endif //__HIT__
