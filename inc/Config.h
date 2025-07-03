
#include "Global.h"

#ifndef __CONFIG__
#define __CONFIG__

uint8_t	map_type	[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
uint8_t	map_det		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
uint8_t	map_idx		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};



class Config
{
	public:
		Config();
		Config(string configdir);
		~Config();

		void ReadDetMapFile(string filename="det_map.csv");

	private:
		void InitializeGlobalVariables();
		
		string configdir;
		uint8_t isid, ibrd, icha;
		uint8_t itype, idet, iidx;



}




#endif // __CONFIG__
