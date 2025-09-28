

#include "Event.h"

#include "HistServer.h"

#ifndef __HISTSERVERUSER__
#define __HISTSERVERUSER__


#define Nclover 12
#define Ncrysal 4
#define Nseg 8
#define Nfv 2

#define Nx6 12
#define Nstrip 6
#define Npad 4



class HistServerUser : public HistServer
{
	public:
		//HistServerUser();
		HistServerUser(string opt_http="");
		~HistServerUser();

		void InitUser();
		void ProcessToHistUser();

///////////// User Area top /////////////////
	private:
		TH2 *h2_ADC_cha;
		TH2 *h2_Energy_cha;
	
		TH1 *h1_Clover_ADC_seg[Nclover][Ncrysal][Nseg];
		TH1 *h1_Clover_Energy_seg[Nclover][Ncrysal][Nseg];

		TH2 *h2_X6_Energy_idx[Nx6];
		TH2 *h2_X6_Pos_idx[Nx6];


		double inner_product_spherical(float r1, float t1, float p1, float r2, float t2, float p2)
		{
			//return r1*r2 + ......... ;
			return 0;
		}
///////////// User Area bottom  /////////////////
};



#endif // __HISTSERVERUSER__
