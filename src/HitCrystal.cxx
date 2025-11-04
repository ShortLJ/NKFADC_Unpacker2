
#include <stdlib.h>
#include <cmath>

#include "HitCrystal.h"


HitCrystal::HitCrystal()
{
}
HitCrystal::HitCrystal(uint8_t idx_, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg)
	: Hit(idx_), vSigAnaFV(v_sigana_fv), vSigAnaSeg(v_sigana_seg), primary(-1)
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
HitCrystal::HitCrystal(uint8_t iclover, uint8_t icrystal, vector<SigAna> &v_sigana_fv, vector<SigAna> &v_sigana_seg)
	: cloverID(iclover), Hit(icrystal), vSigAnaFV(v_sigana_fv), vSigAnaSeg(v_sigana_seg),primary(-1)
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
	Energy = E_fv_avg;
	Process_Primary();
	DopplerCorrE(0.1,0,0);
}

float HitCrystal::DopplerCorrE(double beta_r, double beta_theta, double beta_phi)
{
	double pos_det[3] = {
		seg_pos_cart [cloverID][idx][primary][0],
		seg_pos_cart [cloverID][idx][primary][1],
		seg_pos_cart [cloverID][idx][primary][2]
	};
	double mom_nucl[3] = {
		beta_r * sin(beta_theta) * cos(beta_phi),
		beta_r * sin(beta_theta) * sin(beta_phi),
		beta_r * cos(beta_theta)
	};
	//Energy = Energy / (1-beta*cos(theta/*rad*/)) * sqrt(1-beta*beta);
	//Energy = Energy / (1-inner_product(vec_c, vec_beta)/abs(vec_c)) * sqrt(1-beta*beta);
	double inner_product = 
			(pos_det[0]*mom_nucl[0] + pos_det[1]*mom_nucl[1] + pos_det[2]*mom_nucl[2]) 
		/ sqrt(pos_det[0]*pos_det[0] + pos_det[1]*pos_det[1] + pos_det[2]*pos_det[2]);
	dcEnergy = Energy / (1-inner_product) * sqrt(1-beta_r*beta_r);

	return dcEnergy;
}

bool HitCrystal::isValid()
{
	if (abs(E_seg_sum-E_fv_avg)>100) return false;
	return true;
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
	int nhit=0;
	vector<SigAna>::iterator fv;
	for (fv=vSigAnaFV.begin(); fv!=vSigAnaFV.end(); fv++)
	{
		E_fv_avg+=fv->Energy;
		nhit++;
	}
	if (nhit) E_fv_avg /= nhit;
	else E_fv_avg = -1;
}

void HitCrystal::Process_Primary()
{
	primary = -1;
	if (vSigAnaSeg.size()==0)
	{
		fprintf(stderr,"void HitCrystal::Process_Primary(): vSigAnaSeg.size()==0\n");
		return;
	}
	vector<SigAna>::iterator iseg,pseg;
	for (iseg=vSigAnaSeg.begin(); iseg!=vSigAnaSeg.end(); iseg++)
	{
		if (iseg==vSigAnaSeg.begin()) pseg = iseg;
		else pseg = (pseg->Energy > iseg->Energy) ? pseg : iseg;
	}
	primary = pseg->idx;
}
