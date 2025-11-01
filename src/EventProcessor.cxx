

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
		//fprintf(stdout,"v_sig.size()=%ld\n",v_sig.size());
		timesorter->fmutex_output.unlock();

		Event event = ProcessEvent(v_sig);

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

Event EventProcessor::ProcessEvent(vector<Sig> v_sig)
{
	Event evt;
	evt.Clear();

	vector<SigAna> v_sigana_sort[Ntype][Ndet];
	bool flag_det[Ntype][Ndet]={0};

	vector<Sig>::iterator it_sig;
	for (it_sig=v_sig.begin(); it_sig!=v_sig.end(); it_sig++)
	{
		//it_sig->PrintSummary();
		uint8_t isid = (*it_sig).sid;
		uint8_t ibrd = (*it_sig).brd;
		uint8_t icha = (*it_sig).cha;
		uint8_t itype = map_type  [isid][ibrd][icha]; if (itype==255) continue;
		uint8_t idet  = map_det   [isid][ibrd][icha]; if (idet>Ndet) {fprintf(stderr,"EventProcessor::ProcessEvent(vector<Sig> v_sig): idet %u>Ndet%u\n",idet,Ndet);continue;}
		uint8_t iidx  = map_idx	  [isid][ibrd][icha];

		evt.vSigAna_RAW.emplace_back(itype,idet,iidx,*it_sig);
		v_sigana_sort[itype][idet].emplace_back(itype,idet,iidx,*it_sig);
		flag_det[itype][idet]=1;
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


