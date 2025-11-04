
//#define STRIP_N_TEST 1

#include "HitX6.h"

HitX6::HitX6()
{
}
HitX6::HitX6(uint8_t idet, vector<SigAna> &v_sigana_pad, vector<SigAna> &v_sigana_strpU, vector<SigAna> &v_sigana_strpD)
	: Hit(idet)
{
	vector<SigAna>::iterator sigpad, sigstrpU, sigstrpD;
	for (sigpad=v_sigana_pad.begin(); sigpad!=v_sigana_pad.end(); sigpad++)
	{
		vHitPad.emplace_back(*sigpad);
	}

#ifdef STRIP_N_TEST
	counts_stripU=0; counts_stripD=0;
	for (sigstrpU=v_sigana_strpU.begin(); sigstrpU!=v_sigana_strpU.end(); sigstrpU++)
		counts_stripU += 0x1<<(sigstrpU->idx << 2);
	for (sigstrpD=v_sigana_strpD.begin(); sigstrpD!=v_sigana_strpD.end(); sigstrpD++)
		counts_stripD += 0x1<<(sigstrpD->idx << 2);
	if (counts_stripU!=counts_stripD)
	{
		fprintf(stderr, "X6 #%02d:   %016lX\n", idet, 0xFEDCBA9876543210);
		fprintf(stderr, "X6 #%02d: U %016lX\n", idet, counts_stripU);
		fprintf(stderr, "X6 #%02d: D %016lX\n", idet, counts_stripD);
	}
#endif // STRIP_N_TEST


	for (sigstrpU=v_sigana_strpU.begin(); sigstrpU!=v_sigana_strpU.end(); sigstrpU++)
	{
		for (sigstrpD=v_sigana_strpD.begin(); sigstrpD!=v_sigana_strpD.end(); sigstrpD++)
		{
			if (sigstrpU->idx==sigstrpD->idx)
			{
				vHitStrip.emplace_back(*sigstrpU,*sigstrpD);
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
	flag_pad=0;
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
	flag_strip=0;
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
	return ret;
}
