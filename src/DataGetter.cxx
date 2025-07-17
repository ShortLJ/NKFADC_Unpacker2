
//#include <iostream>
#include <unistd.h>

#include "DataGetter.h"

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
		if (packet_size) ProcessPacket(packet);
		else usleep(1000000); // 1000 us = 1 ms
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
