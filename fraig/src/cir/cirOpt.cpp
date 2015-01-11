/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir optimization functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

//#define debug_opt
using namespace std;

// TODO: Please keep "CirMgr::sweep()" and "CirMgr::optimize()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/**************************************************/
/*   Public member functions about optimization   */
/**************************************************/
void
CirMgr::sweep()
{
   //vector<CirAIGGate*>::iterator itAig;
   //vector<CirUndefGate*>::iterator itUndef;
   cout << "call sweep" << endl;
   for(vector<CirAIGGate*>::const_iterator it = _aigList.begin(); it != _aigList.end(); ) {
        if(!(*it)->_isVisited) {
            //cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            //cout << ", _isVisited = " << (*it)->_isVisited  << endl; 
            it = _aigList.erase(it); 
        }  else {
            //cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            //cout << ", _isVisited = " << (*it)->_isVisited << ", it++" << endl; 
            it ++;
        }
   }             
   for(vector<CirUndefGate*>::iterator it = _undefList.begin(); it != _undefList.end(); ) {
        if(!(*it)->_isVisited) {
            //cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            //cout << ", _isVisited = " << (*it)->_isVisited  << endl;  
            it = _undefList.erase(it); 
        }  else {
            //cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            //cout << ", _isVisited = " << (*it)->_isVisited << ", it++" << endl; 
            it++;
        }
   }             
   for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); ) {
        if(!(*it)->_isVisited) {
            it = _dfsList.erase(it); 
        }  else {
            it++;
        }
   }             
   for(vector<CirGate*>::iterator it = _totalList.begin(); it != _totalList.end(); ){
        if(*it != 0){
  //          cout << "  (Total)ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
  //          cout << ", _isVisited = " << (*it)->_isVisited  << endl;    
            if( (((*it)->_type == "AIG") || ((*it)->_type == "UNDEF")) && !(*it)->_isVisited ){
                cout << "Sweeping: " << (*it)->_type << "(" << (*it)->getID() << ") removed..." << endl;
          //      if( (*it)->_type == "AIG"){
          //          *itAig = (CirAIGGate*) *it; 
          //          _aigList.erase(itAig);
          //      }
          //      else if((*it)->_type == "UNDEF") {
          //          *itUndef = (CirUndefGate*) *it;
          //          _undefList.erase(itUndef);
          //      }
                delete *it;
                it = _totalList.erase(it);
            } else if ( ((((*it)->_type == "PI")) && !(*it)->_isVisited )) { // not visited , means fanout is no usage
                (*it)->_fanoutList.clear();
                it++;
            } else {
                it++;
            } 
            
           
        }  else {
            it++;
        } 
   } 
}

void
CirMgr::optimize()
{
    int i = 0, j = 0; 
    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){
            #ifdef debug_opt 
            cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            cout << ", _isVisited = " << (*it)->_isVisited  << endl;    
            #endif
            if( ((*it)->_type == "CONST") ) {
                #ifdef debug_opt
                cout << "  I'm CONST , do nothing" << endl; 
                #endif
            } else if( ((*it)->_type == "PI") ) {
                #ifdef debug_opt
                cout << "  I'm PI , do nothing" << endl; 
                #endif
            } else if( ((*it)->_type == "AIG") ) {  // check fanin & fanout
                if((*it)->_faninList[0]->_type == "CONST") {
                        (*it)->_isVisited = false;
                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
                                " merging " << (*it)->getID() << "..." << endl;
                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                    (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
                                    #ifdef debug_opt
                                    cout << "=== fanin has constant 0 ===" << endl <<
                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
                                    cout << " (*it)->_fanoutList[i] = " << 
                                            (*it)->_fanoutList[i] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
                                    #endif
                                }
                            }
                        }
                } else if((*it)->_faninList[1]->_type == "CONST") {
                        (*it)->_isVisited = false;
                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
                                " merging " << (*it)->getID() << "..." << endl;
                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                    (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
                                    #ifdef debug_opt
                                    cout << "=== fanin 2 has constant 0 ===" << endl <<
                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
                                    cout << " (*it)->_fanoutList[i] = " << 
                                            (*it)->_fanoutList[i] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
                                    #endif
                                }
                            }
                        }
                } else if((*it)->_faninList[0] == (*it)->_faninList[1]) {
                    if( ((CirAIGGate*)(*it))->_rhs1_invert && ((CirAIGGate*)(*it))->_rhs2_invert ) {   // both are inverted
                        (*it)->_isVisited = false;
                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
                                " merging !" << (*it)->getID() << "..." << endl;
                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                    (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[0];    // put gate , replace origin it
                                    if(j == 0) ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = 1;
                                    else       ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = 1;
                                    #ifdef debug_opt
                                    cout << "  i = " << i << ", j = " << j << ", (*it)->_fanoutList.size() = " << 
                                            (*it)->_fanoutList.size() << endl;
                                    cout << "(CirAIGGate*)(*it))->_rhs1_invert = " << ((CirAIGGate*)(*it))->_rhs1_invert 
                                         << ", (CirAIGGate*)(*it))->_rhs2_invert = " << ((CirAIGGate*)(*it))->_rhs2_invert
                                         << endl;
                                    cout << "=== two fanin is same gate , both are inverted ===" << endl <<
                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_faninList[j]->_type <<
                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
                                    cout << " (*it)->_fanoutList[i] = " << 
                                            (*it)->_fanoutList[i] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
                                    #endif
                                }
                            }
                        }
                    } else if( !((CirAIGGate*)(*it))->_rhs1_invert && !((CirAIGGate*)(*it))->_rhs2_invert ) {
                        (*it)->_isVisited = false;
                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
                                " merging " << (*it)->getID() << "..." << endl;
                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                    (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[0];    // put gate , replace origin it
                                    if(j == 0) ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = 0;
                                    else       ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = 0;
                                    #ifdef debug_opt
                                    cout << "  i = " << i << ", j = " << j << ", (*it)->_fanoutList.size() = "  <<
                                            (*it)->_fanoutList.size() << endl;
                                    cout << "(CirAIGGate*)(*it))->_rhs1_invert = " << ((CirAIGGate*)(*it))->_rhs1_invert 
                                         << ", (CirAIGGate*)(*it))->_rhs2_invert = " << ((CirAIGGate*)(*it))->_rhs2_invert
                                         << endl;
                                    cout << "=== two fanin is same gate ===" << endl <<
                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_faninList[j]->_type <<
                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
                                    cout << " (*it)->_fanoutList[i] = " << 
                                            (*it)->_fanoutList[i] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
                                    #endif
                                }
                            }
                        }
                    } else {    // one is inverted , one is origin
                        (*it)->_isVisited = false;
                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
                                " merging " << (*it)->getID() << "..." << endl;
                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
                                    (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
                                    #ifdef debug_opt
                                    cout << "=== two fanin is same gate, but phase different ===" << endl <<
                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
                                    cout << " (*it)->_fanoutList[i] = " << 
                                            (*it)->_fanoutList[i] << ",  type = " << 
                                            (*it)->_fanoutList[i]->_type << 
                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
                                    #endif
                                }
                            }
                        }
                    }
                } else if(1) {

                }
            } else if( ((*it)->_type == "PO") ) {
                #ifdef debug_opt
                cout << "  I'm PO , do nothing" << endl; 
                #endif
            } else if ( ((*it)->_type == "UNDEF")) {
                #ifdef debug_opt
                cout << "  I'm UNDEF , do nothing" << endl; 
                #endif
            }
        }   
    }
    CirMgr::sweep();
    #ifdef debug_opt
    cout << " ===== merging done ===== " << endl;
    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){ 
            cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            cout << ", _isVisited = " << (*it)->_isVisited  << endl;   
        } 
    }
    #endif
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

