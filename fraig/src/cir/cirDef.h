/****************************************************************************
  FileName     [ cirDef.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic data or var for cir package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_DEF_H
#define CIR_DEF_H

#include <vector>
#include "myHashMap.h"

using namespace std;

// TODO: define your own typedef or enum

class CirGate;
class CirMgr;
class CirPIGate;
class CirPOGate;
class CirAIGGate;
class CirUndefGate;
class CirConstGate;
class CirGateInfo;
class SatSolver;

typedef vector<CirGate*>           GateList;
typedef vector<unsigned>           IdList;
#endif // CIR_DEF_H
