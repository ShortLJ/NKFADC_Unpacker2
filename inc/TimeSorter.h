
#include <mutex>
#include <queue>

#include "Global.h"
#include "Sig.h"


#ifndef __TIMESORTER__
#define __TIMESORTER__

extern bool enabled [N_SID][N_BRD][N_CHA];
extern uint8_t	map_type	[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_det		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_idx		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};


uint16_t TimeSorter_map[N_SID][N_BRD][N_CHA];

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
		uint64_t GetNSorted(){return nsorted;}
		uint64_t GetNenque(){return nenque;}
		
		mutex fmutex_output;
		void SetTimeWindow(int64_t tw) {timewindow=tw;}
		uint64_t GetMinLGT();
		uint64_t GetLGT(uint8_t sid, uint8_t brd, uint8_t cha);
		vector<Sig> GetCoinvSig(uint64_t ct);


	private:
		queue<Sig> q_sig_input;
		uint16_t pq_size;
		vector<priority_queue<Sig>> vpq_sig;
		//priority_queue<Sig> q_sig[Nsid][Nbrd][Ncha];
		vector<Sig> v_sig_coin;

		void Sort(Sig sig);
		//Sig Top(uint8_t sid, uint8_t brd, uint8_t cha);
		//void Pop(uint8_t sid, uint8_t brd, uint8_t cha);

		bool sorterEnd=0;
		uint64_t nsorted=0;
		uint64_t nenque = 0;
		uint8_t isid; uint8_t ibrd; uint8_t icha;

		int64_t timewindow=0;
		int FindSigWithLGT(uint64_t ct);

		//bool Empty(uint8_t sid, uint8_t brd, uint8_t cha);
		bool checker(uint8_t sid, uint8_t brd, uint8_t cha);

};

#endif // __TIMESORTER__
