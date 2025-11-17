
#include "HistServerUser.h"
#include <cmath>

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

void HistServerUser::InitRaw()
{
	h1_timestamp_structure_total = MakeH1(
			"Raw/time_structure",
			"Timestamp_structure_total",
			"Timestamp_structure_total;Timestamp [s];counts in 4 iter / 1 ms",
			1000,0,1
			);
	h1_timestamp_structure_total_avg = MakeH1(
			"Raw/time_structure",
			"Timestamp_structure_total_avg",
			"Timestamp_structure_total_avg;Timestamp [s];counts moving avg / 1 ms * 100",
			1000,0,1
			);

	h2_timestamp_structure_cha = MakeH2(
			"Raw/time_structure",
			"Timestamp_structure_cha",
			Form("Timestamp_structure_cha;Timestamp [s];cha+%d*(brd+%d*sid);counts in 4 iter / 1 ms",Ncha,Nbrd),
			1000,0,1, Ncha*Nbrd*Nsid,0, Ncha*Nbrd*Nsid
			);
	h2_timestamp_structure_cha_avg = MakeH2(
			"Raw/time_structure",
			"Timestamp_structure_cha_avg",
			Form("Timestamp_structure_cha_avg;Timestamp [s];cha+%d*(brd+%d*sid);counts moving avg / 1 ms * 100",Ncha,Nbrd),
			1000,0,1, Ncha*Nbrd*Nsid,0, Ncha*Nbrd*Nsid
			);

	h2_timestamp_structure_Energy = MakeH2(
			"Raw/time_structure",
			"Timestamp_structure_Energy",
			Form("Timestamp_structure_Energy;Timestamp [s];Energy [keV];counts in 4 iter / 1 ms"),
			1000,0,1,  600,0,3000
			);
	h2_timestamp_structure_Energy_avg = MakeH2(
			"Raw/time_structure",
			"Timestamp_structure_Energy_avg",
			Form("Timestamp_structure_Energy_avg;Timestamp [s];Energy [keV];counts moving avg / 1 ms * 100"),
			1000,0,1,  600,0,3000
			);

	h2_timestamp_tn = MakeH2(
			"Raw/time_structure",
			"TrigNumber_Timestamp",
			"TrigNumber_Timestamp;Timestamp [s];Trigger Number",
			1000,0,1, 4000,0,1e6
			);

	for (int isid=0; isid<Nsid; isid++)
	{
		h2_ADC_cha[isid] = MakeH2(
				"Raw/ADC",
				Form("ADC_sid%02d",isid),
				Form("ADC by channel, sid%02d;cha+%d*brd;ADC",isid,Ncha),
				Nbrd*Ncha, 0, Nbrd*Ncha, 1<<12, 0, double(1<<16)
				);
		if (isid<3) h2_Energy_cha[isid] = MakeH2(
				"Raw/Energy",
				Form("Energy_sid%02d",isid),
				Form("Energy by channel, sid%02d;cha+%d*brd;Energy [keV]",isid,Ncha),
				Nbrd*Ncha, 0, Nbrd*Ncha, 2000,0,40000
				);
		if (isid>=3) h2_Energy_cha[isid] = MakeH2(
				"Raw/Energy",
				Form("Energy_sid%02d",isid),
				Form("Energy by channel, sid%02d;cha+%d*brd;Energy [keV]",isid,Ncha),
				Nbrd*Ncha, 0, Nbrd*Ncha, 2000,0,4000
				);
		h2_lgtdiffref_cha[isid] = MakeH2(
				"Raw/Time_Ref",
				Form("Time_Ref_sid%02d",isid),
				Form("TS-TSref by channel, sid%02d;TS-TSref;cha+%d*brd",isid,Ncha),
				1000,-500,500, Nbrd*Ncha, 0, Nbrd*Ncha
				//1000,-4000,4000, Nbrd*Ncha, 0, Nbrd*Ncha
				);
	}
}

void HistServerUser::ProcessToHistRaw()
{
	vector<SigAna>::iterator iSig, jSig;

	for (iSig = event.vSigAna_RAW.begin(); iSig != event.vSigAna_RAW.end(); iSig++)
	{
		if (iSig->coarse_time > TS1)
		{
			if (TS1==0) TS0=event.vSigAna_RAW.begin()->coarse_time;
			else TS0 = TS1;
			TS1 = TS0 + 1000000000;
			tn0 = event.vSigAna_RAW.begin()->trigger_number;
			for (int ibin=1; ibin<=h1_timestamp_structure_total_avg->GetXaxis()->GetNbins(); ibin++)
			{
				h1_timestamp_structure_total_avg->SetBinContent(ibin, (
							h1_timestamp_structure_total_avg->GetBinContent(ibin)*3
							+h1_timestamp_structure_total->GetBinContent(ibin)*1*100
							)/4);
			}
			for (int ibin=1; ibin<=h2_timestamp_structure_cha_avg->GetXaxis()->GetNbins(); ibin++)
			for (int jbin=1; jbin<=h2_timestamp_structure_cha_avg->GetYaxis()->GetNbins(); jbin++)
			{
				h2_timestamp_structure_cha_avg->SetBinContent(ibin,jbin, (
							h2_timestamp_structure_cha_avg->GetBinContent(ibin,jbin)*3
							+ h2_timestamp_structure_cha->GetBinContent(ibin,jbin)*1*100
							)/4);
			}
			for (int ibin=1; ibin<=h2_timestamp_structure_Energy_avg->GetXaxis()->GetNbins(); ibin++)
			for (int jbin=1; jbin<=h2_timestamp_structure_Energy_avg->GetYaxis()->GetNbins(); jbin++)
			{
				h2_timestamp_structure_Energy_avg->SetBinContent(ibin,jbin, (
							h2_timestamp_structure_Energy_avg->GetBinContent(ibin,jbin)*9
							+ h2_timestamp_structure_Energy->GetBinContent(ibin,jbin)*1*100
							)/10);
			}

			h1_timestamp_structure_total->Reset();
			h2_timestamp_structure_cha->Reset();
			h2_timestamp_structure_Energy->Reset();
			h2_timestamp_tn->Reset();
		}
		double ts = ((iSig->coarse_time-TS0)%1000000000)/1000000000.;
		h2_timestamp_tn->Fill(ts,iSig->trigger_number-tn0);
		h1_timestamp_structure_total->Fill(ts);
		h2_timestamp_structure_cha->Fill(ts,iSig->cha+Ncha*(iSig->brd+Nbrd*iSig->sid));
		if (iSig->Energy > 20) h2_timestamp_structure_Energy->Fill(ts,iSig->Energy);
	}
	for (iSig = event.vSigAna_RAW.begin(); iSig != event.vSigAna_RAW.end(); iSig++)
	{
		h2_ADC_cha[iSig->sid]->Fill(iSig->cha + Ncha*iSig->brd, iSig->ADC);
		h2_Energy_cha[iSig->sid]->Fill(iSig->cha + Ncha*iSig->brd, iSig->Energy);
		int64_t ts_tsref = iSig->coarse_time-event.ref_lgt;
		h2_lgtdiffref_cha[iSig->sid]->Fill(ts_tsref,iSig->cha + Ncha*iSig->brd);
	}
}


void HistServerUser::InitUser()
{
	h2_FVEnergy_cha[0] = MakeH2(
			"Clover",
			"FV_Energy_cha_1",
			Form("FV Energy_1; icrystal+%d*iclover; Energy [keV]",Ncrystal),
			Nclover*Ncrystal, 0,Nclover*Ncrystal, 4000,0,4000
			);
	h2_FVEnergy_cha[1] = MakeH2(
			"Clover",
			"FV_Energy_cha_2",
			Form("FV Energy_2; icrystal+%d*iclover; Energy [keV]",Ncrystal),
			Nclover*Ncrystal, 0,Nclover*Ncrystal, 4000,0,4000
			);

	h2_Eg_Eg = MakeH2(
			"Clover",
			"Eg_Eg",
			"g-g coincidence;Energy;Energy",
			1500,0,3000, 1500,0,3000
			);

	for (int iclov=0; iclov<Nclover; iclov++)
	{
		for (int icrys=0;  icrys<Ncrystal; icrys++)
		{
			for (int iseg=0; iseg<Nseg; iseg++)
			{
				h1_Clover_Energy_seg[iclov][icrys][iseg] = MakeH1(
						Form("Clover/Energy/clov%02d/crys%d",iclov,icrys),
						Form("Energy_clov%02d_crys%d_seg%d",iclov,icrys,iseg),
						Form("Energy_clov%02d_crys%d_seg%d; Energy [keV]; count",iclov,icrys,iseg),
						1000,0,4000
						);
			}
			for (int ifv=0; ifv<Nfv; ifv++)
			{
				h1_Clover_Energy_fv[iclov][icrys][ifv] = MakeH1(
						Form("Clover/Energy/clov%02d/crys%d",iclov,icrys),
						Form("Energy_clov%02d_crys%d_fv%d",iclov,icrys,ifv),
						Form("Energy_clov%02d_crys%d_fv%d; Energy [keV]; count",iclov,icrys,ifv),
						1000,0,4000
						);
			}
		}
	}
	h1_Clover_Energy_fv_all = MakeH1(
			"Clover",
			Form("h1_Clover_Energy_fv_all"),
			Form("Energy_clov_FV_all; Energy [keV]; count"),
			1500,0,3000
			);
	h1_Clover_Energy_fv_all_forward = MakeH1(
			"Clover",
			Form("h1_Clover_Energy_fv_all_forward"),
			Form("Energy_clov_FV_all; Energy [keV]; count"),
			1500,0,3000
			);
	h1_Clover_dcEnergy_fv_all_forward = MakeH1(
			"Clover",
			Form("h1_Clover_dcEnergy_fv_all_forward"),
			Form("dcEnergy_clov_FV_all; dcEnergy [keV]; count"),
			1500,0,3000
			);
	h1_Clover_dcEnergy_fv_all_forward_proton = MakeH1(
			"Clover",
			Form("h1_Clover_dcEnergy_fv_all_forward_proton"),
			Form("dcEnergy_clov_FV_all; dcEnergy [keV]; count"),
			1500,0,3000
			);

	h1_Clover_Energy_fv_all_backward = MakeH1(
			"Clover",
			Form("h1_Clover_Energy_fv_all_backward"),
			Form("Energy_clov_FV_all; Energy [keV]; count"),
			1500,0,3000
			);
	h1_Clover_dcEnergy_fv_all_backward = MakeH1(
			"Clover",
			Form("h1_Clover_dcEnergy_fv_all_backward"),
			Form("dcEnergy_clov_FV_all; dcEnergy [keV]; count"),
			1500,0,3000
			);


	h2_Time_Energy_fv_hemi = MakeH2(
			"Clover",
			Form("h2_Time_Energy_fv_hemi"),
			Form("Time_Energy_fv_hemi;Time [s];Energy [keV]; count"),
			300,0,300,3000,0,3000
			);





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
		for (int ipad=0; ipad<Npad; ipad++)
		{
			h2_X6_Pos_idx_byPad[ix6][ipad] = MakeH2(
					Form("X6/det%02d",ix6),
					Form("X6_det%02d_Position_pad%d",ix6,ipad),
					Form("X6_det%02d_Position_pad%d;istrip;Position",ix6,ipad),
					Nstrip,0,Nstrip, 1024,-1.2,1.2
					);

		}
		for (int istrip=0; istrip<Nstrip; istrip++)
		{
			h2_X6_ADC_ADC[ix6][istrip] = MakeH2(
					Form("X6/det%02d/strip%d",ix6,istrip),
					Form("X6_det%02d_strip%d_ADC_ADC",ix6,istrip),
					Form("X6_det%02d_strip%d_ADC_ADC;ADC U;ADC D",ix6,istrip),
					1024,0,65536, 1024,0,65536
					);
			h2_X6_pos_ADCpad_all[ix6][istrip] = MakeH2(
					Form("X6/det%02d/strip%d",ix6,istrip),
					Form("X6_det%02d_posstrip%d_Energypad",ix6,istrip),
					Form("X6_det%02d_posstrip%d_Energypad;pos strip;Energy pad",ix6,istrip),
					120,-1.2,1.2, 600,0,30e3);




			for (int ipad=0; ipad<Npad; ipad++)
			{
				h2_X6_ADC_ADC_byPad[ix6][istrip][ipad] = MakeH2(
						Form("X6/det%02d/strip%d/ADC_ADC",ix6,istrip),
						Form("X6_det%02d_strip%d_ADC_ADC_pad%d",ix6,istrip,ipad),
						Form("X6_det%02d_strip%d_ADC_ADC_pad%d;ADC U;ADC D",ix6,istrip,ipad),
					1024,0,65536, 1024,0,65536
						);
				h2_X6_ADCstrip_ADCpad[ix6][istrip][ipad] = MakeH2(
						Form("X6/det%02d/strip%d/ADCstrip_ADCpad",ix6,istrip),
						Form("X6_det%02d_ADCstrip%d_ADCpad%d",ix6,istrip,ipad),
						Form("X6_det%02d_ADCstrip%d_ADCpad%d;ADC strip;ADC pad",ix6,istrip,ipad),
					1024,0,65536, 1024,0,65536
						);
				h2_X6_pos_ADCpad[ix6][istrip][ipad] = MakeH2(
						Form("X6/det%02d/strip%d/pos_ADCpad",ix6,istrip),
						Form("X6_det%02d_posstrip%d_Energypad%d",ix6,istrip,ipad),
						Form("X6_det%02d_posstrip%d_Energypad%d;pos strip;Energy pad",ix6,istrip,ipad),
						120,-1.2,1.2, 600,0,30e3);

			}
		}
	}

	h2_X6_theta_Energy_backward = MakeH2(
		"asdf",
		"h2_X6_theta_Energy_backward",
		"h2_X6_theta_Energy_backward;theta;pad energy",
		180,0,180, 1000,0,60e3
		);
	h2_X6_theta_Energy_forward = MakeH2(
		"asdf",
		"h2_X6_theta_Energy_forward",
		"h2_X6_theta_Energy_forward;theta;pad energy",
		180,0,180, 1000,0,160e3
		);

	for (int i=0; i<6;i++)
	{
		h2_X6_BB10[i] = MakeH2(
			"asdf",
			Form("h2_X6_BB10_%d",i),
			Form("h2_X6_BB10_%d;X6;BB10",i),
			1<< 8,0,double(1<<16),
			1<< 8,0,double(1<<16)
			);
	}
	h2_X6_BB10_all = MakeH2(
			"asdf",
			Form("h2_X6_BB10_all"),
			Form("h2_X6_BB10_all;X6;BB10"),
			1<< 8,0,double(1<<16),
			1<< 8,0,double(1<<16)
			);


///////////// User Area bottom  /////////////////
}
void HistServerUser::ProcessToHistUser()
{
	EvtSimple *evtSimple = &(event.Simple);
	vector<SigAna>::iterator iSig, jSig;

	/*for (iSig = evtSimple->vSigAna.begin(); iSig != evtSimple->vSigAna.end(); iSig++)
	{
		if (iSig->cha==8 || iSig->cha==9)
		{
			h2_FVEnergy_cha->Fill((iSig->cha-8) + 2*(iSig->brd + Nbrd*iSig->sid), iSig->Energy);
			h1_Clover_Energy_fv_all->Fill(iSig->Energy);
		}
	}*/

	for (iSig = evtSimple->vSigAna.begin(); iSig != evtSimple->vSigAna.end(); iSig++) if (iSig->cha==8)
		for (jSig = evtSimple->vSigAna.begin(); jSig != evtSimple->vSigAna.end(); jSig++) if (jSig->cha==8) if (iSig!=jSig)
		{
			h2_Eg_Eg->Fill(iSig->Energy, jSig->Energy);
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
				//h1_ADC_fv[iClover->idx][iCrystal->idx][iFV->idx]->Fill(iFV->ADC);
				h2_FVEnergy_cha[iFV->idx]->Fill(iCrystal->idx + Ncrystal*iClover->idx, iFV->Energy);
				h1_Clover_Energy_fv[iClover->idx][iCrystal->idx][iFV->idx]->Fill(iFV->Energy);
				if (iFV->idx==0)
				{
					h1_Clover_Energy_fv_all->Fill(iFV->Energy);
					if (iClover->idx>=5)
					{
						if (time0==0) time0 = iFV->coarse_time;
						float time = float(iFV->coarse_time - time0)/1'000'000'000;
						h2_Time_Energy_fv_hemi->Fill(time,iFV->Energy);
					}
				}
			}
			for (iSeg=iCrystal->vSigAnaSeg.begin(); iSeg!=iCrystal->vSigAnaSeg.end(); iSeg++)
			{
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
			for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
			{
				h2_X6_Pos_idx_byPad[iX6->idx][iPad->idx]->Fill(iStrip->idx, iStrip->position);
			}
			h2_X6_ADC_ADC[iX6->idx][iStrip->idx]->Fill(iStrip->sigStripU.ADC,iStrip->sigStripD.ADC);
			for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
			{
				h2_X6_ADC_ADC_byPad[iX6->idx][iStrip->idx][iPad->idx]->Fill(iStrip->sigStripU.ADC,iStrip->sigStripD.ADC);
				h2_X6_ADCstrip_ADCpad[iX6->idx][iStrip->idx][iPad->idx]->Fill(iStrip->sigStripU.ADC+iStrip->sigStripD.ADC, iPad->sigPad.ADC);
				h2_X6_pos_ADCpad[iX6->idx][iStrip->idx][iPad->idx]->Fill(iStrip->position, iPad->Energy);
				h2_X6_pos_ADCpad_all[iX6->idx][iStrip->idx]->Fill(iStrip->position, iPad->Energy);
			}
		}
		for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
		{
			h2_X6_Energy_idx[iX6->idx]->Fill(Nstrip + iPad->idx, iPad->Energy);
		}
	}
	bool flag_proton=0;
	for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
	{
		for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++)
		{
			float coor[3] = {
				strip_pos_cart[iX6->idx][iStrip->idx][0],
				strip_pos_cart[iX6->idx][iStrip->idx][1],
				strip_pos_cart[iX6->idx][iStrip->idx][2] 
			};
			if (iX6->idx<6) coor[2] -= iStrip->position * 75/2;
			if (iX6->idx>=6) coor[2] += iStrip->position  * 75/2;
			double cosi = coor[2]/sqrt(coor[0]*coor[0]+coor[1]*coor[1]+coor[2]*coor[2]);
			double theta_deg = acos(cosi)/3.1415*180;
			if (iX6->idx>=6) if (theta_deg/48.7+iStrip->Energy/35000<1) flag_proton=1;
			//for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
			{
				if (iX6->idx<6)
				{
					//h2_X6_theta_Energy_backward->Fill(theta_deg, iPad->Energy);
					h2_X6_theta_Energy_backward->Fill(theta_deg, iStrip->Energy);
				}
				if (iX6->idx>=6)
				{
					//h2_X6_theta_Energy_forward->Fill(theta_deg, iPad->Energy);
					h2_X6_theta_Energy_forward->Fill(theta_deg, iStrip->Energy);
				}
			}

		}
	}

	for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++) if (iX6->idx>=6)
	{
		for (iSig = evtSimple->vSigAna.begin(); iSig != evtSimple->vSigAna.end(); iSig++) if (iX6->idx-6 == iSig->det)
		{
			for (iPad=iX6->vHitPad.begin(); iPad!=iX6->vHitPad.end(); iPad++)
			{
				h2_X6_BB10[iSig->det]->Fill(iPad->Energy, iSig->ADC);
				h2_X6_BB10_all->Fill(iPad->Energy, iSig->ADC);
			}

		}
	}




	bool flag_forward=0;
	bool flag_backward=0;
	for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
			for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++) if (iStrip->Energy>1800)
	{
		if (iX6->idx<6) flag_backward=1;
		if (iX6->idx>=6) flag_forward=1;
	}
	if (flag_backward)
	{
		for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
		{
			for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
			{
				for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++) if (iFV->idx==0)
				{
					h1_Clover_Energy_fv_all_backward->Fill(iFV->Energy);
				}
				h1_Clover_dcEnergy_fv_all_backward->Fill(iCrystal->dcEnergy);
			}
		}
	}
	if (flag_forward)
	{
		for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
		{
			for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
			{
				for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++) if (iFV->idx==0)
				{
					h1_Clover_Energy_fv_all_forward->Fill(iFV->Energy);
				}
				h1_Clover_dcEnergy_fv_all_forward->Fill(iCrystal->dcEnergy);
				if (flag_proton) h1_Clover_dcEnergy_fv_all_forward_proton->Fill(iCrystal->dcEnergy);
			}
		}
	}





}
