
#include "HistServerUser.h"

HistServerUser::HistServerUser()
{
	fprintf(stdout,"HistServerUser::HistServerUser()\n");
}

HistServerUser::~HistServerUser()
{
}

void HistServerUser::InitUser()
{
	InitFile();
	InitHttp();

///////////// User Area top /////////////////

	for (int iclov=0; iclov<Nclover; iclov++)
		for (int icrys=0;  icrys<Ncrysal; icrys++)
			for (int iseg=0; iseg<Nseg; iseg++)
			{
				h1_ADC_seg[iclov][icrys][iseg] = MakeH1(
						Form("ADC_clov%02d_crys%d_seg%d",iclov,icrys,iseg),
						Form("ADC_clov%02d_crys%d_seg%d; ADC [A.U]; count",iclov,icrys,iseg),
						1<<10,0.,double(1<<16) );
				h1_Energy_seg[iclov][icrys][iseg] = MakeH1(
						Form("Energy_clov%02d_crys%d_seg%d",iclov,icrys,iseg),
						Form("Energy_clov%02d_crys%d_seg%d; Energy [keV]; count",iclov,icrys,iseg),
						1000,0,3000 );
			}

	h2_ADC_cha = MakeH2("ADC","ADC by channel; channel; ADC", Nsid*Nbrd*Ncha, 0,Nsid*Nbrd*Ncha, 1<<10,0,double(1<<16));
	h2_Energy_cha = MakeH2("Energy","Energy by channel; channel; Energy", Nsid*Nbrd*Ncha, 0,Nsid*Nbrd*Ncha, 1000,0,3000);
///////////// User Area bottom  /////////////////
}

void HistServerUser::ProcessToHistUser()
{

///////////// User Area top /////////////////
	EvtASGARD *evtASGARD = &(event.ASGARD);
	vector<HitClover>::iterator iClover, jClover;
	vector<HitCrystal>::iterator iCrystal, jCrystal;
	vector<SigAna>::iterator iSeg,jSeg, iFV,jFV;

	for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
	{
	fprintf(stdout,"debug1\n");
		for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
		{
	fprintf(stdout,"debug2\n");
			for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++)
			{
	fprintf(stdout,"debug3\n");
				///h1_ADC_fv[iClover->idx][iCrystal->idx][iFV->idx]->Fill(iFV->ADC);
				h2_ADC_cha->Fill(iFV->cha + 32*(iFV->brd + 10*iFV->sid), iFV->ADC);
				h2_Energy_cha->Fill(iFV->cha + 32*(iFV->brd + 10*iFV->sid), iFV->Energy);
				fprintf(stdout,"%u %u %u : %u\n",iFV->sid, iFV->brd, iFV->cha, iFV->ADC);
			}
			for (iSeg=iCrystal->vSigAnaSeg.begin(); iSeg!=iCrystal->vSigAnaSeg.end(); iSeg++)
			{
				h1_ADC_seg[iClover->idx][iCrystal->idx][iSeg->idx]->Fill(iSeg->ADC);
				h1_Energy_seg[iClover->idx][iCrystal->idx][iSeg->idx]->Fill(iSeg->Energy);
			}
		}

	}
	// for clov for cry for x6 for strip
	// crystal->r/theta/phi, strip->r/theta/phi
	


///////////// User Area bottom  /////////////////




}
