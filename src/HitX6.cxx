
//#define STRIP_N_TEST 1

#include "HitX6.h"

const float pos_cal[Nx6][Nstrip][3] = {
{
{-0.050146,1.997091,0.006703},
{-0.002985,2.004444,-0.012077},
{-0.015801,2.008746,-0.031184},
{-0.012112,2.011341,0.093391},
{9603.634737,114002.788227,-58094.643816},
{-0.050376,2.024220,0.110896},
{-0.035109,2.012824,0.055057},
{-0.004738,2.004082,0.052011}},
{
{-0.002395,1.995452,0.010320},
{-0.009601,1.997124,0.005172},
{-0.009638,1.997190,0.011614},
{0.013340,1.993311,-0.084107},
{-0.025797,1.988466,-0.024268},
{-0.000348,2.018457,-0.042201},
{-0.021026,1.993490,0.040197},
{-0.003768,2.017628,0.040508}},
{
{-0.034120,2.103279,-0.013989},
{0.010186,2.105206,0.067657},
{0.019959,2.113999,-0.097701},
{0.007683,2.093910,-0.076376},
{0.028137,1.987167,-0.414276},
{0.028876,2.017122,-0.470188},
{0.014828,2.104632,0.047306},
{-0.017540,2.136356,0.052526}},
{
{-0.062626,2.760926,0.047009},
{-0.027837,2.722047,-0.051294},
{-0.013562,2.734311,-0.069443},
{0.018116,2.675245,-0.273682},
{0.033211,2.713847,-0.047812},
{-0.064423,2.726891,0.081818},
{-0.015951,2.677651,-0.015248},
{0.043683,2.673208,-0.147533}},
{
{-0.007293,2.101264,0.001525},
{-0.014607,2.066683,-0.030959},
{-0.037755,2.059407,-0.099145},
{-0.023200,2.076344,0.163965},
{0.005096,2.063965,-0.015573},
{0.023569,2.047051,-0.038735},
{-0.034647,2.031279,0.043335},
{0.037152,2.039102,0.003908}},
{
{-0.092545,2.273126,-0.013474},
{0.021426,2.285711,-0.135233},
{-0.041487,2.293134,0.016192},
{-0.038991,2.242717,-0.086666},
{0.036816,2.251657,-0.151125},
{-0.021813,2.240051,0.087260},
{-0.006594,2.269478,-0.041634},
{0.009389,2.243599,0.000142}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}},
{
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000},
{0.000000,1.000000,0.000000}}};

void HitStrip::ProcessHit()
{
	Energy = (sigStripU.Energy + sigStripD.Energy);
	//position = (sigStripU.Energy - sigStripD.Energy)/(sigStripU.Energy + sigStripD.Energy);
	int16_t diff = sigStripU.ADC-sigStripD.ADC;
	int16_t sum  = sigStripU.ADC+sigStripD.ADC;
	//int16_t diff = sigStripU.ADC-sigStripD.ADC;
	//int16_t sum  = sigStripU.ADC+sigStripD.ADC-8000;
	if (sum>0)
	{
		position = float(diff)/(sum);
		//fprintf(stdout,"%d %d -> %d %d -> %f\n", sigStripU.ADC,sigStripD.ADC, diff,sum, position);
	}
	position = pos_cal[sigStripU.det][idx][0]
			+ pos_cal[sigStripU.det][idx][1] * position
			+ pos_cal[sigStripU.det][idx][2] * position * position;
}


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
