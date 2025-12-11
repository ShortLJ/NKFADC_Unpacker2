
#include "Test.C"

void pos_Energy()
{
	TChain *ch = new TChain("tree");
	ch->Add("Tree_0015.root");
	Test *t = new Test(ch);
	Long64_t nent = t->fChain->GetEntries();

	TH1D *h1_Egamma= new TH1D("h1_Egamma","h1_Egamma",2000,0,4000);
	TH1D *h1_dcEgamma = new TH1D("h1_dcEgamma","h1_dcEgamma",2000,0,4000);
	TH2D *h2_dcEgamma_dcEgamma = new TH2D("h2_dcEgamma_dcEgamma","h2_dcEgamma_dcEgamma", 2000,0,4000, 2000,0,4000);

	TH1D *h1_dcabEgamma = new TH1D("h1_dcabEgamma","h1_dcabEgamma",2000,0,4000);

	TH2D *h2_pos_labz = new TH2D("h2_pos_labz","h2_pos_labz;pos;labz", 200,-2,2, 200,-100,100);
	TH2D *h2_labz_Eparti = new TH2D("h2_labz_Eparti","h2_labz_Eparti;labz;Eparticle",200,-200,200, 1000,0,50e3);
	TH2D *h2_labtheta_Eparti = new TH2D("h2_labtheta_Eparti","h2_labtheta_Eparti;labz;Eparticle",180,0,180, 1000,0,50e3);
	TH2D *h2_thetapg_dcEgamma = new TH2D("h2_thetapg_dcEgamma","h2_thetapg_dcEgamma", 180,0,180, 2000,0,4000);


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
				h1_Egamma->Fill(iCrystal->Energy); // HitCrystal::Energy = Energy of FV0
				/*for (iFV=iCrystal->vSigAnaFV.begin(); iFV!=iCrystal->vSigAnaFV.end(); iFV++) if (iFV->idx==0)
				{
					h1_Egamma->Fill(iFV->Energy);
				}*/

				/* float HitCrystal::DopplerCoorE(float beta, float[3] direction)
					// Corrects doppler shift from HitCrystal::Energy and HitCrystal::seg_coor[3],
					// and stores in HitCrystal::dcEnergy.
					// beta: abs|velocity| of gamma-ray emitter.
					// direction will be normalized in the function. Input doesn't need to unit vector.
				*/
				float beam[3] = {0,0,0.04};
				float thetapg = iCrystal->GetTheta(beam);
				float dcEnergy = iCrystal->DopplerCorrE(beam[2],beam);
				h1_dcEgamma->Fill(iCrystal->dcEnergy); // dcEnergy is a member variable storing last correction.

				float (&gamma)[3] = iCrystal->seg_coor;
				if (iCrystal->GetPrimarySeg()!=0) float primary_seg_energy = iCrystal->GetPrimarySeg()->Energy;
			}
		}
		// now dcEnergy is stored for all crystals.
		for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
			for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
		for (jClover=evtASGARD->vHitClover.begin(); jClover!=evtASGARD->vHitClover.end(); jClover++)
			for (jCrystal=jClover->vHitCrystal.begin(); jCrystal!=jClover->vHitCrystal.end(); jCrystal++)
				if (iCrystal!=jCrystal)
			{
				h2_dcEgamma_dcEgamma->Fill(iCrystal->dcEnergy, jCrystal->dcEnergy);
			}
		// for add-back doppler-correction // soon to be implemented.
		for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
		{
		}

		for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
		{
			for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++)
			{
				float (&particle)[3] = iStrip->hit_coor; // x,y,z in lab frame
				double labtheta = acos(
					particle[2] / sqrt(particle[0]*particle[0]+particle[1]*particle[1]+particle[2]*particle[2])
					)*180/3.1415;
				h2_pos_labz->Fill(iStrip->position, particle[2]);
				h2_labz_Eparti->Fill(particle[2],iStrip->Energy);
				h2_labtheta_Eparti->Fill(labtheta, iStrip->Energy);

				double particleEnergy = iStrip->Energy * 5; // gain factor 5 for low gain in KO2426
				double rel_gamma = particleEnergy/1000 / (39.96 * 931.494) + 1; // MeV/MeV
				double rel_beta = sqrt(1-1/(rel_gamma*rel_gamma));


				for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
				{
					for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
					{
						/* float HitCrystal::DopplerCoorE(beta, direction) */
						float thetapg = iCrystal->GetTheta(particle);
						float dcEnergy = iCrystal->DopplerCorrE(rel_beta, particle);
						h2_thetapg_dcEgamma->Fill(thetapg, dcEnergy);
					}
				}


			}
		}



	}

	TCanvas *c1 = new TCanvas(); c1->Divide(4,1);
	c1->cd(2); h2_pos_labz->Draw();
	c1->cd(3); h2_labz_Eparti->Draw();
	c1->cd(4); h2_labtheta_Eparti->Draw();



}
