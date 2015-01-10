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
}

/***************************************************/
/*   Private member functions about optimization   */
/***************************************************/

