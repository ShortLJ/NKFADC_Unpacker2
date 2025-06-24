

#include "HitX6.h"

HitX6::HitX6()
{
}
HitX6::HitX6(uint8_t idet, vector<SigAna> v_sigana_ohmic, vector<SigAna> v_sigana_strpU, vector<SigAna> v_sigana_strpD)
	: det(idet), vSigAnaOhmic(v_sigana_ohmic), vSigAnaStripU(v_sigana_strpU), vSigAnaStripD(v_sigana_strpD)
{
	ProcessHit();
}
HitX6::~HitX6()
{
}

void HitX6::ProcessHit()
{
	vector<SigAna>::iterator pad, strpU, strpD;

	for (strpU=vSigAnaStripU.begin(); strpU!=vSigAnaStripU.end(); strpU++)
	{
		for (strpD=vSigAnaStripD.begin(); strpD!=vSigAnaStripD.end(); strpD++)
		{
			if (strpU->idx==strpD->idx)
			{
				
			}
		}
	}

}

bool HitX6::isValid()
{
	if (abs(E_seg_sum-E_fv_avg)>100) return false;
	return true
}
