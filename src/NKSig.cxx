


#include "NKSig.h"


NKSig::NKSig(uint8_t *data)
{
	uint64_t ltmp;
	uint32_t itmp;
	uint16_t stmp;

    data_length = data[0] & 0xFF;
	trigger_type = data[1] & 0xFF;

	tcb_trigger_number = 0;
	itmp = data[2] & 0xFF;
	itmp = itmp << 0;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[3] & 0xFF;
	itmp = itmp << 8;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[4] & 0xFF;
	itmp = itmp << 16;
	tcb_trigger_number = tcb_trigger_number + itmp;
	itmp = data[5] & 0xFF;
	itmp = itmp << 24;
	tcb_trigger_number = tcb_trigger_number + itmp;

	uint16_t tcb_trigger_fine_time = data[6] & 0xFF;

	uint64_t tcb_trigger_coarse_time = 0;
	itmp = data[7] & 0xFF;
	itmp = itmp << 0;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;
	itmp = data[8] & 0xFF;
	itmp = itmp << 8;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;
	itmp = data[9] & 0xFF;
	itmp = itmp << 16;
	tcb_trigger_coarse_time = tcb_trigger_coarse_time + itmp;


	brd = data[10] & 0x0F;
	sid = (data[10] & 0xF0) >> 4;

	cha = data[11] & 0x7F;
	cha = cha-1;
	ADC_type = (data[11] & 0x80) >> 7;

	local_trigger_number = 0;
	itmp = data[12] & 0xFF;
	itmp = itmp << 0;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[13] & 0xFF;
	itmp = itmp << 8;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[14] & 0xFF;
	itmp = itmp << 16;
	local_trigger_number = local_trigger_number + itmp;
	itmp = data[15] & 0xFF;
	itmp = itmp << 24;
	local_trigger_number = local_trigger_number + itmp;

	local_trigger_pattern = 0;
	itmp = data[16] & 0xFF;
	itmp = itmp << 0;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[17] & 0xFF;
	itmp = itmp << 8;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[18] & 0xFF;
	itmp = itmp << 16;
	local_trigger_pattern = local_trigger_pattern + itmp;
	itmp = data[19] & 0xFF;
	itmp = itmp << 24;
	local_trigger_pattern = local_trigger_pattern + itmp;

	uint16_t local_gate_fine_time = data[20] & 0xFF;

	uint64_t local_gate_coarse_time = 0;
	ltmp = data[21] & 0xFF;
	ltmp = ltmp << 0;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[22] & 0xFF;
	ltmp = ltmp << 8;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[23] & 0xFF;
	ltmp = ltmp << 16;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[24] & 0xFF;
	ltmp = ltmp << 24;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[25] & 0xFF;
	ltmp = ltmp << 32;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;
	ltmp = data[26] & 0xFF;
	ltmp = ltmp << 40;
	local_gate_coarse_time = local_gate_coarse_time + ltmp;


	ADC = 0;
	stmp = data[27] & 0xFF;
	stmp = stmp << 0;
	ADC = ADC + stmp;
	stmp = data[28] & 0xFF;
	stmp = stmp << 8;
	ADC = ADC + stmp;

	fine_time = 0;
	stmp = data[29] & 0xFF;
	stmp = stmp << 8;
	fine_time = fine_time + stmp;
	stmp = data[30] & 0xFF;
	stmp = stmp << 8;
	fine_time = fine_time + stmp; // lsb=?ns

	flag = data[31] & 0xFF;    // pile_up flag @ bit1, hit flag @ bit0


	tcb_trigger_time = (uint64_t)tcb_trigger_coarse_time * 1000 + (tcb_trigger_fine_time << 3);
	local_gate_time = (uint64_t)local_gate_coarse_time * 1000 + (local_gate_fine_time << 3); // lsb=1ns, 8ns resolution

	trigger_number = tcb_trigger_number;
	coarse_time = local_gate_time;


}


void NKSig::Print(int verbosity)
{
	if (verbosity>0)
	{
		fprintf(stdout,"ttn %u\t",tcb_trigger_number);
		fprintf(stdout,"ltn %u\t",local_trigger_number);
		fprintf(stdout,"sid %u brd %u cha %u\t",sid,brd,cha);
		fprintf(stdout,"ttt %lu\t",tcb_trigger_time);
		fprintf(stdout,"lgt %lu\t",local_gate_time);
		fprintf(stdout,"ltp %u\t",local_trigger_pattern);
		fprintf(stdout,"\b\n");
	}
	PrintSummary();
}


