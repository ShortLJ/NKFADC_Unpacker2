

#include "Event.h"


Event::Event()
{
}
Event::~Event()
{
}

void Event::Clear()
{
	vSigAna_RAW.clear();
	Simple.Clear();
	ASGARD.Clear();
	StarkJr.Clear();
}
