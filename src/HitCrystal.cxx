

#include "HitCrystal.h"

HitCrystal::HitCrystal()
{
}
HitCrystal::HitCrystal(uint8_t idet_, vector<Sig> v_sig_fv, vector<Sig> v_sig_seg)
	: idet(idet_)
{
	iclover = idet>>2;
	icrystal = idet&0x03;

	vector<Sig>::iterator sig;
	for (sig=v_sig_fv.begin(); sig!=v_sig_fv.end(); sig++)
	{
		vHitFV.push_back(HitFV(*sig));
	}
	for (sig=v_sig_seg.begin(); sig!=v_sig_seg.end(); sig++)
	{
		vHitSeg.push_back(HitSeg(*sig));
	}
///////
	E_seg_sum = 0;
	vector<HitSeg>::iterator seg;
	for (seg=vHitSeg.begin(); seg!=vHitSeg.end(); seg++)
	{
		E_seg_sum+=seg->Ecal;
	}
	E_fv_avg = 0;
	vector<HitFV>::iterator fv;
	for (fv=vHitFV.begin(); fv!=vHitFV.end(); fv++)
	{
		E_fv_sum+=fv->Ecal/2.;
	}




//////
}

bool HitCrystal::isValid()
{
	if (abs(E_seg_sum-E_fv_avg)>100) return false;
	return true
}
