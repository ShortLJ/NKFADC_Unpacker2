

#include "EvtSimple.h"
#include "EvtASGARD.h"
#include "EvtStarkJr.h"

#ifndef __EVENT__
#define __EVENT__

class Event
{
	public: 
		Event();
		~Event();

		vector<SigAna> vSigAna_RAW;
		EvtSimple Simple;
		EvtASGARD ASGARD;
		EvtStarkJr StarkJr;

		void Clear();
};

#endif //__EVENT__


