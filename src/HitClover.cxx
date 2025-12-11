
#include <stdlib.h>
#include <cmath>

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

float HitClover::DopplerCorrE(float beta_r, float *ref_coor)
{
	if (GetPrimaryCrystal()==0) return -9999999;
	if (GetPrimaryCrystal()->GetPrimarySeg()==0) return -999999999;
	float (&seg_coor)[3] = GetPrimaryCrystal()->seg_coor;
	float &radi_seg = GetPrimaryCrystal()->radi_seg;

	double radi_ref = sqrt(ref_coor[0]*ref_coor[0]+ref_coor[1]*ref_coor[1]+ref_coor[2]*ref_coor[2]);

	double inner_product = (seg_coor[0]*ref_coor[0]+seg_coor[1]*ref_coor[1]+seg_coor[2]*ref_coor[2]);
	dcEnergy = Energy * (1-beta_r*inner_product/radi_ref/radi_seg) / sqrt(1-beta_r*beta_r);
	return dcEnergy;
}

float HitClover::AddBack()
{
	float Esum=0;
	vector<HitCrystal>::iterator it_hitcrystal, pCrystal;
	for (it_hitcrystal=vHitCrystal.begin(); it_hitcrystal!=vHitCrystal.end(); it_hitcrystal++)
	{
		Esum += it_hitcrystal->Energy;
		if (it_hitcrystal==vHitCrystal.begin()) pCrystal = it_hitcrystal;
		else
		{
			if (pCrystal->GetPrimarySeg()!=0 && it_hitcrystal->GetPrimarySeg()!=0)
				pCrystal = (pCrystal->GetPrimarySeg()->Energy > it_hitcrystal->GetPrimarySeg()->Energy) ? pCrystal : it_hitcrystal;
		}
	}
	ipCrystal = pCrystal - vHitCrystal.begin();
	return Esum;
}

HitCrystal* HitClover::GetPrimaryCrystal()
{
	if (ipCrystal==-1) return 0;
	return &vHitCrystal.at(ipCrystal);
}

bool HitClover::isValid()
{
	return true;
}
