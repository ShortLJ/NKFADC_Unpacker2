
#include <mutex>

#include "Global.h"
#include "TimeSorter.h"
//#include "Config.h"

#include "NKSig.h"

#ifndef __DATAGETTER__
#define __DATAGETTER__

using namespace std;

class DataGetter
{
    public:
        DataGetter();
        ~DataGetter();
        void RegisterTimeSorter (TimeSorter *ts){timesorter=ts;}
        //void RegisterConfig (Config *cfg){config=cfg;}
        void Loop();
		void Stop();
        virtual int GetNextData()
		{
			fprintf(stdout,"not implemented yet\n"); 
			return 0;	
		};

        uint32_t sig_processed;
    private:
        uint8_t data[8192];
        uint8_t data_length;

		mutex fmutex;
        int ProcessData();
		bool getterEnd=0;

        TimeSorter *timesorter;
		//Config *config;



};

#endif // __DATAGETTER__
