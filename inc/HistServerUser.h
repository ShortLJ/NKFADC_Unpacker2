
#include "Global.h"
#include "Event.h"

#include "HistServer.h"

#ifndef __HISTSERVERUSER__
#define __HISTSERVERUSER__




class HistServerUser : public HistServer
{
	public:
		//HistServerUser();
		HistServerUser(string opt_http="");
		~HistServerUser();

		void InitUser();
		void ProcessToHistUser();

		void InitRaw();
		void ProcessToHistRaw();

	private:
		uint32_t tn0=0;
		uint64_t TS0=0; uint64_t TS1=0;
		TH2 *h2_timestamp_tn;
		TH1 *h1_timestamp_structure_total;
		TH1 *h1_timestamp_structure_total_avg;
		TH2 *h2_timestamp_structure_cha;
		TH2 *h2_timestamp_structure_cha_avg;
		TH2 *h2_timestamp_structure_Energy;
		TH2 *h2_timestamp_structure_Energy_avg;

		TH2 *h2_ADC_cha[Nsid];

///////////// User Area top /////////////////
	private:

		TH2 *h2_FVEnergy_cha;
		TH2 *h2_Eg_Eg;
	
		TH1 *h1_Clover_Energy_seg[Nclover][Ncrystal][Nseg];
		TH1 *h1_Clover_Energy_fv[Nclover][Ncrystal][Nfv];
		TH1 *h1_Clover_Energy_fv_all;

		TH2 *h2_X6_Energy_idx[Nx6];
		TH2 *h2_X6_Pos_idx[Nx6];
		TH2 *h2_X6_ADC_ADC[Nx6][Nstrip];

		TH2 *h2_X6_Pos_idx_byPad[Nx6][Npad];
		TH2 *h2_X6_ADC_ADC_byPad[Nx6][Nstrip][Npad];
		TH2 *h2_X6_ADCstrip_ADCpad[Nx6][Nstrip][Npad];

		TH2 *h2_X6_Energy_theta;


		double inner_product_spherical(float r1, float t1, float p1, float r2, float t2, float p2)
		{
			//return r1*r2 + ......... ;
			return 0;
		}
///////////// User Area bottom  /////////////////
};



#endif // __HISTSERVERUSER__
