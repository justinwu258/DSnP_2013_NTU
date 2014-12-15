/****************************************************************************
  FileName     [ cirGate.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define class CirAigGate member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <stdarg.h>
#include <cassert>
#include "cirGate.h"
#include "cirMgr.h"
#include "util.h"

using namespace std;

extern CirMgr *cirMgr;

// TODO: Implement memeber functions for class(es) in cirGate.h

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
    cout << "==================================================" << endl;
    ostringstream oStr;
    int remainLenth = 0;
    oStr << "= " << (*this)._type << "(" <<  (*this).getID() << ")"
         << ", line " << (*this).getLineNo();
    remainLenth = 50 - oStr.str().length();
    cout << oStr.str() << right << setw(remainLenth) << "=" << endl;  
    cout << "==================================================" << endl;

}

void
CirGate::reportFanin(int level) const
{
   assert (level >= 0);
   int printCount = 0;
   bool invert = false;
   recurFaninDFS(level, this,printCount, invert);
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
}

void CirGate::recurFaninDFS(int level,const CirGate* gate, int N, bool inv) const {
    
    for (int i = 0; i < N ; i++) {
        cout << "  ";
    }
    if(inv) cout << "!";
    inv = false;    

    if(gate->_type == "PO") {
        cout << gate->_type << " " << gate->getID()<< endl;
        if(level != 0){
            if(((CirPOGate*)gate)->getIsInv()) inv = true;
            recurFaninDFS(level-1,gate->_faninList[0],N+1,inv);
        }
    }
    if(gate->_type == "PI") {
        cout << gate->_type << " " << gate->getID() << endl;
    }
    if(gate->_type == "") {
        cout << gate->_type << "UNDEF " << gate->getID() << endl;
    }
    if(gate->_type == "AIG") { 
        cout << gate->_type << " " << gate->getID() << endl;
        if(level != 0){
            // search rhs1
            if(((CirAIGGate*)gate)->getRhs1Inv()) inv = true;
            else                                  inv = false;
            recurFaninDFS(level-1,gate->_faninList[0],N+1,inv);
            // search rhs2
            if(((CirAIGGate*)gate)->getRhs2Inv()) inv = true;
            else                                  inv = false;
            recurFaninDFS(level-1,gate->_faninList[1],N+1,inv);
        }
    }
}

