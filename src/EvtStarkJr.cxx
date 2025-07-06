

#include "EvtStarkJr.h"

EvtStarkJr::EvtStarkJr()
{
}
EvtStarkJr::EvtStarkJr(vector<HitX6> v_hit_x6)
{
	vHitX6 = v_hit_x6;
}
EvtStarkJr::~EvtStarkJr()
{

}

void EvtStarkJr::Clear()
{
	vHitX6.clear();
}
bool EvtStarkJr::isValid()
{
	return true;
}
