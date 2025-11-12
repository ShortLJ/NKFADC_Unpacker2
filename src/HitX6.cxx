
//#define STRIP_N_TEST 1

#include "HitX6.h"

const float pos_cal[Nx6][Nstrip][3] = {
{
{-0.005078,2.016425,-0.171544},
{0.002386,2.036919,0.202316},
{-0.003128,2.023809,0.146908},
{0.003130,2.030999,0.040557},
{0.000000,2.000000,0.000000},
{0.012459,2.013926,0.053474},
{0.004371,1.996151,-0.089509},
{0.003439,2.002592,0.050396}},
{
{-0.002816,1.995909,0.160208},
{0.015337,1.985729,0.032859},
{0.023087,1.993539,-0.162125},
{0.003345,1.975361,0.024372},
{0.019559,2.048861,0.195131},
{-0.004683,2.005958,-0.043166},
{0.012913,2.008153,0.043308},
{-0.000618,2.045592,-0.045470}},
{
{0.018219,2.069046,-0.067792},
{-0.005081,2.077014,-0.052017},
{0.010090,2.106267,0.019778},
{0.002312,2.067846,0.152068},
{-0.003044,2.105787,0.188152},
{0.013253,2.107503,-0.167287},
{0.010799,2.119124,0.236960},
{0.005815,2.101394,-0.128960}},
{
{0.009428,2.677354,-0.027244},
{0.010446,2.680597,0.150509},
{0.010040,2.727334,-0.230845},
{0.007817,2.656985,-0.197459},
{0.008494,2.687126,0.032134},
{0.004546,2.674738,-0.125527},
{0.017976,2.637726,0.009218},
{-0.002594,2.691255,0.516015}},
{
{0.000292,2.083008,0.161512},
{-0.003229,2.057229,0.105189},
{0.001885,2.092030,-0.081403},
{-0.003904,2.095131,0.198013},
{-0.004224,2.063542,0.066973},
{-0.010953,2.060433,0.245446},
{-0.008017,1.999445,0.162245},
{-0.007628,2.044309,0.130844}},
{
{0.004860,2.296009,0.124949},
{0.008855,2.266638,-0.071950},
{0.006026,2.299750,-0.048153},
{-0.011200,2.248394,-0.027736},
{-0.008324,2.325362,0.379223},
{0.011456,2.271619,0.109930},
{0.006203,2.231607,-0.094308},
{0.004503,2.255584,-0.032880}},
{
{-0.004908,5.704496,1.425636},
{-0.011199,5.781526,0.058326},
{0.006892,5.570554,-0.295441},
{-0.015541,5.695810,0.652476},
{-0.016136,5.568992,-0.038307},
{-0.002790,5.582324,0.444149},
{-0.000885,5.820248,-2.256471},
{0.007749,5.650332,-0.406904}},
{
{-0.009017,4.282851,-0.118083},
{-0.000841,4.221822,-0.173406},
{0.000517,4.268623,-0.150566},
{-0.014627,4.269659,0.401799},
{0.006310,4.216798,-0.588761},
{-0.003530,4.321348,-0.592482},
{-0.001640,4.304955,-0.430857},
{-0.004684,4.247542,0.158904}},
{
{-0.005270,3.683966,0.076807},
{0.006197,3.672298,-0.413564},
{-0.006591,3.624251,0.407380},
{-0.004460,3.713641,0.022761},
{-0.003872,3.649076,-0.044060},
{0.000702,3.651975,-0.120415},
{-0.004754,3.661677,0.348657},
{-0.002320,3.699770,0.083666}},
{
{0.007364,3.623127,0.069795},
{-0.003146,3.672488,0.262499},
{0.000754,3.697257,0.106506},
{-0.014413,3.692976,0.631335},
{-0.002238,3.648576,0.198324},
{-0.007630,3.718533,0.235157},
{-0.008760,3.714198,0.244121},
{0.008399,3.676148,-0.171521}},
{
{-0.005290,4.540567,10.364300},
{-0.012054,5.464184,2.207793},
{-0.007533,5.455608,-1.044964},
{-0.010834,5.512031,0.901565},
{-0.009556,5.366866,1.121281},
{-0.003748,5.591922,0.667134},
{0.007245,5.473073,0.167833},
{-0.011500,5.387357,0.451659}},
{
{-0.003518,4.525580,0.209958},
{-0.002980,4.494771,0.286415},
{-0.005691,4.535553,0.020440},
{-0.000123,4.539440,0.048008},
{-0.003879,4.577896,0.032846},
{-0.010845,4.550602,-0.142397},
{-0.004858,4.488087,0.226879},
{-0.012532,4.472383,-0.020163}}};



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
