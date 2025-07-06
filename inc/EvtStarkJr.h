


#include "HitX6.h"


#ifndef __EVTSTARKJR__
#define __EVTSTARKJR__

using namespace std;

class EvtStarkJr
{
	public:
		vector<HitX6> vHitX6;

		EvtStarkJr();
		EvtStarkJr(vector<HitX6> v_hit_x6);
		~EvtStarkJr();

		void Clear();
		virtual bool isValid();

	private:

};

#endif // __EVTSTARKJR__
