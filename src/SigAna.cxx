#include "SigAna.h"


SigAna::SigAna(Sig sig)
	: Sig(sig)
{
	type = map_type[sig.sid][sig.brd][sig.cha];
	det = map_det[sig.sid][sig.brd][sig.cha];
	idx = map_idx[sig.sid][sig.brd][sig.cha];

	Energy=-1;
	ProcessSigAna();
}

SigAna::SigAna(uint8_t itype, uint8_t idet, uint8_t iidx, Sig sig)
	: type(itype), det(idet), idx(iidx), Sig(sig)
{	
	Energy=-1;
	ProcessSigAna();
}



bool SigAna::isValid()
{
	return true;
}

void SigAna::ProcessSigAna()
{
	// Ecal, etc
	if (type==3) // pad
	{
		Energy = ADC;
		// if (det == 3 && idx==2) Energy = Energy * 0.9;
	}
	if (type==4) // stripU
	{
		Energy = ADC;
	}
	if (type==5) // stripD
	{
		Energy = ADC;
	}
}
