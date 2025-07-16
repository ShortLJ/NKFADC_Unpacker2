
#include <vector>

#include "SigAna.h"

#ifndef __EVTSIMPLE__
#define __EVTSIMPLE__

using namespace std;

class EvtSimple
{
	public:
		vector<SigAna> vSigAna;

		EvtSimple();
		EvtSimple(vector<SigAna> v_sigana);
		EvtSimple(vector<SigAna> v_sigana[]);
		~EvtSimple();

		void Clear();
		bool isValid();


	private:

};



#endif //__EVTSIMPLE__
