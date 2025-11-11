

#include "Event.h"


Event::Event()
{
}
Event::~Event()
{
}

void Event::Clear()
{
	ref_lgt=0;
	vSigAna_RAW.clear();
	Simple.Clear();
	ASGARD.Clear();
	StarkJr.Clear();
}
