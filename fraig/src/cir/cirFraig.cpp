/****************************************************************************
  FileName     [ cirFraig.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir FRAIG functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2012-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "sat.h"
#include "myHashMap.h"
#include "util.h"

using namespace std;

//#define debug_strash

// TODO: Please keep "CirMgr::strash()" and "CirMgr::fraig()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/*******************************************/
/*   Public member functions about fraig   */
/*******************************************/
void
CirMgr::strash()
{   
    int i,j;
    int poInv, aigInv; 
  if(_isStrashed) {
        cout << "Error: circuit has been strashed!!" << endl;
  } else {
    _isStrashed = true;
    #ifdef debug_strash
        cout << "_dfsList size = " << _dfsList.size() << endl;
        cout << "getHashSize(_dfsList.size()) = " << getHashSize(_dfsList.size()) << endl;
    #endif
    HashMap<HashKey,CirGate*> myHash(getHashSize(_dfsList.size()));
    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){ 
            if((*it)->_type == "AIG"){
                HashKey* myKey = new HashKey((*it)->_faninList[0]->getID(),(*it)->_faninList[1]->getID(),(*it)->_type, 
                                          ((CirAIGGate*)(*it))->_rhs1_invert,((CirAIGGate*)(*it))->_rhs2_invert);
                CirGate* d = (*it);
                if(myHash.check((*myKey),d)) {
                    #ifdef debug_strash
                    cout << "structure similar exist" << endl;
                    cout << "  current gateID = " << (*it)->getID() << ",  exist gateID = " << d->getID() << endl;
                    #endif
                    (*it)->_mergeVisited = true;
                    cout << "Strashing: " << d->getID()  << " merging " << (*it)->getID() << "..." << endl;
                    for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                        for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                            if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                for(int k = 0 ; k < (*it)->_faninList[0]->_fanoutList.size(); k++) {
                                   if( (*it)->_faninList[0]->_fanoutList[k] == (*it) ) {
                                        (*it)->_faninList[0]->_fanoutList.erase((*it)->_faninList[0]->_fanoutList.begin()+k);
                                        //(*it)->_faninList[0]->_fanoutList[k] = (*it)->_fanoutList[i];
                                    }
                                }
                                for(int k = 0 ; k < (*it)->_faninList[1]->_fanoutList.size(); k++) {
                                   if( (*it)->_faninList[1]->_fanoutList[k] == (*it) ) {
                                        (*it)->_faninList[1]->_fanoutList.erase((*it)->_faninList[1]->_fanoutList.begin()+k);
                                        //(*it)->_faninList[1]->_fanoutList[k] = (*it)->_fanoutList[i];
                                    }
                                }
                                d->_fanoutList.push_back((*it)->_fanoutList[i]);
                                (*it)->_fanoutList[i]->_faninList[j] = d;    // use d replace similar exist gate(*it)
                            }
                        }
                    }
                } else {
                    #ifdef debug_strash
                    cout << "insert gate to hash" << endl;
                    #endif
                    myHash.insert((*myKey),(*it));
                }
            } else if( ((*it)->_type == "PO") ) {
                ((CirPOGate*)(*it))->setFaninID((*it)->_faninList[0]->getID());
            }
        }
    }
    CirMgr::mergeSweep();
    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){
            (*it)->_isVisited = false;
        }
    }
    _dfsList.clear();
    for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
         myDFS(*it);
     }
  }
}

void
CirMgr::fraig()
{
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/

