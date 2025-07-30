
#include "DataGetter.h"

#ifndef __NKMESSAGEQUEUE__
#define __NKMESSAGEQUEUE__


#include <fcntl.h>
#include <mqueue.h>
#include <time.h>

#include "NKSig.h"

class NKMessageQueue : public DataGetter
{
	public:
		uint8_t *GetNextPacket() override;
		int Interpret(uint8_t *&tmp, Sig &sig) override;

	public:
		NKMessageQueue(string mq_name);
		~NKMessageQueue();
	private:
		string mq_name;
		mq_attr atr;
		mqd_t msgq;
		struct timespec timeout;

		uint8_t NKPacket[16384];


};


#endif // __NKMESSAGEQUEUE__
