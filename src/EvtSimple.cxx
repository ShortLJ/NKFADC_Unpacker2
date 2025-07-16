


#include "EvtSimple.h"

EvtSimple::EvtSimple()
{
}
EvtSimple::EvtSimple(vector<SigAna> v_sigana)
{
	vSigAna=v_sigana;
}
EvtSimple::EvtSimple(vector<SigAna> v_sigana[])
{
	vector<SigAna>::iterator it_sigana;
	for (uint8_t idet=0; idet<Ndet; idet++)
	{
		for (it_sigana=v_sigana[idet].begin(); it_sigana!=v_sigana[idet].end(); it_sigana++)
		{
			vSigAna.push_back(*it_sigana);
		}
	}
}
EvtSimple::~EvtSimple()
{
}

bool EvtSimple::isValid()
{
	return true;
}
void EvtSimple::Clear()
{
	vSigAna.clear();
}
