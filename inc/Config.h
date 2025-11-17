
#include <string>



#ifndef __CONFIG__
#define __CONFIG__

#include "Global.h"

bool enabled [N_SID][N_BRD][N_CHA];
bool participate_ref [N_SID][N_BRD][N_CHA];
uint8_t	map_type	[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
uint8_t	map_det		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
uint8_t	map_idx		[N_SID][N_BRD][N_CHA];// = {{{0xFF}}};
float Ecal_par0		[N_SID][N_BRD][N_CHA];
float Ecal_par1		[N_SID][N_BRD][N_CHA];
float Ecal_par2		[N_SID][N_BRD][N_CHA];

float seg_pos_cart [Nclover][Ncrystal][Nseg][3];
float strip_pos_cart [Nx6][Nstrip][3];



using namespace std;

class Config
{
	public:
		Config();
		Config(string configdir);
		~Config();

		void ReadDetMapFile(string filename="det_map.csv");
		void ReadRefPartiFile(string filename="ref_parti.csv");
		void ReadErgCalFile(string filename="erg_cal.csv");
		void ReadSegPosFile(string filename="seg_pos.csv");
		void ReadStripPosFile(string filename="strip_pos.csv");

	private:
		void InitializeGlobalVariables();
		
		string configdir;
		uint8_t isid, ibrd, icha;



};




#endif // __CONFIG__
