
#include <mutex>
#include <queue>

#include "Global.h"
#include "Sig.h"


#ifndef __TIMESORTER__
#define __TIMESORTER__

using namespace std;

class TimeSorter
{
    public:
        TimeSorter();
        ~TimeSorter();

		void Run();
		void Stop();

		mutex fmutex_input;
		void Push(Sig sig);
		
		mutex fmutex_output;
		void SetTimeWindow(int64_t tw) {timewindow=tw;}
		uint64_t GetMinLGT();
		uint64_t GetLGT(uint8_t sid, uint8_t brd, uint8_t cha);
		vector<Sig> GetCoinvSig(uint64_t ct);


	private:
		queue<Sig> q_sig_input;
		priority_queue<Sig> q_sig[Nsid][Nbrd][Ncha];
		vector<Sig> v_sig_coin;

		void Sort(Sig sig);
		Sig Top(uint8_t sid, uint8_t brd, uint8_t cha);
		void Pop(uint8_t sid, uint8_t brd, uint8_t cha);

		bool sorterEnd=0;
		uint64_t nenque=0;
		uint64_t nsorted=0;
		uint8_t isid; uint8_t ibrd; uint8_t icha;

		int64_t timewindow=0;
		int FindSigWithLGT(uint64_t ct);

		bool Empty(uint8_t sid, uint8_t brd, uint8_t cha);
		bool checker(uint8_t sid, uint8_t brd, uint8_t cha);

};

#endif // __TIMESORTER__
