
#ifdef __CLING__
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#ifndef __SIGLINK__
#define __SIGLINK__
#include "Sig.h"
#include "SigAna.h"
#pragma link C++ class SigAna+;
#pragma link C++ class vector<SigAna>+;
#endif // __SIGLINK__
#include "HitX6.h"
#pragma link C++ class HitPad+;
#pragma link C++ class vector<HitPad>+;
#pragma link C++ class HitStrip+;
#pragma link C++ class vector<HitStrip>+;
#pragma link C++ class HitX6+;
#pragma link C++ class vector<HitX6>+;
#include "EvtStarkJr.h"
#pragma link C++ class EvtStarkJr+;
#endif
