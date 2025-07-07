
#include "TimeSorter.h"
#include "Config.h"
#include "TreeWriter.h"
#include "HistServer.h"

#include "Sig.h"
#include "Event.h"

#ifndef __EVENTPROCESSOR__
#define __EVENTPROCESSOR__

using namespace std;

class EventProcessor
{
	public:
		EventProcessor();
		~EventProcessor();

		void RegisterTimeSorter(TimeSorter *ts){timesorter=ts;}
		void RegisterConfig(Config *cfg){config=cfg;}

		void RegisterHistServer(HistServer *hs){histserver=hs;}
		void RegisterTreeWriter(TreeWriter *tw){treewriter=tw;}

		mutex fmutex;
		void Run();
		void Stop();

	private:
		TimeSorter *timesorter;
		Config *config;
		TreeWriter *treewriter;
		HistServer *histserver;

		Event ProcessEvent(vector<Sig> v_sig);

		bool processorEnd;


};


#endif //__EVENTPROCESSOR__
