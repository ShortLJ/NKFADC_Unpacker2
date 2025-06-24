

#include "HitCrystal.h"

HitCrystal::HitCrystal()
{
}
HitCrystal::HitCrystal(uint8_t idet, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg)
	: det(idet), vSigAnaFV(v_sigana_fv), vSigAnaSeg(v_sigana_seg)
{
	clover = det>>2;
	crystal = det&0x03;

	ProcessHit();
}
HitCrystal::~HitCrystal()
{
}

void HitCrystal::ProcessHit()
{
	Process_Esum();
}
bool HitCrystal::isValid()
{
	if (abs(E_seg_sum-E_fv_avg)>100) return false;
	return true
}
void HitCrystal::Process_Esum()
{
	E_seg_sum = 0;
	vector<SigAna>::iterator seg;
	for (seg=vSigAnaSeg.begin(); seg!=vSigAnaSeg.end(); seg++)
	{
		E_seg_sum+=seg->Energy;
	}
	E_fv_avg = 0;
	vector<SigAna>::iterator fv;
	for (fv=vSigAnaFV.begin(); fv!=vSigAnaFV.end(); fv++)
	{
		E_fv_sum+=fv->Energy/2.;
	}
}


