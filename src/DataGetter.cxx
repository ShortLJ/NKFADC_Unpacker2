
//#include <iostream>
#include <unistd.h>

#include "DataGetter.h"

void DataGetter::print_binary(uint8_t *&tmp, long N)
{
       uint8_t *tt = tmp;
       uint8_t *t = tmp;
       while (t-tt<N)
       {
               fprintf(stdout,
               "%02x %02x %02x %02x %02x %02x %02x %02x (%04ld) %02x %02x %02x %02x %02x %02x %02x %02x (%04ld) %02x %02x %02x %02x %02x %02x %02x %02x (%04ld) %02x %02x %02x %02x %02x %02x %02x %02x (%04ld)\n",
               *t++,*t++,*t++,*t++,*t++,*t++,*t++,*t++,t-tt,
               *t++,*t++,*t++,*t++,*t++,*t++,*t++,*t++,t-tt,
               *t++,*t++,*t++,*t++,*t++,*t++,*t++,*t++,t-tt,
               *t++,*t++,*t++,*t++,*t++,*t++,*t++,*t++,t-tt
               );
       }
}


DataGetter::DataGetter()
{
}
DataGetter::~DataGetter()
{
}


int DataGetter::ProcessPacket(uint8_t *packet)
{
	uint8_t *tmp = packet;
	//fprintf(stdout,"packet_size %d\n",packet_size);

	while(tmp<packet+packet_size)
	{
		Sig sig_tmp;
		if ( Interpret(tmp,sig_tmp)==1)
		{
			//if (sig_tmp.sid<3) if (sig_tmp.ADC<2000) continue;
			timesorter->fmutex_input.lock();
			timesorter->Push(sig_tmp);
			timesorter->fmutex_input.unlock();
			sig_processed++;
		}
	}
	return sig_processed;
}

void DataGetter::Loop()
{
	fmutex.lock();
	getterEnd=0;
	fmutex.unlock();
	while (1)
	{
		fmutex.lock();
		if (getterEnd) break;
		fmutex.unlock();
		uint8_t *packet = GetNextPacket();
#ifdef PEND_READING_ON_FULL
		while (timesorter->GetNSorted()+timesorter->GetNenque()>NBUFF_SORTER_ULIMIT) usleep(100000);
#endif // PEND_READING_ON_FULL
		if (packet_size>0) ProcessPacket(packet);
		else usleep(1000); // 1000 us = 1 ms
		//else usleep(1000000); // 1000 us = 1 ms
		//usleep(10000);
	}
	fmutex.unlock();
}
void DataGetter::Stop()
{
	fmutex.lock();
	getterEnd=1;
	fmutex.unlock();
}
