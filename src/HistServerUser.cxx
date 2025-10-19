
#include "HistServerUser.h"

//HistServerUser::HistServerUser()
//{
//}
HistServerUser::HistServerUser(string opt_)
	: HistServer(opt_)
{
	fprintf(stdout,"HistServerUser::HistServerUser(%s)\n",opt_.c_str());
}

HistServerUser::~HistServerUser()
{
}

void HistServerUser::InitUser()
{
	InitFile();
	//InitHttp();

///////////// User Area top /////////////////
	for (int isid=0; isid<Nsid; isid++)
	{
		h2_ADC_cha[isid] = MakeH2(
			Form("ADC_sid%02d",isid),
			Form("ADC by channel %02d;cha+%d*brd;ADC",isid,Ncha),
			Nbrd*Ncha, 0, Nbrd*Ncha, 1<<12, 0, double(1<<15)
			);
	}
	h2_Energy_cha = MakeH2(
			"Energy_cha",
			"Energy by channel; channel; Energy",
			Nsid*Nbrd*Ncha, 0,Nsid*Nbrd*Ncha, 1000,0,3000
			);

	for (int iclov=0; iclov<Nclover; iclov++)
	{
		for (int icrys=0;  icrys<Ncrysal; icrys++) for (int iseg=0; iseg<Nseg; iseg++)
		{
			h1_Clover_ADC_seg[iclov][icrys][iseg] = MakeH1(
					Form("Clover/ADC/clov%02d/crys%d",iclov,icrys),
					Form("ADC_clov%02d_crys%d_seg%d",iclov,icrys,iseg),
					Form("ADC_clov%02d_crys%d_seg%d; ADC [A.U]; count",iclov,icrys,iseg),
					1<<10,0.,double(1<<16)
					);
			h1_Clover_Energy_seg[iclov][icrys][iseg] = MakeH1(
					Form("Clover/Energy/clov%02d/crys%d",iclov,icrys),
					Form("Energy_clov%02d_crys%d_seg%d",iclov,icrys,iseg),
					Form("Energy_clov%02d_crys%d_seg%d; Energy [keV]; count",iclov,icrys,iseg),
					1000,0,3000
					);
		}
	}

	for (int ix6=0; ix6<Nx6; ix6++)
	{
		h2_X6_Energy_idx[ix6] = MakeH2(
				Form("X6/det%02d",ix6),
				Form("X6_det%02d_Energy",ix6),
				Form("X6_det%02d_Energy;idx;Energy",ix6),
				Nstrip+Npad,0,Nstrip+Npad, 1024,0,double(1<<16)
				);
		h2_X6_Pos_idx[ix6] = MakeH2(
				Form("X6/det%02d",ix6),
				Form("X6_det%02d_Position",ix6),
				Form("X6_det%02d_Position;istrip;Position",ix6),
				Nstrip,0,Nstrip, 1024,-1.2,1.2
				);
	}



///////////// User Area bottom  /////////////////
}

void HistServerUser::ProcessToHistUser()
{

///////////// User Area top /////////////////
	EvtSimple *evtSimple = &(event.Simple);
	vector<SigAna>::iterator iSig, jSig;
	for (iSig = evtSimple->vSigAna.begin(); iSig != evtSimple->vSigAna.end(); iSig++)
	{
		h2_ADC_cha[iSig->sid]->Fill(iSig->cha + Ncha*iSig->brd, iSig->ADC);
		//h2_ADC_cha->Fill(iSig->cha + Ncha*(iSig->brd + Nbrd*iSig->sid), iSig->ADC);
	}

	EvtASGARD *evtASGARD = &(event.ASGARD);
	vector<HitClover>::iterator iClover, jClover;
	vector<HitCrystal>::iterator iCrystal, jCrystal;
	vector<SigAna>::iterator iSeg,jSeg, iFV,jFV;

	for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
	{
		for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
		{
			for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++)
			{
				///h1_ADC_fv[iClover->idx][iCrystal->idx][iFV->idx]->Fill(iFV->ADC);
				//h2_ADC_cha->Fill(iFV->cha + Ncha*(iFV->brd + Nbrd*iFV->sid), iFV->ADC);
				h2_Energy_cha->Fill(iFV->cha + Ncha*(iFV->brd + Nbrd*iFV->sid), iFV->Energy);
			}
			for (iSeg=iCrystal->vSigAnaSeg.begin(); iSeg!=iCrystal->vSigAnaSeg.end(); iSeg++)
			{
				h1_Clover_ADC_seg[iClover->idx][iCrystal->idx][iSeg->idx]->Fill(iSeg->ADC);
				h1_Clover_Energy_seg[iClover->idx][iCrystal->idx][iSeg->idx]->Fill(iSeg->Energy);
			}
		}

	}
	
	EvtStarkJr *evtStarkJr = &(event.StarkJr);
	vector<HitX6>::iterator iX6, jX6;
	vector<HitStrip>::iterator iStrip, jStrip;
	vector<HitPad>::iterator iPad, jPad;

	for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
	{
		for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++)
		{
			h2_X6_Energy_idx[iX6->idx]->Fill(iStrip->idx, iStrip->Energy);
			h2_X6_Pos_idx[iX6->idx]->Fill(iStrip->idx, iStrip->position);
		}
		for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
		{
			h2_X6_Energy_idx[iX6->idx]->Fill(Nstrip + iPad->idx, iPad->Energy);
		}
	}

	// crystal->r/theta/phi, strip->r/theta/phi



///////////// User Area bottom  /////////////////




}
