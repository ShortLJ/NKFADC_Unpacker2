


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
	fprintf(stdout,"tn %u ct %lu\t",trigger_number,coarse_time);
	fprintf(stdout,"sid %u brd %u cha %u\t",sid,brd,cha);
	fprintf(stdout,"ADC %u ct %lu ft %u\t",ADC,coarse_time,fine_time);
	fprintf(stdout,"\b\n");
}

#ifdef STORE_BINARY
void Sig::print_binary()
{
       uint8_t *tt = binarydata;
       uint8_t *t = binarydata;
       while (t-tt<32)
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
#endif // STORE_BINARY

