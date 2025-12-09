
//#define STRIP_N_TEST 1

#include "HitX6.h"
const float pos_cal[Nx6][Nstrip][3] = {
{   // 0
{-0.005078,2.016418,-0.171583},
{0.002383,2.036922,0.202373},
{-0.003127,2.023808,0.146899},
{0.003130,2.030999,0.040556},
{0.000000,2.000000,0.000000},
{0.012459,2.013924,0.053466},
{0.004371,1.996151,-0.089507},
{0.003439,2.002590,0.050386}},
{   // 1
{-0.002816,1.995909,0.160208},
{0.015339,1.985722,0.032811},
{0.023087,1.993542,-0.162109},
{0.003345,1.975361,0.024371},
{0.019559,2.048854,0.195105},
{-0.004683,2.005958,-0.043167},
{0.012913,2.008152,0.043291},
{-0.000618,2.045592,-0.045465}},
{   // 2
{0.018218,2.069045,-0.067768},
{-0.005080,2.077008,-0.052068},
{0.010091,2.106266,0.019766},
{0.002312,2.067846,0.152068},
{-0.003044,2.105786,0.188153},
{0.013254,2.107503,-0.167300},
{0.010799,2.119132,0.236989},
{0.005815,2.101404,-0.128959}},
{   // 3
{0.009428,2.677354,-0.027231},
{0.010446,2.680597,0.150513},
{0.010039,2.727331,-0.230825},
{0.007817,2.656990,-0.197447},
{0.008494,2.687125,0.032113},
{0.004546,2.674739,-0.125523},
{0.017978,2.637715,0.009151},
{-0.002593,2.691255,0.516001}},
{   // 4
{0.000292,2.083009,0.161519},
{-0.003229,2.057230,0.105185},
{0.001884,2.092018,-0.081426},
{-0.003906,2.095130,0.198036},
{-0.004223,2.063543,0.066972},
{-0.010953,2.060433,0.245445},
{-0.008017,1.999445,0.162244},
{-0.007628,2.044311,0.130851}},
{   // 5
{0.004860,2.296006,0.124930},
{0.008854,2.266638,-0.071927},
{0.006029,2.299751,-0.048218},
{-0.011200,2.248395,-0.027732},
{-0.008324,2.325379,0.379303},
{0.011457,2.271621,0.109937},
{0.006203,2.231606,-0.094309},
{0.004503,2.255591,-0.032841}},
{   // 6
{0.007480,2.698796,-0.167485},
{0.012298,2.694609,-0.053115},
{-0.004951,2.626255,0.209964},
{0.015619,2.639656,-0.294678},
{0.022227,2.588113,-0.159946},
{0.003444,2.615990,-0.046661},
{-0.002889,2.667561,0.365438},
{-0.011445,2.622052,0.104949}},
{   // 7
{0.012476,2.012235,0.014478},
{-0.002774,2.018176,0.292631},
{-0.001700,1.982116,0.041309},
{0.012671,1.971708,-0.127652},
{-0.018420,1.973213,0.295195},
{0.001472,2.016677,0.137823},
{0.000819,1.979163,0.088025},
{0.008567,1.943514,-0.216868}},
{   // 8
{0.009793,1.700115,-0.138705},
{-0.003562,1.705288,0.006628},
{0.008162,1.704358,-0.080009},
{0.002884,1.712781,0.009161},
{0.005577,1.699615,-0.022835},
{0.000923,1.706157,-0.015152},
{0.002540,1.694747,-0.087886},
{0.002029,1.733860,0.022215}},
{   // 9
{-0.008352,1.709914,0.064275},
{-0.001707,1.714110,-0.008597},
{-0.005024,1.705521,0.056627},
{0.013900,1.715349,-0.071828},
{-0.005346,1.726404,0.162850},
{0.007524,1.722184,-0.071602},
{0.014282,1.711536,-0.045699},
{-0.009848,1.718183,0.061122}},
{   // 10
{0.001649,2.532905,0.110673},
{0.008198,2.549559,-0.197174},
{0.013137,2.540727,0.202679},
{0.014785,2.579906,-0.131164},
{0.012209,2.509860,-0.287880},
{0.004531,2.615766,-0.228630},
{-0.010214,2.508397,-0.106190},
{0.008184,2.526349,0.171330}},
{   // 11
{0.004375,2.072706,-0.179949},
{-0.002335,2.098205,0.026231},
{-0.000864,2.089437,0.064985},
{0.009882,2.111622,-0.134111},
{0.000179,2.102327,0.008317},
{0.013100,2.125479,0.042847},
{-0.000255,2.071783,-0.018302},
{0.024666,2.089796,-0.212918}}};

HitPad::HitPad(SigAna &sigana)
	 : Hit(sigana.idx), sigPad(sigana) 
{
	Energy = sigana.Energy;
	coarse_time = sigana.coarse_time;
	fine_time = sigana.fine_time;
}

void HitStrip::ProcessHit()
{
	Energy = (sigStripU.Energy + sigStripD.Energy);
	//int16_t diff = sigStripU.ADC-sigStripD.ADC;
	//int16_t sum  = sigStripU.ADC+sigStripD.ADC;
	float diff = sigStripU.Energy-sigStripD.Energy;
	float sum  = sigStripU.Energy+sigStripD.Energy;
	if (sum>0)
	{
		position = float(diff)/(sum);
		//fprintf(stdout,"%d %d -> %d %d -> %f\n", sigStripU.ADC,sigStripD.ADC, diff,sum, position);
		uint8_t ix6 = sigStripU.det;
		position = pos_cal[ix6][idx][0]
			+ pos_cal[ix6][idx][1] * position
			+ pos_cal[ix6][idx][2] * position * position;
		hit_coor[0] = strip_pos_cart[ix6][idx][0];
		hit_coor[1] = strip_pos_cart[ix6][idx][1];
		hit_coor[2] = strip_pos_cart[ix6][idx][2];

		if (ix6<6) 	hit_coor[2] -= position * 75/2;
		else		hit_coor[2] += position * 75/2;
	}
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
