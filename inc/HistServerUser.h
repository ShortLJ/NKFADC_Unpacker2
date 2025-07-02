

#include "Event.h"

#include "HistServer.h"

#ifndef __HISTSERVERUSER__
#define __HISTSERVERUSER__

class HistServerUser : protected HistServer
{
	public:
		HistServerUser();
		~HistServerUser();

		void InitUser();
		TH1* MakeH1();
		TH2* MakeH2();
		void ProcessToHistUser();

///////////// User Area top /////////////////
	private:
		TH1 *h1_ADC[Nclover][Ncrysal][Nseg];
		TH1 *h1_Energy[Nclover][Ncrysal][Nseg];

		TH2 *h2_Energy_cha;
	
		double inner_product_spherical(float r1, float t1, float p1, float r2, float t2, float p1)
		{
			//return r1*r2 + ......... ;
			return 0;
		}
///////////// User Area bottom  /////////////////
}



#endif // __HISTSERVERUSER__
