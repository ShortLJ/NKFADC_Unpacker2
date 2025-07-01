

#include "HitCrystal.h"

HitCrystal::HitCrystal()
{
}
HitCrystal::HitCrystal(uint8_t icrystal, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg)
	: idx(icrystal), vSigAnaFV(v_sigana_fv), vSigAnaSeg(v_sigana_seg)
{
	if(vSigAnaFV.size()) detID = vSigAnaFV.begin()->det;
	else if (v_sigana_seg.size()) detID = v_sigana_seg.begin()->det;
	else 
	{
		fprintf(stderr,"HitCrystal::HitCrystal(uint8_t, vector<SigAna>,vector<SigAna>) : ?????????\n");
		exit(-32);
	}
	cloverID = detID >> 2;

	ProcessHit();
}
HitCrystal::HitCrystal(uint8_t iclover, uint8_t icrystal, vector<SigAna> v_sigana_fv, vector<SigAna> v_sigana_seg)
	: cloverID(iclover), idx(icrystal), vSigAnaFV(v_sigana_fv), vSigAnaSeg(v_sigana_seg)
{
	detID = (cloverID<<2) | icrystal ; 

	ProcessHit();
}
HitCrystal::~HitCrystal()
{
}

void HitCrystal::ProcessHit()
{
	Process_Esum();
	Energy = E_fv_sum;
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


