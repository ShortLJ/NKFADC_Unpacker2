
#include "Sig.h"
#include "SigAna.h"
#include "HitCrystal.h"
#include "HitClover.h"
#include "HitX6.h"
#include "EvtSimple.h"
#include "EvtASGARD.h"
#include "EvtStarkJr.h"


#ifdef __CLING__
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ class SigAna+;
#pragma link C++ class vector<SigAna>+;
#pragma link C++ class EvtSimple+;

#pragma link C++ class HitCrystal+;
#pragma link C++ class vector<HitCrystal>+;
#pragma link C++ class HitClover+;
#pragma link C++ class vector<HitClover>+;
#pragma link C++ class EvtASGARD+;

#pragma link C++ class HitPad+;
#pragma link C++ class vector<HitPad>+;
#pragma link C++ class HitStrip+;
#pragma link C++ class vector<HitStrip>+;
#pragma link C++ class HitX6+;
#pragma link C++ class vector<HitX6>+;
#pragma link C++ class EvtStarkJr+;
#endif
