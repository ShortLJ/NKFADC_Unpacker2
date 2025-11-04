

#include "NKMessageQueue.h"

NKMessageQueue::~NKMessageQueue()
{
	mq_unlink(mq_name.c_str());

}

uint8_t* NKMessageQueue::GetNextPacket()
{
	clock_gettime(CLOCK_REALTIME,&timeout);
	timeout.tv_sec += 1;
	packet_size = mq_timedreceive(msgq, (char*)&NKPacket, atr.mq_msgsize,NULL,&timeout);
	if (packet_size==-1)
	{
		perror("mq_receive error:");
		//fprintf(stdout,"%jd %09ld\n", timeout.tv_sec, timeout.tv_nsec);
	}
	else
	{
		//fprintf(stdout,"received %d bytes, starting with %02x\n", packet_size,*NKPacket);
	}
	return NKPacket;
}
int NKMessageQueue::Interpret(uint8_t *&tmp, Sig &sig)
{
	int ret;
	uint8_t data_type = tmp[0] & 0xFF;
	uint16_t data_length=0;

	switch (data_type)
	{
		case 0x20:
		{
			//fprintf(stdout, "data_type %u: ADC data\n",data_type);
			NKSig nksig(tmp);
			//nksig.Print();
			data_length = 32;
			//print_binary(tmp,32);
			sig = nksig.GetSig();
			ret = 1;
			break;
		}
		case 0x40:
		{
			//fprintf(stdout, "data_type %u: TCB data\n",data_type);
			data_length = 64;
			ret = 2;
			break;
		}
		case 0x00:
		{
			if (tmp[1]==0x20)
			{
				// waveform data
				fprintf(stdout,"waveform data found\n");
				data_length = 8192;
				ret = 3;
				break;
			}
			// else : proceed to default
		}
		default:
		{
			fprintf(stderr, "NKMessageQueue::Interpret : data_type %u: unknown\n",data_type);
			//data_length = packet_size;
			data_length = 8192;
			//print_binary(tmp,16);
			ret = -1;
			//exit(-61);
			break;
		}
	}
	tmp += data_length;
	return ret;
}

NKMessageQueue::NKMessageQueue(string mq_name_)
	:mq_name(mq_name_)
{
	int ret;
	msgq = mq_open(mq_name.c_str(), O_RDONLY, 0666, &atr);
	if (msgq == -1)
	{
		perror("mq_open error:");
		exit(-22);
	}
	ret =  mq_getattr(msgq, &atr);
	fprintf(stdout, "mq \"%s\" opend. mq_attr: %ld %ld %ld %ld\n", mq_name.c_str(), atr.mq_flags, atr.mq_maxmsg, atr.mq_msgsize, atr.mq_curmsgs);

}




	
