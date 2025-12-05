

#ifndef __GLOBAL__
#define __GLOBAL__

//#define WAVEFORM 1
#define NBUFF_SORTER 20000

#define PEND_READING_ON_FULL 1
#ifdef PEND_READING_ON_FULL
	#define NBUFF_SORTER_ULIMIT 1000000
#endif // PEND_READING_ON_FULL










//// maxium
#define N_SID 16 // 0~15 sid mask 0xF0
#define N_BRD 10 // 0~9 mid mask 0x0F but a crate has 10 board slots
#define N_CHA 32 // 0~9 cha mask 0xFF but NKFADC10 has 10 mcx inputs

//// used
#define Nsid 10
#define Nbrd 10 //// mid 9 not used
#define Ncha 32



/// global static variables defined in Config.h
#include <stdint.h>

#define Ntype 6
// 0: simple
// 1,2: ASGARD FV, Seg
// 3,4,5: StarkJr pad,strp U,D
#define Ndet 56

	#define Nclover 10
	#define Ncrystal 4
	#define Nseg 8
	#define Nfv 2
	#define Nx6 12
	#define Nstrip 8
	#define Npad 4








#endif // __GLOBAL__
