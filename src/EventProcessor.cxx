

#include <unistd.h>

#include "EventProcessor.h"


EventProcessor::EventProcessor()
{
}

EventProcessor::~EventProcessor()
{
}

void EventProcessor::Run()
{
	processorEnd=0;
	processorQuit=0;
	while(!processorQuit)
	{
		fmutex.lock();
		if (!processorEnd)
		{
			fmutex.unlock();
			timesorter->fmutex_output.lock();
			if (timesorter->GetNSorted()<NBUFF_SORTER)
			{
				timesorter->fmutex_output.unlock();
				usleep (100000);
				continue;
			}
			//timesorter->fmutex_output.unlock();
		}
		fmutex.unlock();

		//timesorter->fmutex_output.lock();
		if ( timesorter->GetNSorted()==0) 
		{
			timesorter->fmutex_output.unlock();
			break;
		}
		uint64_t ref_lgt = timesorter->GetMinLGT();
		//uint64_t ref_lgt = timesorter->GetLGT(sid,brd,cha);
		vector<Sig> v_sig = timesorter->GetCoinvSig(ref_lgt); /// potential error
		//fprintf(stdout,"ref_lgt %lu size %ld\n",ref_lgt, v_sig.size());
		timesorter->fmutex_output.unlock();

		Event event = ProcessEvent(v_sig,ref_lgt);
		n_eventprocessed++;
		if ((n_eventprocessed & 0x7ff)==0)
		{
			fprintf(stdout,"%ld event processed with %lu \n",n_eventprocessed,ref_lgt);
		}

		if (treewriter)
		{
			treewriter->fmutex.lock();
			treewriter->Enque(&event);
			treewriter->fmutex.unlock();
		}
		if (histserver)
		{
			histserver->fmutex.lock();
			histserver->Enque(&event);
			histserver->fmutex.unlock();
		}
	}
}

void EventProcessor::Stop()
{
	fmutex.lock();
	processorEnd=1;
	fmutex.unlock();
}
void EventProcessor::Quit()
{
	fmutex.lock();
	processorEnd=1;
	processorQuit=1;
	fmutex.unlock();
}

Event EventProcessor::ProcessEvent(vector<Sig> v_sig, uint64_t ref_lgt=0)
{
	Event evt;
	evt.Clear();
	evt.ref_lgt = ref_lgt;

	vector<SigAna> v_sigana_sort[Ntype][Ndet];
	bool flag_det[Ntype][Ndet]={0};

	vector<Sig>::iterator it_sig;
	for (it_sig=v_sig.begin(); it_sig!=v_sig.end(); it_sig++)
	{
		//it_sig->PrintSummary();
		evt.vSigAna_RAW.emplace_back(*it_sig);
		SigAna *this_sigana = &(evt.vSigAna_RAW.back());

		if (this_sigana->type==255) continue;
		if (this_sigana->det>Ndet) {fprintf(stderr,"EventProcessor::ProcessEvent(vector<Sig> v_sig): idet %u>Ndet%u\n",this_sigana->det,Ndet);continue;}
		v_sigana_sort[this_sigana->type][this_sigana->det].push_back(*this_sigana);
		flag_det[this_sigana->type][this_sigana->det]=1;
	}
	////////// Simple //////////
	evt.Simple = EvtSimple(v_sigana_sort[0]);

#if Nclover>=Ndet
	#error "Nclover>=Ndet"
#endif
	////////// ASGARD //////////
	bool flag_clover[Nclover]={0};
	for (uint8_t iclover=0; iclover<Nclover; iclover++)
	{
		bool flag_crystal[Ncrystal]={0};
		for (uint8_t icrystal=0; icrystal<Ncrystal; icrystal++)
		{
			uint8_t idet = iclover * Ncrystal + icrystal;
			if (flag_det[1][idet]||flag_det[2][idet]) 
			{
				flag_clover[iclover]=1;
				flag_crystal[icrystal]=1;
			}
		}
		if (flag_clover[iclover])
		{
			evt.ASGARD.vHitClover.emplace_back(iclover);
			for (uint8_t icrystal=0; icrystal<Ncrystal; icrystal++) if (flag_crystal[icrystal])
			{
				uint8_t idet = iclover * Ncrystal + icrystal;
				evt.ASGARD.vHitClover.back().vHitCrystal.emplace_back(iclover,icrystal,v_sigana_sort[1][idet], v_sigana_sort[2][idet]);
			}
			evt.ASGARD.vHitClover.back().ProcessHit();
		}
	}

#if Nx6>=Ndet
	#error "Nx6>=Ndet"
#endif
	////////// StarkJr //////////
	bool flag_X6[Nx6] = {0};
	for (uint8_t ix6=0; ix6<Nx6; ix6++)
	{
		if (flag_det[3][ix6]||flag_det[4][ix6]||flag_det[5][ix6])
		{
			evt.StarkJr.vHitX6.emplace_back(ix6,v_sigana_sort[3][ix6],v_sigana_sort[4][ix6],v_sigana_sort[5][ix6]);
		}
	}
	return evt;
}


