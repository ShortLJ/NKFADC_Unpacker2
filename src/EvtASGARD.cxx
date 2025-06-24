


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
EvtASGARD::EvtASGARD(vector<Sig> v_sig_fv, vector<Sig> v_sig_seg)
{
	vector<Sig>::iterator it_sig;
	vector<Sig> v_sig_fv_sort[Ndet];
	for (it_sig=v_sig_fv.begin(); it_sig!=v_sig_fv.end(); it_sig++)
	{
		uint8_t idet = map_det[(*it_sig).isid][(*it_sig).imid][(*it_sig).icha];
		v_sig_fv_sort[idet].push_back(*it_sig);
	}
	vector<Sig> v_sig_seg_sort[Ndet];
	for (it_sig=v_sig_seg.begin(); it_sig!=v_sig_seg.end(); it_sig++)
	{
		uint8_t idet = map_det[(*it_sig).isid][(*it_sig).imid][(*it_sig).icha];
		v_sig_seg_sort[idet].push_back(*it_sig);
	}
	for (uint8_t idet=0; idet<Ndet; idet++)
	{
		if (v_sig_fv_sort[idet].size()+v_sig_seg_sort[idet].size()>0)
		{
			vCrystal.push_back(HitCrystal(idet,v_sig_fv_sort[idet],v_sig_seg_sort[idet]));
		}
	}
}
#endif // CLOVERLAYER

