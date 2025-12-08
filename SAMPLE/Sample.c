
#include "Test.C"

void Sample()
{
	Test *t = new Test();
	Long64_t nent = t->fChain->GetEntries();

	TH1D *h1 = new TH1D("h1_Eg","h1_Eg",2000,0,4000);

	vector<HitClover>::iterator iClover, jClover;
	vector<HitCrystal>::iterator iCrystal, jCrystal;
	vector<SigAna>::iterator iSeg,jSeg, iFV,jFV;

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
					fprintf(stdout,"TS %llu\n",iFV->coarse_time);
				}
			}
		}
	}
	h1->Draw();
}
