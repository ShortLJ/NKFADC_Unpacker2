
#include <unistd.h>


#include "TimeSorter.h"


TimeSorter::TimeSorter()
{
}
TimeSorter::~TimeSorter()
{
}

void TimeSorter::Push(Sig sig)
{
	q_sig_input.push(sig);
}
Sig TimeSorter::Top(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	if (q_sig[isid][ibrd][icha].empty()) 
	{
		fprintf(stderr,"q_sig[isid%u][ibrd%u][icha%u] is empty, but you tried to call Top()!!\n",isid,ibrd,icha); exit(-6);
	}
	return q_sig[isid][ibrd][icha].top();

}
void TimeSorter::Pop(uint8_t sid, uint8_t brd, uint8_t cha)
{
	checker(isid,ibrd,icha);
	if(q_sig[isid][ibrd][icha].size()==0) exit(-5);
	q_sig[isid][ibrd][icha].pop();
	nsorted--;
}



void TimeSorter::Sort(Sig sig)
{
	isid=sig.sid;
	ibrd=sig.brd;
	icha=sig.cha;
	if (checker(isid,ibrd,icha))
	{
		q_sig[isid][ibrd][icha].push(sig);
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
			fmutex_input.unlock();
			fmutex_output.lock();
			Sort(sig_tmp);
			fmutex_output.unlock();
		}
		else //if ( q_sig_input.empty())
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
	for (isid=0; isid<Nsid; isid++) for (ibrd=0; ibrd<Nbrd; ibrd++) for (icha=0; icha<Ncha; icha++) if (!Empty(isid,ibrd,icha))
	{
		if (ret > Top(isid,ibrd,icha).coarse_time)
		{
			ret = Top(isid,ibrd,icha).coarse_time;
		}
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
	return Top(isid,ibrd,icha).coarse_time;
}

int TimeSorter::FindSigWithLGT(uint64_t ct)
{
	int ret=0;
	for (isid=0; isid<Nsid; isid++) for (ibrd=0; ibrd<Nbrd; ibrd++) for (icha=0; icha<Ncha; icha++) if (!Empty(isid,ibrd,icha))
	{
		if (Top(isid,ibrd,icha).coarse_time - ct <= timewindow)
		{
			v_sig_coin.push_back(Top(isid,ibrd,icha));
			Pop(isid,ibrd,icha);
			ret++;
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




bool TimeSorter::Empty(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	checker(isid,ibrd,icha);
	return q_sig[isid][ibrd][icha].empty();
}


bool TimeSorter::checker(uint8_t isid, uint8_t ibrd, uint8_t icha)
{
	if(isid<Nsid && ibrd<Nbrd && icha<Ncha) return true;
	else
    {
        fprintf(stderr,"TimeSorter::checker(uint8_t isid, uint8_t ibrd, uint8_t icha): (isid%u<Nsid%d && ibrd%u<Nbrd%d && icha%u<Ncha%d)\n", isid,Nsid,ibrd,Nbrd,icha,Ncha);
        //exit(-4);
        return false;
    }
    return false;
}






