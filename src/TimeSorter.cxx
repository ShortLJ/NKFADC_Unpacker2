
#include <unistd.h>
#include <cstdlib>


#include "TimeSorter.h"


TimeSorter::TimeSorter()
{
	pq_size=0;
	for (uint8_t isid=0; isid<N_SID; isid++)
	for (uint8_t ibrd=0; ibrd<N_BRD; ibrd++)
	for (uint8_t icha=0; icha<N_CHA; icha++)
	{
		if (enabled[isid][ibrd][icha]) 
		{
			TimeSorter_map[isid][ibrd][icha] = pq_size++;
			vpq_sig.emplace_back();
			if (vpq_sig.size()!=pq_size)
			{
				fprintf(stderr,"DEBUG function. vq.sig.size()!=pq_size\n");
				exit(-71);
			}
		}
		else
		{
			TimeSorter_map[isid][ibrd][icha]=-1;
		}
	}
	for (uint8_t isid=0; isid<N_SID; isid++)
	for (uint8_t ibrd=0; ibrd<N_BRD; ibrd++)
	for (uint8_t icha=0; icha<N_CHA; icha++)
	{
		if (participate_ref[isid][ibrd][icha])
		{
			vpqptr_sig_refs.push_back(&(vpq_sig.at(TimeSorter_map[isid][ibrd][icha])));
			fprintf(stdout,"TimeSorter: ref parti: %u %u %u, %u\n",isid,ibrd,icha,TimeSorter_map[isid][ibrd][icha]);
		}
	}
}
TimeSorter::~TimeSorter()
{
}

void TimeSorter::Push(Sig sig)
{
	q_sig_input.push(sig);
	nenque++;
}
/*Sig TimeSorter::Top(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	if (vpq_sig[TimeSorter_map[isid][ibrd][icha]].empty()) 
	{
		fprintf(stderr,"q_sig[isid%u][ibrd%u][icha%u] is empty, but you tried to call Top()!!\n",isid,ibrd,icha); exit(-6);
	}
	return vpq_sig[TimeSorter_map[isid][ibrd][icha]].top();

}*/
/*void TimeSorter::Pop(uint8_t sid, uint8_t brd, uint8_t cha)
{
	checker(isid,ibrd,icha);
	if(q_sig[isid][ibrd][icha].size()==0) exit(-5);
	q_sig[isid][ibrd][icha].pop();
	nsorted--;
}*/



void TimeSorter::Sort(Sig sig)
{
	isid=sig.sid;
	ibrd=sig.brd;
	icha=sig.cha;
	if (checker(isid,ibrd,icha))
	{
		vpq_sig[TimeSorter_map[isid][ibrd][icha]].push(sig);
		nsorted++;
	}
}




void TimeSorter::Run()
{
	sorterEnd=0;
	while (1)
	{
		fmutex_input.lock();
		if (!q_sig_input.empty())
		{
			//fprintf(stdout,"if (!q_sig_input.empty())\n");
			Sig sig_tmp = q_sig_input.front();
			q_sig_input.pop();
			nenque--;
			fmutex_input.unlock();
			fmutex_output.lock();
			Sort(sig_tmp);
			fmutex_output.unlock();
		}
		if ( q_sig_input.empty() || sorterEnd)
		{
			//fprintf(stdout,"else //if ( q_sig_input.empty())\n");
			if ( sorterEnd)
			{
				//fprintf(stdout,"if ( sorterEnd)\n");
				fmutex_input.unlock();
				break;
			}
			else //if (!sorterEnd)
			{
				//fprintf(stdout,"else //if (!sorterEnd)\n");
				fmutex_input.unlock();
				usleep(100000);
			}
		}
	}
}

void TimeSorter::Stop()
{
	fmutex_input.lock();
	sorterEnd=1;
	fmutex_input.unlock();
}



uint64_t TimeSorter::GetMinLGT()
{
	uint64_t ret=-1; /// uint64_t max
	vector<priority_queue<Sig>*>::iterator ipq;
	for (ipq=vpqptr_sig_refs.begin(); ipq!=vpqptr_sig_refs.end(); ipq++) if (!(*ipq)->empty())
	{
		ret = ret > (*ipq)->top().coarse_time ? (*ipq)->top().coarse_time : ret;
	}
	if (ret==-1)
	{
		fprintf(stderr,"TimeSorter::GetMinLGT(): all q is empty\n");
		//exit (-11);
	}
	return ret;
}
uint64_t TimeSorter::GetLGT(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	checker(isid,ibrd,icha);
	return vpq_sig[TimeSorter_map[isid][ibrd][icha]].top().coarse_time;
}

int TimeSorter::FindSigWithLGT(uint64_t ct)
{
	int ret=0;
	vector<priority_queue<Sig>>::iterator ipq_sig;
	for (ipq_sig=vpq_sig.begin(); ipq_sig!=vpq_sig.end(); ipq_sig++) if (!ipq_sig->empty())
	{
		while (!ipq_sig->empty())
		{
			if (ipq_sig->top().coarse_time + timewindow <= ct)
			{
				//fprintf(stdout,"TimeSorter: abandon %ld from %lu\n",int64_t(ipq_sig->top().coarse_time-ct),ct);
				ipq_sig->pop();
				nsorted--;
				continue;
			}
			if (ipq_sig->top().coarse_time <= ct + timewindow)
			{
				//fprintf(stdout,"TimeSorter: get %ld from %lu\n",int64_t(ct-ipq_sig->top().coarse_time),ct);
				v_sig_coin.push_back(ipq_sig->top());
				ipq_sig->pop();
				nsorted--;
				ret++;
			}
			break;
		}
	}
	return ret;
}

vector<Sig> TimeSorter::GetCoinvSig(uint64_t ct)
{
	v_sig_coin.clear();	
	while (FindSigWithLGT(ct)>0) {}
	return v_sig_coin;
}




/*bool TimeSorter::Empty(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	checker(isid,ibrd,icha);
	return vpq_sig[TimeSorter_map[isid][ibrd][icha]].empty();
}*/


bool TimeSorter::checker(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	if(isid<Nsid && ibrd<Nbrd && icha<Ncha) if (enabled[isid][ibrd][icha])
		return true;
	else
    {
        fprintf(stderr,"TimeSorter::checker(uint8_t isid, uint8_t ibrd, uint8_t icha): (isid%u<Nsid%d && ibrd%u<Nbrd%d && icha%u<Ncha%d)\n", isid,Nsid,ibrd,Nbrd,icha,Ncha);
        //exit(-4);
        return false;
    }
    return false;
}






