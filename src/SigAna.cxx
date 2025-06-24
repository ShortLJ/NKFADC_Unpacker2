#include "SigAna.h"


SigAna::SigAna(Sig sig)
	: Sig(sig)
{
	type = map_type[sig.sid][sig.mid][sig.cha];
	det = map_det[sig.sid][sig.mid][sig.cha];
	idx = map_idx[sig.sid][sig.mid][sig.ch];

	Energy=-1;
	ProcessHit();
}

SigAna::SigAna(uint8_t itype, uint8_t idet, uint8_t iidx, Sig sig)
	: type(itype), det(idet), idx(iidx), Sig(sig)
{	
	Energy=-1;
	ProcessHit();
}



bool SigAna::isValid()
{

}

void SigAna::ProcessSigAna()
{
	// Ecal, etc
}
