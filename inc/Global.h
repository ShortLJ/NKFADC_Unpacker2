

#ifndef __GLOBAL__
#define __GLOBAL__

//#define WAVEFORM 1
#define NBUFF_SORTER 1000










//// maxium
#define N_SID 16 // 0~15 sid mask 0xF0
#define N_BRD 10 // 0~9 mid mask 0x0F but a crate has 10 board slots
#define N_CHA 32 // 0~9 cha mask 0xFF but NKFADC10 has 10 mcx inputs

//// used
#define Nsid 1
#define Nbrd 10
#define Ncha 32



/// global static variables defined in Config.h
#include <stdint.h>

#define Ntype 6
// 0: simple
// 1,2: ASGARD FV, Seg
// 3,4,5: StarkJr pad,strp U,D
#define Ndet 48
extern uint8_t	map_type	[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_det		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
extern uint8_t	map_idx		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
///




#endif // __GLOBAL__
