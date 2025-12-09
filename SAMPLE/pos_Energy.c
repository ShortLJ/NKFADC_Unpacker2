
#include "Test.C"

void pos_Energy()
{
	Test *t = new Test();
	Long64_t nent = t->fChain->GetEntries();

	TH1D *h1 = new TH1D("h1_Eg","h1_Eg",2000,0,4000);

	TH2D *h2_pos_labz = new TH2D("h2_pos_labz","h2_pos_labz;pos;labz", 200,-2,2, 200,-100,100);
	TH2D *h2_pos_Eparti = new TH2D("h2_pos_Eparti","h2_pos_Eparti;pos;Eparticle",200,-2,2, 1000,0,50e3);
	TH2D *h2_labz_Eparti = new TH2D("h2_labz_Eparti","h2_labz_Eparti;labz;Eparticle",200,-200,200, 1000,0,50e3);
	TH2D *h2_labtheta_Eparti = new TH2D("h2_labtheta_Eparti","h2_labtheta_Eparti;labz;Eparticle",200,0,200, 1000,0,50e3);

	vector<HitClover>::iterator iClover, jClover;
	vector<HitCrystal>::iterator iCrystal, jCrystal;
	vector<SigAna>::iterator iSeg,jSeg, iFV,jFV;

	vector<HitX6>::iterator iX6, jX6;
	vector<HitStrip>::iterator iStrip, jStrip;
	vector<HitPad>::iterator iPad, jPad;



	for (Long64_t ient=0; ient<nent; ient++)
	{
		t->GetEntry(ient);
		EvtSimple *evtSimple = t->EvtSimple;
		EvtASGARD *evtASGARD = t->ASGARD;
		EvtStarkJr *evtStarkJr = t->StarkJr;

		for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
		{
			for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
			{
				for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++) if (iFV->idx==0)
				{
					h1->Fill(iFV->Energy);
					//fprintf(stdout,"TS %llu\n",iFV->coarse_time);
				}
			}
		}

		for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
		{
			for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++)
			{
				h2_pos_labz->Fill(iStrip->position, iStrip->hit_coor[2]);
				h2_pos_Eparti->Fill(iStrip->position, iStrip->Energy);
				h2_labz_Eparti->Fill(iStrip->hit_coor[2],iStrip->Energy);
				float (&particle)[3] = iStrip->hit_coor;
				double labtheta = acos(particle[2]/sqrt(particle[0]*particle[0]+particle[1]*particle[1]+particle[2]*particle[2]))*180/3.1415;
				h2_labtheta_Eparti->Fill(labtheta, iStrip->Energy);
			}
		}



	}

	TCanvas *c1 = new TCanvas(); c1->Divide(4,1);
	c1->cd(1); h2_pos_Eparti->Draw();
	c1->cd(2); h2_pos_labz->Draw();
	c1->cd(3); h2_labz_Eparti->Draw();
	c1->cd(4); h2_labtheta_Eparti->Draw();



}
