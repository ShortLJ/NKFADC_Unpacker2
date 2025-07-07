
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
#include "HitCrystal.h"
#pragma link C++ class HitCrystal+;
#pragma link C++ class vector<HitCrystal>+;
#include "HitClover.h"
#pragma link C++ class HitClover+;
#pragma link C++ class vector<HitClover>+;
#include "EvtASGARD.h"
#pragma link C++ class EvtASGARD+;
#endif
