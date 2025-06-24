

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
	nenque++;
}
Sig TimeSorter::Top(uint8_t isid, uint8_t imid, uint8_t icha)
{
	if (q_sig[isid][imid][icha].empty()) 
	{
		fprintf(stderr,"q_sig[isid%u][imid%u][icha%u] is empty, but you tried to call Top()!!\n",isid,imid,icha); exit(-6);
	}
	return q_sig[isid][imid][icha].top();

}
void TimeSorter::Pop(uint8_t sid, uint8_t mid, uint8_t cha);
{
	checker(isid,imid,icha);
	if(q_sig[isid][imid][icha]==0) exit(-5);
	q_sig[isid][imid][icha].pop();
	nsorted--;
}



void TimeSorter::Sort(Sig sig)
{
	isid=sig.sid;
	imid=sig.mid;
	icha=sig.cha;
	checker(isid,imid,icha);
	q_sig[isid][imid][icha].push(sig);
	nsorted++;
}




void TimeSorter::Run()
{
	sorterEnd=0;
	while ()
	{
		fmutex_input.lock();
		if (!q_sig_input.empty)
		{
			Sig sig_tmp = q_sig_input.front();
			q_sig_input.pop();
			fmutex_input.unlock();
			fmutex_output.lock();
			Sort(sig_tmp);
			fmutex_output.unlock();
		}
		else //if ( q_sig_input.empty)
		{
			if ( sorterEnd)
			{
				fmutex_input.unlock();
				break;
			}
			else //if (!sorterEnd)
			{
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
	for (isid=0; isid<Nsid; isid++) for (imid=0; imid<Nmid; imid++) for (icha=0; icha<Ncha; icha++) if (!Empty(isid,imid,icha))
	{
		if (ret > Top(isid,imid,icha).local_gate_time)
		{
			ret = Top(isid,imid,icha).local_gate_time;
		}
	}
	if (ret==-1)
	{
		fprintf(stderr,"TimeSorter::GetMinLGT(): all q is empty\n");
		exit (-11);
	}
	return ret;
}
uint64_t TimeSorter::GetLGT(uint8_t isid, uint8_t imid, uint8_t icha)
{
	checker(isid,imid,icha);
	return Top(isid,imid,icha).local_gate_time;
}

int TimeSorter::FindSigWithLGT(uint64_t lgt)
{
	int ret=0;
	for (isid=0; isid<Nsid; isid++) for (imid=0; imid<Nmid; imid++) for (icha=0; icha<Ncha; icha++) if (!Empty(isid,imid,icha))
	{
		if (Top(isid,imid,icha).local_gate_time - lgt <= timewindow)
		{
			v_sig_coin.push_back(Top(isid,imid,icha));
			Pop(isid,imid,icha);
			ret++;
		}
	}
	return ret;
}

vector<Sig> GetCoinvSig(uint64_t lgt)
{
	v_sig_coin.clear();	
	while (FindSigWithLGT(lgt)>0) {}
	return v_sig_coin;
}




bool TimeSorter::Empty(uint8_t isid, uint8_t imid, uint8_t icha)
{
	checker(isid,imid,icha);
	return q_sig[isid][imid][icha].empty();
}


bool TimeSorter::checker(uint8_t isid, uint8_t imid, uint8_t icha)
{
	if(isid<Nsid && imid<Nmid && icha<Ncha) return true;
	else
    {
        fprintf(stderr,"TimeSorter::checker(uint8_t isid, uint8_t imid, uint8_t icha): (isid%u<Nsid%d && imid%u<Nmid%d && icha%u<Ncha%d)\n", isid,Nsid,imid,Nmid,icha,Ncha);
        exit(-4);
        return false;
    }
    return false;
}






