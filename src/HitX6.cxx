

#include "HitX6.h"

HitX6::HitX6()
{
}
HitX6::HitX6(uint8_t idet, vector<SigAna> v_sigana_ohmic, vector<SigAna> v_sigana_strpU, vector<SigAna> v_sigana_strpD)
	: det(idet)
{
	vector<SigAna>::iterator pad, strpU, strpD;
	for (pad=v_sigana_pad.begin(); pad!=v_sigana_pad.end(); pad++)
	{
		vHitPad.push_back(HitPad(pad))
	}

	for (strpU=v_sigana_strpU.begin(); strpU!=v_sigana_strpU.end(); strpU++)
	{
		for (strpD=v_sigana_strpD.begin(); strpD!=v_sigana_strpD.end(); strpD++)
		{
			if (strpU->idx==strpD->idx)
			{
				vHitStrip.push_back(HitStrip(strpU,strpD);
			}
		}
	}


	ProcessHit();
}
HitX6::~HitX6()
{
}

void HitX6::ProcessHit()
{
	if (!isValid())
	{
		fprintf(stdout,"HitX6::ProcessHit() : skip non-valid X6 hit\n");
		return;
	}
#ifdef HITX6PARTICLE
	vector<HitPad>::iterator pad;
	vector<HitStrip>::iterator strip;
	for (strip=vHitStrip.begin(); strip!=vHitStrip.end(); strip++)
	{
		for (pad=vHitPad.begin(); pad!=vHitPad.end(); pad++)
		{
			if (abs(strip->position*2 - (pad->idx-2))<1)
			{
				vHitX6Particle.push_back(HitX6Particle(pad,strip);
			}
		}
	}
#endif // HITX6PARTICLE


}

bool HitX6::isValid()
{
	bool ret=1;
	vector<HitPad>::iterator pad;
	for (pad=vHitPad.begin(); pad!=vHitPad.end(); pad++)
	{
		if (flag_pad & (0x1 << (pad->idx))) 
		{
			Print();
			pad->Print();
			fprintf(stderr,"multiple hits in pad %u\n",pad->idx);
			ret=0;
		}
		flag_pad |= 0x1<<(pad->idx);
	}
	vector<HitStrip>::iterator strip;
	for (strip=vHitStrip.begin(); strip!=vHitStrip.end(); strip++)
	{
		if (flag_strip & (0x1 << (strip->idx))) 
		{
			Print();
			strip->Print();
			fprintf(stderr,"multiple hits in strip %u\n",strip->idx);
			ret=0;
		}
		flag_strip |= 0x1<<(strip->idx);
	}


	



}
