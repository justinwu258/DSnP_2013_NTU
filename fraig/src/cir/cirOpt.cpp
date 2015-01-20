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
   #ifdef debug_opt 
   cout << "call sweep" << endl;
   #endif
   for(vector<CirAIGGate*>::iterator it = _aigList.begin(); it != _aigList.end(); ) {
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
                //cout << "  (*it)->_faninList.size() = " << (*it)->_faninList.size() << 
                //        "  (*it)->ID = " << (*it)->getID() << endl;
                for(int i = 0; i < (*it)->_faninList.size(); i++){ // Note: AIG fanin = 2 , ** UNDEF maybe = 0
                    for(int j = 0; j < (*it)->_faninList[i]->_fanoutList.size(); j++) {
                        if( (*it)->_faninList[i]->_fanoutList[j] == (*it) ) {
                            (*it)->_faninList[i]->_fanoutList.erase((*it)->_faninList[i]->_fanoutList.begin()+j);
                        }
                    }
                }
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
CirMgr::mergeSweep()
{
   #ifdef debug_opt 
   cout << "call sweep" << endl;
   #endif
   for(vector<CirAIGGate*>::iterator it = _aigList.begin(); it != _aigList.end(); ) {
        if((*it)->_mergeVisited && ((*it)->_isVisited)) {
            it = _aigList.erase(it); 
        }  else {
            it ++;
        }
   }             
   for(vector<CirUndefGate*>::iterator it = _undefList.begin(); it != _undefList.end(); ) {
        if((*it)->_mergeVisited && ((*it)->_isVisited)) {
            it = _undefList.erase(it); 
        }  else {
            it++;
        }
   }             
   for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); ) {
        if((*it)->_mergeVisited && ((*it)->_isVisited)) {
            it = _dfsList.erase(it); 
        }  else {
            it++;
        }
   }             
   for(vector<CirGate*>::iterator it = _totalList.begin(); it != _totalList.end(); ){
        if(*it != 0){
            if( (((*it)->_type == "AIG") || ((*it)->_type == "UNDEF")) && (*it)->_mergeVisited  && ((*it)->_isVisited) ){
                #ifdef debug_opt
                cout << "mergeSweeping: " << (*it)->_type << "(" << (*it)->getID() << ") removed..." << endl;
                #endif
                delete *it;
                it = _totalList.erase(it);
            } else if ( ((((*it)->_type == "PI")) && (*it)->_mergeVisited )  && ((*it)->_isVisited) ) { // not visited , means fanout is no usage
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
CirMgr::optimize() {
    
    if(_isSimulated) {
        cout << "Error: circuit has been simulated!! Do \"CIRFraig\" first!!" << endl;
    } else { 
        for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
            if(*it != 0){
               if( ((*it)->_type == "AIG")) {
                    if( (*it)->_faninList[0]->_type == "CONST"  ||  (*it)->_faninList[1]->_type == "CONST") { // Type1 , Type2
                        if( ((*it)->_faninList[0]->_type == "CONST" && !((CirAIGGate*)(*it))->_rhs1_invert) ||   // Type2: use const 0 replace gate
                            ((*it)->_faninList[1]->_type == "CONST" && !((CirAIGGate*)(*it))->_rhs2_invert)  )
                        {
                            merge(_totalList[0], (*it), 2, 0, 0);
                        } else if(((*it)->_faninList[0]->_type == "CONST" && ((CirAIGGate*)(*it))->_rhs1_invert) ) { //Type1: use rhs2 + phase replace gate
                            merge((*it)->_faninList[1], (*it), 1, ((CirAIGGate*)(*it))->_rhs2_invert, 0);
                        } else if(((*it)->_faninList[1]->_type == "CONST" && ((CirAIGGate*)(*it))->_rhs2_invert) ) { //Type1: use rhs1 + phase replace gate
                            merge((*it)->_faninList[0], (*it), 1, ((CirAIGGate*)(*it))->_rhs1_invert, 0);
                        }
                    } else if((*it)->_faninList[0] == (*it)->_faninList[1]) {   // Type3, Type4
                        if( (((CirAIGGate*)(*it))->_rhs1_invert ^ ((CirAIGGate*)(*it))->_rhs2_invert) ) {   //Type4: fanins are same , but phase inverted
                            merge(_totalList[0], (*it), 4, 0, 0);
                        } else if ( ((CirAIGGate*)(*it))->_rhs1_invert & ((CirAIGGate*)(*it))->_rhs2_invert ) { //Type3: phase is "inverted"
                            merge((*it)->_faninList[0], (*it), 3, 1, 0);
                        } else {                                                                                //Type3: two fanin is same
                            merge((*it)->_faninList[0], (*it), 3, 0, 0);
                        }
                    }
               }
               else if( ((*it)->_type == "PO") ) { 
                    ((CirPOGate*)(*it))->setFaninID((*it)->_faninList[0]->getID());
                }  
            }
        }
        CirMgr::mergeSweep();
        for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
            if(*it != 0){
                (*it)->_isVisited = false;
                (*it)->_mergeVisited = false;
            }
        }
        _dfsList.clear(); 
        for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
             myDFS(*it);
        }
        _isStrashed = false;
    }
}
//void
//CirMgr::optimize()
//{
//    int i = 0, j = 0; 
//    int poInv,aigInv;
//    if(_isStrashed) {
//        for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
//            if(*it != 0){
//                (*it)->_mergeVisited = false;
//            }
//        }
//    }
//    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
//        if(*it != 0){
//            //if( ((*it)->_type != "AIG") ) 
//            //    (*it)->_mergeVisited = true;
//            #ifdef debug_opt 
//            cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
//            cout << ", _isVisited = " << (*it)->_isVisited;   
//                if((*it)->_type == "AIG") {
//                        cout << ", _rhs1_inv = " << ((CirAIGGate*)(*it))->_rhs1_invert;
//                        cout << ", _rhs2_inv = " << ((CirAIGGate*)(*it))->_rhs2_invert;
//                }
//                if((*it)->_type == "PO") {
//                        cout << ", _isInvert = " << ((CirPOGate*)(*it))->_isInvert;
//                }
//            cout << endl;
//           cout << "    ---- fanout ----" << endl;
//           for(vector<CirGate*>::const_iterator itG = (*it)->_fanoutList.begin(); itG != (*it)->_fanoutList.end(); itG++) {
//               cout << "    *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID() << endl;
//           }
//           cout << "    ---- fanin ----" << endl;
//           for(vector<CirGate*>::const_iterator itG = (*it)->_faninList.begin(); itG != (*it)->_faninList.end(); itG++) {
//               cout << "    *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID();
//                cout << endl;
//           }
//            cout << endl; 
//            #endif
//            if( ((*it)->_type == "CONST") ) {
//                //(*it)->_mergeVisited = true;
//                #ifdef debug_opt
//                cout << "  I'm CONST , do nothing" << endl; 
//                #endif
//            } else if( ((*it)->_type == "PI") ) {
//                #ifdef debug_opt
//                cout << "  I'm PI , do nothing" << endl; 
//                #endif
//            } else if( ((*it)->_type == "AIG") ) {  // check fanin & fanout
//                if((*it)->_faninList[0]->_type == "CONST") {
//                        (*it)->_mergeVisited = true;
//                        if(((CirAIGGate*)(*it))->_rhs1_invert == 0) {
//                            cout << "Simplifying: " << _totalList[0]->getID()  << 
//                                    " merging " << (*it)->getID() << "..." << endl;
//                        } else  { // const 1 , use other fanin as new fanin
//                            if((*it)->_faninList[1]->_type != "CONST") {
//                                if(((CirAIGGate*)(*it))->_rhs2_invert) {
//                                    cout << "Simplifying: " << (*it)->_faninList[1]->getID()  << 
//                                        " merging !" << (*it)->getID() << "..." << endl;
//                                } else {
//                                    cout << "Simplifying: " << (*it)->_faninList[1]->getID()  << 
//                                        " merging " << (*it)->getID() << "..." << endl;
//                                }
//                                    //cout << "A" << endl;
//                            } else { 
//                                cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                    " merging " << (*it)->getID() << "..." << endl;
//                            }
//                        }
//                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
//                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
//                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
//                                    if(((CirAIGGate*)(*it))->_rhs1_invert == 0) //rhs1 is const , and not invert ===> means const 0 
//                                        (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
//                                    else {                                      // use rhs2 replace fanout's fanin
//                                        (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[1]; 
//                                        if( (*it)->_faninList[1]->_type != "CONST" && (*it)->_fanoutList[i]->_type =="AIG") {
//                                            if(j == 0) {
//                                                aigInv = ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert ^ ((CirAIGGate*)(*it))->_rhs2_invert;
//                                                ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = aigInv;
//                                                //((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = ((CirAIGGate*)(*it))->_rhs2_invert;
//                                            }
//                                            else {
//                                                aigInv = ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert ^ ((CirAIGGate*)(*it))->_rhs2_invert;
//                                                ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = aigInv;
//                                                //((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = ((CirAIGGate*)(*it))->_rhs2_invert;
//                                            }
//                                        }  else if( (*it)->_faninList[1]->_type != "CONST" && (*it)->_fanoutList[i]->_type =="PO") {
//                                            poInv = ((CirPOGate*)(*it)->_fanoutList[i])->getIsInv() ^ ((CirAIGGate*)(*it))->_rhs2_invert;
//                                            ((CirPOGate*)(*it)->_fanoutList[i])->setInvert(poInv);
//                                            //cout << "ans poInv = " << poInv << endl;;
//                                        }
//                                    }
//                                    #ifdef debug_opt
//                                    cout << "=== fanin has constant 0 ===" << endl <<
//                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
//                                    cout << " (*it)->_fanoutList[i] = " << 
//                                            (*it)->_fanoutList[i] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
//                                    #endif
//                                }
//                            }
//                        }
//                } else if((*it)->_faninList[1]->_type == "CONST") {
//                        (*it)->_mergeVisited = true;
//                        if(((CirAIGGate*)(*it))->_rhs2_invert == 0) {
//                            cout << "Simplifying: " << _totalList[0]->getID()  << 
//                                    " merging " << (*it)->getID() << "..." << endl;
//                        } else { // const 1 , use other fanin as new fanin
//                            if( (*it)->_faninList[0]->_type != "CONST") {
//                                if(((CirAIGGate*)(*it))->_rhs1_invert) {
//                                    cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                        " merging !" << (*it)->getID() << "..." << endl;
//                                } else {
//                                    cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                        " merging " << (*it)->getID() << "..." << endl;
//                                }
//                            } else { 
//                                cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                    " merging " << (*it)->getID() << "..." << endl;
//                            }
//                        }
//                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
//                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
//                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
//                                    if(((CirAIGGate*)(*it))->_rhs2_invert == 0) //rhs2 is const , and not invert ===> means const 0 
//                                        (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
//                                    else {                                      // use rhs1 replace fanout's fanin
//                                        (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[0]; 
//                                        if( (*it)->_faninList[0]->_type != "CONST" && (*it)->_fanoutList[i]->_type =="AIG") {
//                                            if(j == 0) {
//                                                aigInv = ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert  ^ ((CirAIGGate*)(*it))->_rhs1_invert;    
//                                                ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = aigInv;
//                                                //((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert = ((CirAIGGate*)(*it))->_rhs1_invert;
//                                            }
//                                            else {
//                                                aigInv = ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert ^ ((CirAIGGate*)(*it))->_rhs1_invert;
//                                                ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = aigInv;
//                                                //((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert = ((CirAIGGate*)(*it))->_rhs1_invert;
//                                            }
//                                            //cout << "ans invert = " << ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert << endl;;
//                                        } else if( (*it)->_faninList[0]->_type != "CONST" && (*it)->_fanoutList[i]->_type =="PO") {
//                                            poInv = ((CirPOGate*)(*it)->_fanoutList[i])->getIsInv() ^ ((CirAIGGate*)(*it))->_rhs1_invert;
//                                            ((CirPOGate*)(*it)->_fanoutList[i])->setInvert(poInv);
//                                            //cout << "ans poInv = " << poInv << endl;;
//                                        }
//                                    }
//                                    #ifdef debug_opt
//                                    cout << "=== fanin 2 has constant 0 ===" << endl <<
//                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
//                                    cout << " (*it)->_fanoutList[i] = " << 
//                                            (*it)->_fanoutList[i] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
//                                    #endif
//                                }
//                            }
//                        }
//                } else if((*it)->_faninList[0] == (*it)->_faninList[1]) {
//                    if( ((CirAIGGate*)(*it))->_rhs1_invert && ((CirAIGGate*)(*it))->_rhs2_invert ) {   // both are inverted
//                        (*it)->_mergeVisited = true;
//                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                " merging !" << (*it)->getID() << "..." << endl;
//                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
//                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
//                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
//                                    (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[0];    // put gate , replace origin it
//                                    #ifdef debug_opt
//                                        cout << "  (*it)->_faninList[0]->ID = " << (*it)->_faninList[0]->getID() <<
//                                                ", (*it)->_faninList[1]->ID = " << (*it)->_faninList[1]->getID() << endl;
//                                    #endif
//                                    if((*it)->_fanoutList[i]->_type == "AIG"){
//                                        if(j == 0) ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert ^= 1;
//                                        else       ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert ^= 1;
//                                    }   else if((*it)->_fanoutList[i]->_type == "PO") {
//                                        poInv = ((CirPOGate*)(*it)->_fanoutList[i])->getIsInv() ^ 1;
//                                        ((CirPOGate*)(*it)->_fanoutList[i])->setInvert(poInv);
//                                    }
//                                    #ifdef debug_opt
//                                    cout << "  i = " << i << ", j = " << j << ", (*it)->_fanoutList.size() = " << 
//                                            (*it)->_fanoutList.size() << endl;
//                                    cout << "(CirAIGGate*)(*it))->_rhs1_invert = " << ((CirAIGGate*)(*it))->_rhs1_invert 
//                                         << ", (CirAIGGate*)(*it))->_rhs2_invert = " << ((CirAIGGate*)(*it))->_rhs2_invert
//                                         << endl;
//                                    cout << "=== two fanin is same gate , both are inverted ===" << endl <<
//                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j]->_type <<
//                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
//                                    cout << " (*it)->_fanoutList[i] = " << 
//                                            (*it)->_fanoutList[i] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
//                                    #endif
//                                }
//                            }
//                        }
//                    } else if( !((CirAIGGate*)(*it))->_rhs1_invert && !((CirAIGGate*)(*it))->_rhs2_invert ) { //both are same
//                        (*it)->_mergeVisited = true;
//                        cout << "Simplifying: " << (*it)->_faninList[0]->getID()  << 
//                                " merging " << (*it)->getID() << "..." << endl;
//                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
//                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
//                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
//                                    (*it)->_fanoutList[i]->_faninList[j] = (*it)->_faninList[0];    // put gate , replace origin it
//                                    #ifdef debug_opt
//                                        cout << "  (*it)->_faninList[0]->ID = " << (*it)->_faninList[0]->getID() <<
//                                                ", (*it)->_faninList[1]->ID = " << (*it)->_faninList[1]->getID() << endl;
//                                    #endif
//                                    if((*it)->_fanoutList[i]->_type == "AIG"){
//                                        if(j == 0) ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs1_invert ^= 0;
//                                        else       ((CirAIGGate*)(*it)->_fanoutList[i])->_rhs2_invert ^= 0;
//                                    } else if((*it)->_fanoutList[i]->_type == "PO"){
//                                        poInv = ((CirPOGate*)(*it)->_fanoutList[i])->getIsInv() ^ 0;
//                                        ((CirPOGate*)(*it)->_fanoutList[i])->setInvert(poInv);
//                                    }
//                                    #ifdef debug_opt
//                                    cout << "  i = " << i << ", j = " << j << ", (*it)->_fanoutList.size() = "  <<
//                                            (*it)->_fanoutList.size() << endl;
//                                    cout << "(CirAIGGate*)(*it))->_rhs1_invert = " << ((CirAIGGate*)(*it))->_rhs1_invert 
//                                         << ", (CirAIGGate*)(*it))->_rhs2_invert = " << ((CirAIGGate*)(*it))->_rhs2_invert
//                                         << endl;
//                                    cout << "=== two fanin is same gate ===" << endl <<
//                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j]->_type <<
//                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
//                                    cout << " (*it)->_fanoutList[i] = " << 
//                                            (*it)->_fanoutList[i] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
//                                    #endif
//                                }
//                            }
//                        }
//                    } else {    // one is inverted , one is origin
//                        (*it)->_mergeVisited = true;
//                        cout << "Simplifying: " << _totalList[0]->getID()  << 
//                                " merging " << (*it)->getID() << "..." << endl;
//                        for(i = 0; i < (*it)->_fanoutList.size(); ++i) {
//                            for(j = 0; j < (*it)->_fanoutList[i]->_faninList.size(); ++j) {
//                                if ((*it)->_fanoutList[i]->_faninList[j] == (*it)) {
//                                    (*it)->_fanoutList[i]->_faninList[j] = _totalList[0];    // put const 0 , replace it
//                                    #ifdef debug_opt
//                                    cout << "=== two fanin is same gate, but phase different ===" << endl <<
//                                            " (*it)->_fanoutList[i]->_faninList[j] = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_faninList[j]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->_faninList[j]->getID()  << endl;
//                                    cout << " (*it)->_fanoutList[i] = " << 
//                                            (*it)->_fanoutList[i] << ",  type = " << 
//                                            (*it)->_fanoutList[i]->_type << 
//                                            ",  ID = " << (*it)->_fanoutList[i]->getID()  << endl;
//                                    #endif
//                                }
//                            }
//                        }
//                    }
//                } else if(1) {
//
//                }
//            } else if( ((*it)->_type == "PO") ) {
//                ((CirPOGate*)(*it))->setFaninID((*it)->_faninList[0]->getID());
//                #ifdef debug_opt
//       //         cout << "PO fanin ID = " << ((CirPOGate*)(*it))->_faninList[0]->getID() << endl;
//       //         cout << "PO fanin's fanin1 type = " << ((CirAIGGate*)(*it)->_faninList[0])->_faninList[0]->_type << endl;
//       //         cout << "PO fanin's fanin2 type = " << ((CirAIGGate*)(*it)->_faninList[0])->_faninList[1]->_type << endl;
//       //         cout << "PO fanin's fanin1 invert = " << ((CirAIGGate*)(*it)->_faninList[0])->_rhs1_invert << endl;
//       //         cout << "PO fanin's fanin2 invert = " << ((CirAIGGate*)(*it)->_faninList[0])->_rhs2_invert << endl;
//       //         if(((CirAIGGate*)(*it)->_faninList[0])->_faninList[0]->_type != "CONST" ){//&& 
//       //           // ((CirAIGGate*)(*it)->_faninList[0])->_faninList[1]->_type =="CONST" && ((CirAIGGate*)(*it)->_faninList[0])->_rhs2_invert==1 ) {
//       //             //if( ((CirAIGGate*)(*it)->_faninList[0])->_rhs1_invert  )   ((CirPOGate*)(*it))->setInvert(1);                    
//       //             //else                                                       ((CirPOGate*)(*it))->setInvert(0);
//       //             cout << "Invert Check = " << ((CirPOGate*)(*it))->_faninList[0]->getID() << endl;
//       //             //poInv = ((CirPOGate*)(*it))->getIsInv() ^ ((CirAIGGate*)(*it)->_faninList[0])->_rhs1_invert;
//       //             //((CirPOGate*)(*it))->setInvert(poInv);
//       //         }
//       //         if(((CirAIGGate*)(*it)->_faninList[0])->_faninList[1]->_type != "CONST" ) {
//       //         //    if( ((CirAIGGate*)(*it)->_faninList[0])->_rhs1_invert  )   ((CirPOGate*)(*it))->setInvert(1);
//       //         //    else                                                       ((CirPOGate*)(*it))->setInvert(0);
//       //         }
//       //         cout << "  I'm PO , do nothing" << endl; 
//                #endif
//            } else if ( ((*it)->_type == "UNDEF")) {
//                #ifdef debug_opt
//                cout << "  I'm UNDEF , do nothing" << endl; 
//                #endif
//            }
//        }   
//    }
////    cout << "Opt OK " << endl;
////    for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
////    //     optDFS(*it);
////         cout << "OptDFS OK " << endl;
////     }
//
//    CirMgr::mergeSweep();
//    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
//        if(*it != 0){
//            (*it)->_isVisited = false;
//            (*it)->_mergeVisited = false;
//        }
//    }
//    _dfsList.clear(); 
//    for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
//         myDFS(*it);
//    }
//    _isStrashed = false;
//    #ifdef debug_opt
//    cout << " ===== merging done ===== " << endl;
//    for(vector<CirGate*>::iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
//        if(*it != 0){ 
//            cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
//            cout << ", _isVisited = " << (*it)->_isVisited;   
//                if((*it)->_type == "AIG") {
//                        cout << ", _rhs1_inv = " << ((CirAIGGate*)(*it))->_rhs1_invert;
//                        cout << ", _rhs2_inv = " << ((CirAIGGate*)(*it))->_rhs2_invert;
//                }
//                if((*it)->_type == "PO") {
//                        cout << ", _isInvert = " << ((CirPOGate*)(*it))->_isInvert;
//                }
//            cout << endl;
//           cout << "    ---- fanout ----" << endl;
//           for(vector<CirGate*>::const_iterator itG = (*it)->_fanoutList.begin(); itG != (*it)->_fanoutList.end(); itG++) {
//               cout << "    *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID() << endl;
//           }
//           cout << "    ---- fanin ----" << endl;
//           for(vector<CirGate*>::const_iterator itG = (*it)->_faninList.begin(); itG != (*it)->_faninList.end(); itG++) {
//               cout << "    *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID();
//                cout << endl;
//           }
//            cout << endl; 
//        } 
//    }
//    #endif
//}

//use gateA replace gateB
void CirMgr::merge(CirGate* gateA,CirGate* gateB, int mergeMethod, bool mergeInv, int cmdType){
    int i,j;
    int poInv, aigInv;
    gateB->_mergeVisited = true; // gateB will be removed
    if(cmdType == 0)
        mergeMsg("Simplifying: ", gateA->getID(), mergeInv, gateB->getID());
    else if(cmdType == 1)
        mergeMsg("Strashing: ", gateA->getID(), mergeInv, gateB->getID());
    else if(cmdType == 2)
        mergeMsg("Fraig: ", gateA->getID(), mergeInv, gateB->getID());
    //cout << "Simplifying: " << gateA->getID() << " merging " << gateB->getID() << "..." << endl;
    //check gateB's fanout
    for(i = 0; i < gateB->_fanoutList.size(); ++i) {
        for(j = 0; j < gateB->_fanoutList[i]->_faninList.size(); ++j) { // actual fanin.size() should equal 2
            if(gateB->_fanoutList[i]->_faninList[j] == gateB) {
                gateB->_fanoutList[i]->_faninList[j] = gateA;
                if(gateB->_fanoutList[i]->_type == "AIG") {
                    if(j==0) ((CirAIGGate*)gateB->_fanoutList[i])->_rhs1_invert ^= mergeInv;
                    else     ((CirAIGGate*)gateB->_fanoutList[i])->_rhs2_invert ^= mergeInv;  
                } else if(gateB->_fanoutList[i]->_type == "PO") {
                    ((CirPOGate*)gateB->_fanoutList[i])->_isInvert ^= mergeInv;
                }
            } 
        }
        gateA->_fanoutList.push_back(gateB->_fanoutList[i]);
    }
    //check gateB's fanin
    for(i = 0; i < gateB->_faninList.size(); ++i) { // actual fanin.size() == 2
        for(j = 0; j < gateB->_faninList[i]->_fanoutList.size(); ++j) { 
            if( gateB->_faninList[i]->_fanoutList[j] == gateB) { 
                gateB->_faninList[i]->_fanoutList.erase(gateB->_faninList[i]->_fanoutList.begin()+j); 
            }      
            //gateB->_faninList[i]->_fanoutList.push_back(gateB->_fanoutList[j]);
           // if(gateB->_faninList[i]->_fanoutList[j] == gateB) {
           //     //gateB->_faninList[i]->_fanoutList.erase(gateB->_faninList[i]->_fanoutList.begin()+j); 
           //     //gateB->_faninList[i]->_fanoutList.push_back(gateA); 
           //     
           // } 
        }
        //gateB->_faninList[i]->_fanoutList.push_back(gateA);
    }
    //check gateA's fanout match (gateB's fanout)'s fanin
    for(i = 0; i < gateA->_fanoutList.size(); ++i) {
    
    }
}
void
CirMgr::mergeMsg(string mergeType,int mergeID,bool mergeIsInv,int removeID){
    string invSymbol;
    if(mergeIsInv) invSymbol = "!";
    else           invSymbol = "";
    cout << mergeType << mergeID  << " merging "<< invSymbol << removeID<< "..." << endl;

}
/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

