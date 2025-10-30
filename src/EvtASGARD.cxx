


#include "EvtASGARD.h"

EvtASGARD::EvtASGARD()
{
}
EvtASGARD::EvtASGARD(vector<HitClover> v_hit_clover)
{
	vHitClover=v_hit_clover;
}
EvtASGARD::~EvtASGARD()
{
}
EvtASGARD::EvtASGARD(vector<HitCrystal> v_hit_crystal)
{
	vHitClover.clear();
	vector<HitCrystal> v_hit_crystal_sort[Nclover];
	vector<HitCrystal>::iterator crystal;
	for (crystal=v_hit_crystal.begin(); crystal!=v_hit_crystal.end(); crystal++)
	{
		uint8_t iclover = (crystal->detID)>>2;
		v_hit_crystal_sort[iclover].push_back(*crystal);
	}
	for (uint8_t iclover=0; iclover<Nclover; iclover++)
	{
		if (v_hit_crystal_sort[iclover].size()>0)
		{
			vHitClover.push_back(HitClover(iclover,v_hit_crystal_sort[iclover]));
		}
	}
}

bool EvtASGARD::isValid()
{
	return true;
}
void EvtASGARD::Clear()
{
	vHitClover.clear();
}
