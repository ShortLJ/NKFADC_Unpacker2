
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
	while(1)
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

Event EventProcessor::ProcessEvent(vector<Sig> v_sig)
{
	Event evt;
	evt.Clear();

	vector<SigAna> v_sigana_sort[Ntype][Ndet];

	vector<Sig>::iterator it_sig;
	for (it_sig=v_sig.begin(); it_sig!=v_sig.end(); it_sig++)
	{
		//it_sig->PrintSummary();
		uint8_t isid = (*it_sig).sid;
		uint8_t ibrd = (*it_sig).brd;
		uint8_t icha = (*it_sig).cha;
		uint8_t itype = map_type  [isid][ibrd][icha]; if (itype==255) continue;
		uint8_t idet  = map_det   [isid][ibrd][icha];
		uint8_t iidx  = map_idx	  [isid][ibrd][icha];

		v_sigana_sort[itype][idet].push_back(SigAna(itype,idet,iidx,*it_sig));
	}
	////////// ASGARD //////////
	vector<HitCrystal> v_hit_crystal;
	for (uint8_t idet=0; idet<Ndet; idet++)
	{
		if (v_sigana_sort[0][idet].size()+v_sigana_sort[1][idet].size()>0)
		{
			uint8_t iclover = idet >> 2; 
			uint8_t icrystal = idet & 0x3;
			v_hit_crystal.push_back(HitCrystal(iclover,icrystal,v_sigana_sort[0][idet],v_sigana_sort[1][idet]));
		}
	}
	evt.ASGARD=EvtASGARD(v_hit_crystal);
	////////// StarkJr //////////
	vector<HitX6> v_hit_x6;
	for (uint8_t idet=0; idet<Ndet; idet++)
	{
		if (v_sigana_sort[2][idet].size()+v_sigana_sort[3][idet].size()+v_sigana_sort[4][idet].size()>0)
		{
			v_hit_x6.push_back(HitX6(idet,v_sigana_sort[2][idet],v_sigana_sort[3][idet],v_sigana_sort[4][idet]));
		}
	}
	evt.StarkJr=EvtStarkJr(v_hit_x6);
	return evt;
}


