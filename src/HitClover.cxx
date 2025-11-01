
#include <stdlib.h>

#include "HitClover.h"



HitClover::HitClover()
{
}

HitClover::HitClover(uint8_t iclover)
	: Hit(iclover)
{

}

HitClover::HitClover(uint8_t iclover, vector<HitCrystal> &v_hit_crystal)
	: Hit(iclover), vHitCrystal(v_hit_crystal)
{
	
	ProcessHit();
}

HitClover::HitClover(uint8_t iclover, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg)
{
	vector<SigAna> v_sigana_sort_fv[4], v_sigana_sort_seg[4];
	vector<SigAna>::iterator it_sigana;
	for (it_sigana=v_sigana_fv.begin(); it_sigana!=v_sigana_fv.end(); it_sigana++)
	{
		if ((it_sigana->det >> 2) != iclover)
		{
			fprintf(stderr,"HitClover::HitClover(uint8_t iclover, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg):\nPut idx for clover number.\n");
			continue;
		}
		uint8_t icrstl =  (it_sigana->det)&0x3;
		v_sigana_sort_fv[icrstl].push_back(*it_sigana);
	}
	for (it_sigana=v_sigana_seg.begin(); it_sigana!=v_sigana_seg.end(); it_sigana++)
	{
		if ((it_sigana->det >> 2) != iclover)
		{
			fprintf(stderr,"HitClover::HitClover(uint8_t iclover, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg):\nPut idx for clover number.\n");
			continue;
		}
		uint8_t icrstl =  (it_sigana->det)&0x3;
		v_sigana_sort_seg[icrstl].push_back(*it_sigana);
	}

	for (uint8_t icrstl=0; icrstl<4; icrstl++)
	{
		vHitCrystal.emplace_back(icrstl, v_sigana_sort_fv[icrstl], v_sigana_sort_seg[icrstl]);
	}

	ProcessHit();

}
HitClover::~HitClover()
{
}
void HitClover::Clear()
{
	Hit::Clear();
	vHitCrystal.clear();
}

void HitClover::ProcessHit()
{
	Energy = AddBack();
}

float HitClover::AddBack()
{
	float ret=0;
	vector<HitCrystal>::iterator it_hitcrystal;
	for (it_hitcrystal=vHitCrystal.begin(); it_hitcrystal!=vHitCrystal.end(); it_hitcrystal++)
	{
		ret += it_hitcrystal->Energy;
	}
	return ret;
}

bool HitClover::isValid()
{
	return true;
}
