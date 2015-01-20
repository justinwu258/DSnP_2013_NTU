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
  } else if(_isSimulated){
        cout << "Error: circuit has been simulated!! Do \"CIRFraig\" first!!" << endl;
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
                HashKey myKey((*it)->_faninList[0]->getID(),(*it)->_faninList[1]->getID(),(*it)->_type, 
                                          ((CirAIGGate*)(*it))->_rhs1_invert,((CirAIGGate*)(*it))->_rhs2_invert);
                CirGate* d = (*it);
                if(myHash.check((myKey),d)) {
                    #ifdef debug_strash
                    cout << "structure similar exist" << endl;
                    cout << "  current gateID = " << (*it)->getID() << ",  exist gateID = " << d->getID() << endl;
                    #endif
                    (*it)->_mergeVisited = true;
                    //cout << "Strashing: " << d->getID()  << " merging " << (*it)->getID() << "..." << endl;
                    merge(d,(*it),1,0,1);
                } else {
                    #ifdef debug_strash
                    cout << "insert gate to hash" << endl;
                    #endif
                    myHash.insert((myKey),(*it));
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
    if(_isSimulated) {
        _isStrashed = false;
        _isSimulated = false;

        SatSolver solver;
        solver.initialize();
        myGenProofModel(solver);
      
        int satCount = 0 , unsatCount = 0, gateB_ID = 0, cmpGateID;
        int _totalFECnum = _fecGrps.size(); 
        bool result, isConst = false, resInv = false;
        Var newV = solver.newVar();
     //   for(int i = 0; i < _fecGrps.size(); ++i) {
     //      for(int j = 0; j < _fecGrps[i]->size(); ++j) {
     //           
     //       } 
     //   } 
        for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++) {
            if((*it)!=0 && !(*it)->_mergeVisited && (*it)->_fecGrpsIdx != -1) {
                //cout << "now (*it) ID = " <<  (*it)->getID() << endl;
                if((*it)->_type == "AIG" ) {
                    if( ((*it)->_patternValue == _totalList[0]->_patternValue || (*it)->_patternValue == ~_totalList[0]->_patternValue)  && (*it)->_fecGrpsIdx == 0 ) { // const 0 case, use zero to merge
              //  cout << "is const error ??  " << endl;
                         isConst = true;
        CirGate* cmpGate = _totalList[0];
        Var newV = solver.newVar();
        solver.addXorCNF(newV, (*it)->getVar(), false , cmpGate->getVar(), false);
        solver.assumeProperty(newV, true);
        result = solver.assumpSolve();
        //                 satCheck(solver, (*it), gateB_ID,result, resInv,isConst);    
                         if(result == 0) { 
                             cout << "\r" << "Proving " << (*it)->getID() << " = " << 0 << "...UNSAT!!" << endl; 
                             merge(_totalList[0], (*it), 1, (*it)->_fecInv, 2);   
                             ++unsatCount;
                         } else {
                             cout << "\r" << "Proving " << (*it)->getID() << " = " << 1 << "...SAT!!" << endl;
                             ++satCount;
                         }            
                    } else {                      // AIG case
                         //cout << "is AIG error ??  " << endl;
                         isConst = false;
                        // cout << "(*it)->_fecGrpsIdx = " << (*it)->_fecGrpsIdx << endl;
                        //  cout << ", grp size = "  <<_fecGrps[(*it)->_fecGrpsIdx]->size() << endl;
                      //   satCheck(solver, (*it), gateB_ID,result, resInv,isConst);
                      //if(!(*it)->_mergeVisited) {
                          for(int j = 0; j < _fecGrps[(*it)->_fecGrpsIdx]->size(); ++j){
                              cmpGateID = (*_fecGrps[(*it)->_fecGrpsIdx])[j];
                              CirGate* cmpGate = _totalList[cmpGateID];
                             // cout << "  same group gateID = " <<  cmpGateID << ", group size = " << _fecGrps[(*it)->_fecGrpsIdx]->size() << endl;
                            if((*it) != cmpGate && !(cmpGate)->_mergeVisited && (*it)->_fecGrpsIdx == (cmpGate)->_fecGrpsIdx){ 
                                 Var newV = solver.newVar();
                                 solver.addXorCNF(newV, (*it)->getVar(), (*it)->_fecInv  , cmpGate->getVar(), cmpGate->_fecInv);
                            if((*it)->_patternValue == cmpGate->_patternValue)  solver.assumeProperty(newV, true);
                            else                                                solver.assumeProperty(newV, false);
                                 result = solver.assumpSolve();
                                 resInv = (*it)->_fecInv ^ _totalList[cmpGateID]->_fecInv;
                                 if(result == 0) { 
                                     cout << "\r" << "Proving (" << ((*it)->_fecInv? "!" : "") << (*it)->getID() << ", " << 
                                    (_totalList[cmpGateID]->_fecInv? "!" : "") << cmpGateID << ")...UNSAT!!" << endl;                                 
                                    _fecGrps[(*it)->_fecGrpsIdx]->erase(_fecGrps[(*it)->_fecGrpsIdx]->begin()+j);
                                    --j; 
                                    merge((*it), _totalList[cmpGateID], 1, resInv, 2);   
                                     ++unsatCount;
                                 } else {
                                     cout << "\r" << "Proving (" << ((*it)->_fecInv? "!" : "") << (*it)->getID() << ", " << 
                                             (_totalList[gateB_ID]->_fecInv? "!" : "") << cmpGateID << ")...SAT!!" << endl; 
                                     ++satCount;
                                 }
                             }            
                           }
                       //}
                    } 
                }
            }
        }
        //cout << "\r" << "Updating by UNSAT... Total #FEC Group = " << _fecGrps.size() - unsatCount;
        //cout << "\r" << "Updating by SAT... Total #FEC Group = " << _fecGrps.size() - unsatCount - satCount;
    //    delete solver;
    } else {
        cout << "Error: circuit is not yet simulated!!" << endl;
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

void 
CirMgr::satCheck(SatSolver& s, CirGate* gateA,int& gateB_ID , bool& result,bool& resInv, bool isConst){
    int j , cmpGateID;
    if(isConst) { // in this case , gateA = const 0  ,  gateB = in _fecGrp[0]
        CirGate* cmpGate = _totalList[0];
        Var newV = s.newVar();
        s.addXorCNF(newV, gateA->getVar(), gateA->_fecInv , cmpGate->getVar(), cmpGate->_fecInv);
        s.assumeProperty(newV, true);
        result = s.assumpSolve();
        // actually ,  below two no use 
        gateB_ID = 0;    
        resInv = gateA->_fecInv;
    } else {
        for(j = 0; j < _fecGrps[gateA->_fecGrpsIdx]->size(); ++j){
            cmpGateID = (*_fecGrps[gateA->_fecGrpsIdx])[j];
            CirGate* cmpGate = _totalList[cmpGateID];
            if( gateA != cmpGate ) {
                Var newV = s.newVar();
                s.addXorCNF(newV, gateA->getVar(), gateA->_fecInv , cmpGate->getVar(), cmpGate->_fecInv);
                s.assumeProperty(newV, true);
                result = s.assumpSolve();
                gateB_ID = cmpGateID;
                resInv = gateA->_fecInv ^ cmpGate->_fecInv;
            }
        }
    }

}

void
CirMgr::myGenProofModel(SatSolver& s){
    int i;
 //   for(i = 0; i < I; ++i) {
 //           Var v = s.newVar();
 //           _totalList[_piList[i]->getID()]->setVar(v);
 //   }
    for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++){
        if((*it) != 0) {
            Var v = s.newVar();
            (*it)->setVar(v);
        }
    }
    for(vector<CirAIGGate*>::const_iterator it = _aigList.begin(); it != _aigList.end(); it++) {
        if((*it) != 0) {
            Var v = s.newVar();
            (*it)->setVar(v);
            if((*it)->_isVisited){
                s.addAigCNF((*it)->getVar(),(*it)->_faninList[0]->getVar(),(*it)->_rhs1_invert,
                                            (*it)->_faninList[1]->getVar(),(*it)->_rhs2_invert );
            }
        }
    }
    _totalList[0]->setVar(0); // const Gate add , too

    //for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++){
    //    (*it)->setVargt
    //}
}

/********************************************/
/*   Private member functions about fraig   */
/********************************************/

