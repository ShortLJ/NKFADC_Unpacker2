

#include "Hit.h"


Hit::Hit()
{
	idx=-1;
	Energy=-1;
	coarse_time=-1;
	fine_time=-1;
}

Hit::Hit(SigAna &sigana)
	: idx(sigana.idx), Energy(sigana.Energy), coarse_time(sigana.coarse_time), fine_time(sigana.fine_time)
{

}
Hit::Hit(uint8_t idx_, float Energy_, uint64_t coarse_time_, uint16_t fine_time_)
	: idx(idx_), Energy(Energy_), coarse_time(coarse_time_), fine_time(fine_time_)
{
}
Hit::Hit(uint8_t idx_)
	: idx(idx_)
{
}
Hit::~Hit()
{
}

void Hit::Clear()
{
	idx=-1;
	Energy=0;
	coarse_time=-1;
	fine_time=-1;
}

bool Hit::isValid()
{
	return true;
}
void Hit::ProcessHit()
{
}
void Hit::Print()
{
	fprintf(stdout,"Hit idx %u Energy %f ct %lu ft %u\n", idx,Energy,coarse_time,fine_time);
}




