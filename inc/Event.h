

#include "EvtASGARD.h"
#include "EvtStarkJr.h"

#ifndef __EVENT__
#define __EVENT__

class Event
{
	public: 
		EvtASGARD ASGARD;
		EvtStarkJr StarkJr;

		void Clear();
}

#endif //__EVENT__

Event::Event()
{
}
Event::~Event()
{
}

void Event::Clear()
{
	ASGARD.Clear();
	StarkJr.Clear();
}
