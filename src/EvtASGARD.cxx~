


#include "EvtASGARD.h"

EvtASGARD::EvtASGARD()
{
}
#ifdef CLOVERLAYER
EvtASGARD::EvtASGARD(vector<HitClover> v_clover)
{
	vClover=v_clover;
}
EvtASGARD::EvtASGARD(vector<HitCrystal> v_crystal)
{
	vClover.Clear();
	constexpr Nclover = Ndet/4;
	vector<HitCrystal> v_crystal_sort[Nclover];
	vector<HitCrystal>::iterator crystal;
	for (crystal=v_crystal.begin(); crystal!=v_crystal.end(); crystal++)
	{
		uint8_t iclover = (crystal->det)>>2;
		v_crystal_sort[iclover].push_back(crystal);
	}
	for (uint8_t iclover=0; iclover<Nclover; iclover++)
	{
		if (v_crystal_sort[iclover].size()>0)
		{
			vClover.push_back(HitClover(iclover,v_crystal_sort[iclover]));
		}
	}
}
#else // CLOVERLAYER
EvtASGARD::EvtASGARD(vector<HitCrystal> v_crystal)
{
	vCrystal=v_crystal;
}
EvtASGARD::EvtASGARD(vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg)
{
	vector<SigAna>::iterator it_sigana;
	vector<SigAna> v_sigana_fv_sort[Ndet];
	for (it_sigana=v_sigana_fv.begin(); it_sigana!=v_sigana_fv.end(); it_sigana++)
	{
		v_sigana_fv_sort[it_sigana->det].push_back(*it_sigana);
	}
	vector<SigAna> v_sigana_seg_sort[Ndet];
	for (it_sigana=v_sigana_seg.begin(); it_sigana!=v_sigana_seg.end(); it_sigana++)
	{
		v_sigana_seg_sort[it_sigana->det].push_back(*it_sigana);
	}
	for (uint8_t idet=0; idet<Ndet; idet++)
	{
		if (v_sigana_fv_sort[idet].size()+v_sigana_seg_sort[idet].size()>0)
		{
			vCrystal.push_back(HitCrystal(idet,v_sigana_fv_sort[idet],v_sigana_seg_sort[idet]));
		}
	}
}
#endif // CLOVERLAYER

