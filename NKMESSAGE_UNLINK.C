#include <fcntl.h>
#include <mqueue.h>

#define MQ_NAME "/MQ_ASGARD"

int NKMESSAGE_UNLINK()
{
	mq_attr atr;
	mqd_t msgq;

	msgq = mq_open(MQ_NAME, O_RDWR|O_CREAT, 0666, NULL);


	return 	mq_unlink(MQ_NAME);
}
