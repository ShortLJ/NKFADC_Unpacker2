
#include <mutex>

//#include "Global.h"
#include "TimeSorter.h"
//#include "Config.h"

#include "Sig.h"

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

	protected:
        virtual uint8_t *GetNextPacket() = 0;
		//{
		//	fprintf(stdout,"GetNextPacket(): ");
		//	fprintf(stdout,"not implemented yet. 1 second sleep\n"); 
		//	packet_size=0;
		//	return 0;	// packet size?
		//}
		virtual Sig Interpret(uint8_t *&tmp) = 0;
		//{
		//	fprintf(stderr, "DataGetter::Interpret(uint8_t *&tmp) is virtual only.\n");
		//	uint8_t data_length = tmp[0] & 0xFF;
		//	Sig ret(tmp);
		//	tmp += data_length;
		//	return ret;
		//}
		int packet_size; // byte - uint8_t 

		mutex fmutex;
		bool getterEnd=0;
        uint32_t sig_processed;

    private:
		//uint8_t data[8192];
		//uint8_t data_length;

		int ProcessPacket(uint8_t *packet);


        TimeSorter *timesorter;
		//Config *config;



};

#endif // __DATAGETTER__
