
//#include <iostream>
#include <unistd.h>

#include "DataGetter.h"

DataGetter::DataGetter()
{
}
DataGetter::~DataGetter()
{
}


int DataGetter::ProcessData()
{
	uint8_t *tmp = data;

	while(tmp<data+8192)
	{
		data_length = tmp[0] & 0xFF;
		if (data_length != 32)
		{
			fprintf(stderr, "\ndata_length %u!=32\n data_read += fread(data, 1, 8160, fp);\n",data_length);
		}
		else
		{
			//Sig sig_tmp(tmp);
			NKSig nksig_tmp(tmp); Sig sig_tmp=nksig_tmp.GetSig();
			timesorter->fmutex_input.lock();
			timesorter->Push(sig_tmp);
			timesorter->fmutex_input.unlock();
			sig_processed++;
			tmp += 32;
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
		int ret = GetNextData();
		if (ret) ProcessData();
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
