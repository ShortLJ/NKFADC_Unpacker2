
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

		RegisterTimeSorter(TimeSorter *ts){timesorter=ts;}
		RegisterConfig(Config *cfg){config=cfg;}

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


}


#endif //__EVENTPROCESSOR__
