


#include "Sig.h"

/*
Sig::Sig(uint8_t *data)
{
	uint64_t ltmp;
	uint32_t itmp;
	uint16_t stmp;
}
*/

void Sig::PrintSummary()
{
	fprintf(stdout,"tn %u ct %u\t",trigger_number,coarse_time);
	fprintf(stdout,"sid %u brd %u cha %u\t",sid,brd,cha);
	fprintf(stdout,"ADC %u ft %u\t",ADC,fine_time);
	fprintf(stdout,"\b\n");
}


