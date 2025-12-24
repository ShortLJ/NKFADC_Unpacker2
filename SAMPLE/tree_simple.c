
#include "Test.C"

void tree_simple(int run)
{
	TChain *ch = new TChain("tree");
	ch->Add(Form("../Tree_%04d.root",run));
	Test *t = new Test(ch);
	Long64_t nent = t->fChain->GetEntries();

	vector<HitClover>::iterator iClover, jClover;
	vector<HitCrystal>::iterator iCrystal, jCrystal;
	vector<SigAna>::iterator iSeg,jSeg, iFV,jFV;

	vector<HitX6>::iterator iX6, jX6;
	vector<HitStrip>::iterator iStrip, jStrip;
	vector<HitPad>::iterator iPad, jPad;




	TFile *fileout = new TFile(Form("./simple/simple_%04d.root",run),"recreate");

	TH1D *h1_Egamma= new TH1D("h1_Egamma","h1_Egamma",2000,0,4000);
	TH1D *h1_dcEgamma = new TH1D("h1_dcEgamma","h1_dcEgamma",2000,0,4000);
	TH2D *h2_dcEgamma_dcEgamma = new TH2D("h2_dcEgamma_dcEgamma","h2_dcEgamma_dcEgamma", 2000,0,4000, 2000,0,4000);

	TH1D *h1_dcabEgamma = new TH1D("h1_dcabEgamma","h1_dcabEgamma",2000,0,4000);
	TH2D *h2_dcabEgamma_dcabEgamma = new TH2D("h2_dcabEgamma_dcabEgamma","h2_dcabEgamma_dcabEgamma", 2000,0,4000, 2000,0,4000);

	TH2D *h2_pos_labz = new TH2D("h2_pos_labz","h2_pos_labz;pos;labz", 200,-2,2, 200,-100,100);
	TH2D *h2_labz_Eparti = new TH2D("h2_labz_Eparti","h2_labz_Eparti;labz;Eparticle",200,-200,200, 1000,0,200e3);
	TH2D *h2_labtheta_Eparti = new TH2D("h2_labtheta_Eparti","h2_labtheta_Eparti;labz;Eparticle",180,0,180, 1000,0,200e3);
	TH2D *h2_thetapg_dcEgamma = new TH2D("h2_thetapg_dcEgamma","h2_thetapg_dcEgamma", 180,0,180, 2000,0,4000);

	double labtheta;
	double Eparticle;

	vector<float> vdcEgamma;
	vector<float> vdcabEgamma;

	TTree *simpletree = new TTree("simpletree","simpletree");
    simpletree->Branch("labtheta", &labtheta);
    simpletree->Branch("Eparticle", &Eparticle);
    simpletree->Branch("vdcEgamma", &vdcEgamma);
    simpletree->Branch("vdcabEgamma", &vdcabEgamma);



	Long64_t ient=0;
	for (ient=0; ient<nent; ient++)
	{
		if ((ient&0x7ff)==0){ fprintf(stdout,"\rient %lld of %lld, (%.1f%%)",ient,nent,100.*ient/nent); fflush(stdout);}
		t->GetEntry(ient);
		EvtSimple *evtSimple = t->pEvtSimple;
		EvtASGARD *evtASGARD = t->ASGARD;
		EvtStarkJr *evtStarkJr = t->StarkJr;

		for (iX6=evtStarkJr->vHitX6.begin(); iX6!=evtStarkJr->vHitX6.end(); iX6++)
		{
			for (iStrip=iX6->vHitStrip.begin(); iStrip!=iX6->vHitStrip.end(); iStrip++)
			{
				float (&particle)[3] = iStrip->hit_coor; // x,y,z in lab frame
				labtheta = acos(
					particle[2] / sqrt(particle[0]*particle[0]+particle[1]*particle[1]+particle[2]*particle[2])
					)*180/3.1415;
				Eparticle = iStrip->Energy * 5; // gain factor 5 for low gain in KO2426
				double rel_gamma = Eparticle/1000 / (39.96 * 931.494) + 1; // MeV/MeV
				double rel_beta = sqrt(1-1/(rel_gamma*rel_gamma));

				h2_pos_labz->Fill(iStrip->position, particle[2]);
				h2_labz_Eparti->Fill(particle[2],Eparticle);
				h2_labtheta_Eparti->Fill(labtheta, Eparticle);

				vdcEgamma.clear();
				vdcabEgamma.clear();

				for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
				{
					for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
					{
						/* float HitCrystal::DopplerCoorE(beta, direction) */
						float thetapg = iCrystal->GetTheta(particle);
						float dcEnergy = iCrystal->DopplerCorrE(rel_beta, particle);
						h2_thetapg_dcEgamma->Fill(thetapg, dcEnergy);
						h1_dcEgamma->Fill(iCrystal->dcEnergy);
						vdcEgamma.push_back(iCrystal->dcEnergy);
					}
					iClover->DopplerCorrE(rel_beta,particle);
					h1_dcabEgamma->Fill(iClover->dcEnergy);
					vdcabEgamma.push_back(iClover->dcEnergy);
				}
				simpletree->Fill();


				for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
				for (jClover=evtASGARD->vHitClover.begin(); jClover!=evtASGARD->vHitClover.end(); jClover++) if (iClover!=jClover)
				{
					h2_dcabEgamma_dcabEgamma->Fill(iClover->dcEnergy,jClover->dcEnergy);
				}

				for (iClover=evtASGARD->vHitClover.begin(); iClover!=evtASGARD->vHitClover.end(); iClover++)
				for (jClover=evtASGARD->vHitClover.begin(); jClover!=evtASGARD->vHitClover.end(); jClover++)
				for (iCrystal=iClover->vHitCrystal.begin(); iCrystal!=iClover->vHitCrystal.end(); iCrystal++)
				for (jCrystal=jClover->vHitCrystal.begin(); jCrystal!=jClover->vHitCrystal.end(); jCrystal++) if (iCrystal!=jCrystal)
				{
					h2_dcEgamma_dcEgamma->Fill(iCrystal->dcEnergy, jCrystal->dcEnergy);
				}
			}
		}



	}
	fprintf(stdout,"\rient %lld of %lld, (%.1f%%)",ient,nent,100.*ient/nent);
	fprintf(stdout,"\n");
	fflush(stdout);

	fileout->cd();
	simpletree->Write();

	h2_labtheta_Eparti->Write();
	h2_dcEgamma_dcEgamma->Write();
	h2_dcabEgamma_dcabEgamma->Write();
	h2_thetapg_dcEgamma->Write();
	h1_Egamma->Write();
	h1_dcEgamma->Write();
	h1_dcabEgamma->Write();


}
