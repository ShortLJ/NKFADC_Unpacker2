

#include "Event.h"

#include "HistServer.h"

#ifndef __HISTSERVERUSER__
#define __HISTSERVERUSER__

class HistServerUser : protected HistServer
{
	HistServerUser();
	~HistServerUser();

	void InitUser();
	TH1* MakeH1();
	TH2* MakeH2();
	void ProcessToHistUser();

///////////// User Area top /////////////////
	TH1 *h1_ADC[Nclover][Ncrysal][Nseg];
	TH1 *h1_Energy[Nclover][Ncrysal][Nseg];

	TH2 *h2_Energy_cha;
///////////// User Area bottom  /////////////////
}



#endif // __HISTSERVERUSER__
