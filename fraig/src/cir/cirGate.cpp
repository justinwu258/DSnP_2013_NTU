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

// TODO: Keep "CirGate::reportGate()", "CirGate::reportFanin()" and
//       "CirGate::reportFanout()" for cir cmds. Feel free to define
//       your own variables and functions.

extern CirMgr *cirMgr;

/**************************************/
/*   class CirGate member functions   */
/**************************************/
void
CirGate::reportGate() const
{
    cout << "==================================================" << endl;
    ostringstream oStr;
    int remainLenth = 0;
    oStr << "= " << (*this)._type << "(" <<  (*this).getID() << ")";
    if((*this)._name != "") oStr << "\"" << (*this)._name << "\"";
    oStr     << ", line " << (*this).getLineNo();
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
   vector<CirAIGGate*>  vPathRecord;
   recurFaninDFS(level, this,printCount, invert, vPathRecord);
   for(vector<CirAIGGate*>::iterator it = vPathRecord.begin(); it != vPathRecord.end(); it++) {
        if(*it != 0) {
             (*it)->_isRecurVisited = false;
        }
   }
   vPathRecord.clear();
}

void
CirGate::reportFanout(int level) const
{
   assert (level >= 0);
   int printCount = 0;
   bool invert = false;
   vector<CirAIGGate*>  vPathRecord;
   recurFanoutDFS(level, this,printCount, invert, vPathRecord);
   for(vector<CirAIGGate*>::iterator it = vPathRecord.begin(); it != vPathRecord.end(); it++) {
        if(*it != 0) {
             (*it)->_isRecurVisited = false;
        }
   }
   vPathRecord.clear();
}

void CirGate::recurFaninDFS(int level,const CirGate* gate, int N, bool inv , vector<CirAIGGate*>& vPathRecord) const {
    
    for (int i = 0; i < N ; i++) {
        cout << "  ";
    }
    if(inv) cout << "!";
    inv = false;    

    if(gate->_type == "PO") {
        cout << gate->_type << " " << gate->getID()<< endl;
        if(level != 0){
            if(((CirPOGate*)gate)->getIsInv()) inv = true;
            
            if(gate->_faninList.size()==0) { 
                CirUndefGate* undef = new CirUndefGate(0,((CirPOGate*)gate)->getFaninID()); //only for recur issue
                recurFaninDFS(level-1,undef,N+1,inv , vPathRecord);  
                delete undef;         //remember release it 
            }
            else recurFaninDFS(level-1,gate->_faninList[0],N+1,inv , vPathRecord);
        }
    }
    if(gate->_type == "PI") {
        cout << gate->_type << " " << gate->getID() << endl;
    }
    if(gate->_type == "CONST") {
        cout << gate->_type << " 0"  << endl;
    }
    if(gate->_type == "") {
        cout << gate->_type << "UNDEF " << gate->getID() << endl;
    }
    if(gate->_type == "AIG") { 
        if(gate->_isRecurVisited && level != 0)
            cout << gate->_type << " " << gate->getID() << " (*)"<< endl;
        else {
            cout << gate->_type << " " << gate->getID() << endl;
            if(level != 0){
                // search rhs1
                if(((CirAIGGate*)gate)->getRhs1Inv()) inv = true;
                else                                  inv = false;
                recurFaninDFS(level-1,gate->_faninList[0],N+1,inv, vPathRecord);
                // search rhs2
                if(((CirAIGGate*)gate)->getRhs2Inv()) inv = true;
                else                                  inv = false;
                recurFaninDFS(level-1,gate->_faninList[1],N+1,inv, vPathRecord);
                gate->_isRecurVisited = true; // level != 0 , print will do , set it visited
            }
            vPathRecord.push_back((CirAIGGate*)gate);
        }
    }
}

void CirGate::recurFanoutDFS(int level,const CirGate* gate, int N, bool inv , vector<CirAIGGate*>& vPathRecord) const {
    for (int i = 0; i < N ; i++) {
        cout << "  ";
    }
    if(inv) cout << "!";
    inv = false; 
    
       
    if(gate->_type == "PO") {
        cout << gate->_type << " " << gate->getID()<< endl;
    }
    if(gate->_type == "PI" || gate->_type == "CONST" || gate->_type == "" || gate->_type == "AIG") {
        if(gate->_type == "PI")
            cout << gate->_type << " " << gate->getID() << endl;
        else if(gate->_type == "CONST")
            cout << gate->_type << " 0"  << endl;
        else if(gate->_type == "")
            cout << gate->_type << "UNDEF " << gate->getID() << endl;
        else if(gate->_type == "AIG"){
            if(gate->_isRecurVisited && level != 0)
                cout << gate->_type << " " << gate->getID() << " (*)"<< endl;
            else {
                cout << gate->_type << " " << gate->getID() << endl;
            }
        }
                        
        if(level != 0 &&  gate->_isRecurVisited == false){
          //  if(((CirPIGate*)gate)->getIsInv()) inv = true;
          //  if(gate->_fanoutList.size()==0) 
          //      recurFaninDFS(level-1,new CirUndefGate(0,-1),N+1,inv , vPathRecord);
          //  else recurFaninDFS(level-1,gate->_faninList[0],N+1,inv , vPathRecord);
            
            for(vector<CirGate*>::const_iterator it = gate->_fanoutList.begin(); it != gate->_fanoutList.end(); it++) {
                 if(*it != 0) {
                      if((*it)->_type == "AIG") {
                          if((*it)->_faninList[0] == gate) {
                              if(((CirAIGGate*)(*it))->getRhs1Inv()) inv = true;
                              else                                   inv = false;
                          } else if((*it)->_faninList[1] == gate) {
                              if(((CirAIGGate*)(*it))->getRhs2Inv()) inv = true;
                              else                                   inv = false;
                          }
                      }
                      if((*it)->_type == "PO") {
                          if((*it)->_faninList[0] == gate) {
                              if(((CirPOGate*)(*it))->getIsInv()) inv = true;
                              else                                inv = false;
                 //     cout << "gate->getID() = " << gate->getID() << endl;
                 //     cout << "gate->_isRecurVisited = " << gate->_isRecurVisited << endl;
                 //     cout << "(*it)->getID() = " << (*it)->getID() << endl;
                 //     cout << "(*it)->_isRecurVisited = " << (*it)->_isRecurVisited << endl;
                            //  if(gate->_type == "AIG"){
                            //       gate->_isRecurVisited = true;
                            //  }
                          } 
                          else cout << " this is bug!!" << endl;
                      }
                    //  cout << "gate->getID() = " << gate->getID() << endl;
                    //  cout << "gate->_isRecurVisited = " << gate->_isRecurVisited << endl;
                    //  cout << "(*it)->getID() = " << (*it)->getID() << endl;
                    //  cout << "(*it)->_isRecurVisited = " << (*it)->_isRecurVisited << endl;
                      recurFanoutDFS(level-1,*it ,N+1,inv , vPathRecord);
                 }
                 
                 if(gate->_type == "AIG"){
                     gate->_isRecurVisited = true;
                 }
                 vPathRecord.push_back((CirAIGGate*)gate);
            }
        }
    }
}
