/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

//#define debug_inout
//#define debug_DFS
using namespace std;

// TODO: Implement memeber functions for class CirMgr

/*******************************/
/*   Global variable and enum  */
/*******************************/
CirMgr* cirMgr = 0;

enum CirParseError {
   EXTRA_SPACE,
   MISSING_SPACE,
   ILLEGAL_WSPACE,
   ILLEGAL_NUM,
   ILLEGAL_IDENTIFIER,
   ILLEGAL_SYMBOL_TYPE,
   ILLEGAL_SYMBOL_NAME,
   MISSING_NUM,
   MISSING_IDENTIFIER,
   MISSING_NEWLINE,
   MISSING_DEF,
   CANNOT_INVERTED,
   MAX_LIT_ID,
   REDEF_GATE,
   REDEF_SYMBOLIC_NAME,
   REDEF_CONST,
   NUM_TOO_SMALL,
   NUM_TOO_BIG,

   DUMMY_END
};

/**************************************/
/*   Static varaibles and functions   */
/**************************************/
static unsigned lineNo = 0;  // in printint, lineNo needs to ++
static unsigned colNo  = 0;  // in printing, colNo needs to ++
static char buf[1024];
static string errMsg;
static int errInt;
static CirGate *errGate;

static bool
parseError(CirParseError err)
{
   switch (err) {
      case EXTRA_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Extra space character is detected!!" << endl;
         break;
      case MISSING_SPACE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing space character!!" << endl;
         break;
      case ILLEGAL_WSPACE: // for non-space white space character
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal white space char(" << errInt
              << ") is detected!!" << endl;
         break;
      case ILLEGAL_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal "
              << errMsg << "!!" << endl;
         break;
      case ILLEGAL_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Illegal identifier \""
              << errMsg << "\"!!" << endl;
         break;
      case ILLEGAL_SYMBOL_TYPE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Illegal symbol type (" << errMsg << ")!!" << endl;
         break;
      case ILLEGAL_SYMBOL_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Symbolic name contains un-printable char(" << errInt
              << ")!!" << endl;
         break;
      case MISSING_NUM:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Missing " << errMsg << "!!" << endl;
         break;
      case MISSING_IDENTIFIER:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing \""
              << errMsg << "\"!!" << endl;
         break;
      case MISSING_NEWLINE:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": A new line is expected here!!" << endl;
         break;
      case MISSING_DEF:
         cerr << "[ERROR] Line " << lineNo+1 << ": Missing " << errMsg
              << " definition!!" << endl;
         break;
      case CANNOT_INVERTED:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": " << errMsg << " " << errInt << "(" << errInt/2
              << ") cannot be inverted!!" << endl;
         break;
      case MAX_LIT_ID:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Literal \"" << errInt << "\" exceeds maximum valid ID!!"
              << endl;
         break;
      case REDEF_GATE:
         cerr << "[ERROR] Line " << lineNo+1 << ": Literal \"" << errInt
              << "\" is redefined, previously defined as "
              << errGate->getTypeStr() << " in line " << errGate->getLineNo()
              << "!!" << endl;
         break;
      case REDEF_SYMBOLIC_NAME:
         cerr << "[ERROR] Line " << lineNo+1 << ": Symbolic name for \""
              << errMsg << errInt << "\" is redefined!!" << endl;
         break;
      case REDEF_CONST:
         cerr << "[ERROR] Line " << lineNo+1 << ", Col " << colNo+1
              << ": Cannot redefine const (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_SMALL:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too small (" << errInt << ")!!" << endl;
         break;
      case NUM_TOO_BIG:
         cerr << "[ERROR] Line " << lineNo+1 << ": " << errMsg
              << " is too big (" << errInt << ")!!" << endl;
         break;
      default: break;
   }
   return false;
}

/**************************************************************/
/*   Justin defined member functions                          */
/**************************************************************/
void CirMgr::cleanGate() {
   for(vector<CirGate*>::const_iterator it = _totalList.begin(); it != _totalList.end(); it++){
        if((*it) != 0 && ((*it)->_type != "UNDEF") ) { 
            delete *it;
        }   
   }
   for(vector<CirUndefGate*>::iterator it = _undefList.begin(); it != _undefList.end(); ++it ) { 
      if((*it) != 0 ) { 
          delete *it;
      }   
   }      
}
void CirMgr::aagRecorder(string  token, size_t countLine, size_t beginAddr, size_t order = -1)
{
        if(countLine == 1) {
            if(order == 0)
                aagDebugPrint(token, countLine, beginAddr,token);
            else if(order == 1) {
                aagDebugPrint(token, countLine, beginAddr, "M");
                M = atoi(token.c_str());
                _totalList.resize(M+1); // init Vector Size
                CirConstGate* conGate = new CirConstGate(0,0);
                _totalList[0] = conGate;  //store const Gate in tail
                conGate->_type = "CONST";
            }  else if(order == 2) {
                aagDebugPrint(token, countLine, beginAddr, "I");
                I = atoi(token.c_str());
            }  else if(order == 3) {
                aagDebugPrint(token, countLine, beginAddr, "L");
                L = atoi(token.c_str());
            }  else if(order == 4) {
                aagDebugPrint(token, countLine, beginAddr, "O");
                O = atoi(token.c_str());
                _totalList.resize(M+O+1); // init Vector Size
            }  else if(order == 5) {
                aagDebugPrint(token, countLine, beginAddr, "A");
                A = atoi(token.c_str());
            }
        } else if(countLine >=2){  //read PI
           // int tmpLine = countLine;
            if(countLine <= I+1) {
                aagDebugPrint(token, countLine, beginAddr, "PI" , 1);
                CirPIGate* pi = new CirPIGate(countLine,atoi(token.c_str())/2 );
                _piList.push_back(pi);
                _totalList[atoi(token.c_str())/2] = pi;
                pi->_type = "PI";
               // cout << "_totalList.size() = " << _totalList.size() << endl; 
               // cout << "_totalList.capacity() = " << _totalList.capacity() << endl; 
                //_totalList.insert(_totalList.begin()+(atoi(token.c_str())/2),pi);
                //cout << "pi = " << pi << endl;
            } else if(countLine <= O+I+1) { // read PO
                    aagDebugPrint(token, countLine, beginAddr, "PO" , 1 );
                    CirPOGate* po = new CirPOGate(countLine, M+_poList.size()+1, atoi(token.c_str())/2); //(line,ID,faninID)
                    if(atoi(token.c_str())%2 == 1) {po->_isInvert = 1;}
                    _poList.push_back(po);
                    //cout << "faninID = " << atoi(token.c_str())/2 << endl;
                    //_totalList.resize(M+_poList.size()+1);
                    _totalList[M+_poList.size()] = po;
                    po->_type = "PO";
                    //  cout << "PO _totalList.size() = " << _totalList.size() << endl; 
                    //  cout << "PO _totalList.capacity() = " << _totalList.capacity() << endl; 
                    //  if(_totalList.capacity()==16) cout << "_totalList[16] = "  <<_totalList[16] << endl;
            } else if(countLine <= A+O+I+1) {
                    aagDebugPrint(token, countLine, beginAddr, "aig" , 1 );
                    string tmpToken = token;
                    size_t m;
                    size_t n = newMyStrGetTok(tmpToken, token, m); 
                    //cout << "token = "  << token << endl;
                    CirAIGGate* aig = new CirAIGGate(countLine,atoi(token.c_str())/2 );
                    _aigList.push_back(aig);
                    _totalList[atoi(token.c_str())/2] = aig;
                    aig->_type = "AIG";
                    n = newMyStrGetTok(tmpToken, token, m , n);
                    int rhs1 = 0 ,rhs2 = 0, count = 1; 
                    while(token.size()){    
                        //cout << "   while token = "  << token << endl;
                        if(count == 1) rhs1 = atoi(token.c_str());
                        else if(count == 2) rhs2 = atoi(token.c_str());
                        ++count ;
                        n = newMyStrGetTok(tmpToken, token, m , n); 
                    }

                    
                    if(_totalList[rhs1/2] != 0) {         // this Gate is defined
                        aig->_faninList.push_back(_totalList[rhs1/2]);
                        _totalList[rhs1/2]->_fanoutList.push_back(aig);
                        if(rhs1%2 == 1) aig->_rhs1_invert = 1;
                    } else {
                        CirUndefGate* undef = new CirUndefGate(0, rhs1/2);
                        _undefList.push_back(undef);
                        aig->_faninList.push_back(undef);
                        undef->_fanoutList.push_back(aig);
                        undef->_type = "UNDEF";
                        if(rhs1%2 == 1) aig->_rhs1_invert = 1;
                    }
                    if(_totalList[rhs2/2] != 0) {         // this Gate is defined
                        aig->_faninList.push_back(_totalList[rhs2/2]);
                        _totalList[rhs2/2]->_fanoutList.push_back(aig);
                        if(rhs2%2 == 1) aig->_rhs2_invert = 1;
                    } else {
                        CirUndefGate* undef = new CirUndefGate(0, rhs2/2);
                        _undefList.push_back(undef);
                        aig->_faninList.push_back(undef);
                        undef->_fanoutList.push_back(aig);
                        undef->_type = "UNDEF";
                        if(rhs2%2 == 1) aig->_rhs2_invert = 1;
                    }
                    
            }  else {        // symbolic
                 string tmpToken = token;
                 size_t m;
                 size_t n = newMyStrGetTok(tmpToken, token, m);
                 int count = 1 , index = -1;
                 bool flagIO = false;   // Input = false ,  output = true;
                 string subToken;
                 //while(token.size()){
                     if(count == 1)  {
                        if(token[0] == 'o')  flagIO = true;
                        else               flagIO = false;
                        subToken = token.substr(1);
                        index = atoi(subToken.c_str());
                        //cout << "index = " << index; 
                        ++count ;
                        //n = newMyStrGetTok(tmpToken, token, m , n);
                     }
                     
                    if(count == 2) {
                        token = tmpToken.substr(n+1);
                        //cout << "token =" << token << endl; 
                        if(flagIO) { _poList[index]->_name = token;}
                        else       { _piList[index]->_name = token;} 
                     }
                     //++count ;
                     //n = newMyStrGetTok(tmpToken, token, m , n);
                 //}
            } 

        }
}
/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
bool
CirMgr::readCircuit(const string& fileName)
{
   string line;
   size_t countLine = 0;
   ifstream myfile;
   string token;
   //cout << "fileName = " << fileName << endl; 
   myfile.open(fileName.c_str(),ios::in);
   if(myfile.is_open()) {
     while(getline(myfile,line)) {
        ++countLine;
        //cout << line << endl;
        size_t m = 0; //record beginAddr
        size_t n , order = -1; 
       // cout << "n = " << n << endl;
        if(countLine == 1){
            n = newMyStrGetTok(line, token, m);
            while(token.size()){
              order++;
              //cout << "token = " << token << endl;
              //cout << "token = " << token << ",  m+1 = " << m+1 << endl;
              aagRecorder(token,countLine,m+1,order);
              n = newMyStrGetTok(line, token, m,n);
              //cout << "n = " << n << endl;
            }
        } else {
            if(line=="c") { 
                //cout << "line = " << countLine << " , read to 'c' , break ";
                break; 
            }
            aagRecorder(line,countLine,m+1);
        }
     }
    
     for(vector<CirUndefGate*>::iterator it = _undefList.begin(); it != _undefList.end(); ) {
        //cout << "undef ID = " << (*it)->getID() << endl;
        if(_totalList[(*it)->getID()] != 0) {
            
            (*it)->_type = _totalList[(*it)->getID()]->_type;
            _totalList[(*it)->getID()]->_fanoutList.push_back((*it)->_fanoutList[0]); // assign fanout to Correct gate
            
            if((*it)->_fanoutList[0]->_faninList[0]->getID() == (*it)->getID()) // rhs1 is record undef Gate , replace defined Gate
                (*it)->_fanoutList[0]->_faninList[0] = _totalList[(*it)->getID()];// assign next gate it's fanin
            if((*it)->_fanoutList[0]->_faninList[1]->getID() == (*it)->getID()) // rhs2 is record undef Gate , replace defined Gate  
               (*it)->_fanoutList[0]->_faninList[1] = _totalList[(*it)->getID()];
              
             //cout << "ID = " << (*it)->getID() <<  ", type = " << (*it)->_type << endl;
             delete *it;
             it = _undefList.erase(it);         
        } else {
            _totalList[(*it)->getID()] = (*it);
            ++it;
        }
     }
     
     for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end();it++ ) { //add faninList to PO
        if(_totalList[(*it)->_faninID] != 0) {
            (*it)->_faninList.push_back(_totalList[(*it)->_faninID]);
            #ifdef debug_inout
                cout << "put ID = \"" <<  (*it)->getID() << "\"  to AIG output" << endl;
            #endif
            _totalList[(*it)->_faninID]->_fanoutList.push_back((*it));
        } else {    // *** fraig project add from cirGate.cpp
            CirUndefGate* undef = new CirUndefGate(0,((*it)->getFaninID())); //only for recur issue 
            undef->_type = "UNDEF";
            (*it)->_faninList.push_back(undef);
            _undefList.push_back(undef);
        }
     }
    
     
//     reverse(_poList.begin(), _poList.end());
     for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
         myDFS(*it);
     } 
//     reverse(_poList.begin(), _poList.end());
     //cout << "_piList.size() = " << _piList.size() << endl;
     //cout << "M = " << M << ", I = " << I << ", L = " << L<< " , O = " << O<< ", A = " << A << endl<<endl;
     myfile.close();
   } else  {
        cout  << "read failure" << endl;
        return false; 
   }
   return true;
}

/**********************************************************/
/*   class CirMgr member functions for circuit printing   */
/**********************************************************/
/*********************
Circuit Statistics
==================
  PI          20
  PO          12
  AIG        130
------------------
  Total      162
*********************/
void
CirMgr::printSummary() const
{
    cout << endl;
    cout << "Circuit Statistics" << endl;
    cout << "==================" << endl;
    cout << "  PI   " << right << setw(9) << _piList.size() << endl; 
    cout << "  PO   " << right << setw(9) << _poList.size() << endl; 
    cout << "  AIG  " << right << setw(9) << _aigList.size() << endl; 
    cout << "------------------" << endl;
    cout << "  Total  " << right << setw(7) << _piList.size() + _poList.size() + _aigList.size() << endl;    
}

void
CirMgr::printNetlist() const
{
   #ifdef debug_inout
   int bugCount = 0;
   int totalFanin = 0;
   int totalFanout = 0;
   for(vector<CirGate*>::const_iterator it = _totalList.begin(); it != _totalList.end(); it++){
      cout << "bug count = " << bugCount << ",  *it = " <<(*it);
      if(*it != 0){
          cout << ", _isVisited = " << (*it)->_isVisited;
          if((*it)->_type == "AIG") {
                            cout << ", _rhs1_inv = " << ((CirAIGGate*)(*it))->_rhs1_invert;
                            cout << ", _rhs2_inv = " << ((CirAIGGate*)(*it))->_rhs2_invert;
          } 
          if((*it)->_type == "PO") {
                            cout << ", _isInvert = " << ((CirPOGate*)(*it))->_isInvert;
          }  
      }   
      cout << endl;
      ++bugCount;
      if(*it != 0){
           totalFanin += (*it)->_faninList.size();
           totalFanout += (*it)->_fanoutList.size();
           // #ifdef debug_inout
           cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it)<< endl;
           cout << "  ---- fanout ----" << endl;
           //cout << "(totalList[ID] =" << _totalList[(*it)->getID()]->getID() << ")"; 
           for(vector<CirGate*>::const_iterator itG = (*it)->_fanoutList.begin(); itG != (*it)->_fanoutList.end(); itG++) {
               cout << "  *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID() << endl;
           }
           cout << "  ---- fanin ----" << endl;
           //cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type << "  (fanin)"<< endl;
           //cout << "(totalList[ID] =" << _totalList[(*it)->getID()]->getID() << ")"; 
           for(vector<CirGate*>::const_iterator itG = (*it)->_faninList.begin(); itG != (*it)->_faninList.end(); itG++) {
               cout << "  *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID() << endl;
           }
            cout << endl;
           // #endif
      }
             //cout << " IP" << endl ;
   }
   cout << "*********  totalFanin = " << totalFanin << " ,  totalFanout = " <<  totalFanout << "  ***********" << endl;
   #endif
   #ifdef debug_DFS
   cout << "=========   print all _totalList   ========= "  << endl;
   for(vector<CirGate*>::const_iterator it = _totalList.begin(); it != _totalList.end(); it++){
        if(*it != 0){
           cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            cout << ", _isVisited = " << (*it)->_isVisited  << endl;          
        }
   }
   cout << "=========   print all _dfsList   ========= "  << endl;
   for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){
           cout << "ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
            cout << ", _isVisited = " << (*it)->_isVisited  << endl;          
        }
   }
   #endif
   size_t count = 0;
   cout << endl;
   for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if( (*it)->_type == "UNDEF") //undef Gate
            continue;
        cout << "["  << count << "] ";
        cout << (*it)->_type;
        if((*it)->_type == "CONST") {
            if(((CirConstGate*)(*it))->_isInvert) { cout << "1";}
            else                                { cout << "0";}
        }
        else if((*it)->_type == "PI") {
            cout << "  " << (*it)->getID();
            if((*it)->_name != "")  cout << " (" << (*it)->_name << ")";
        } else if ((*it)->_type == "PO") {
            cout << "  " << (*it)->getID();
            if((*it)->_faninList[0]->_type == "UNDEF" )  { cout << " *"; }
            else                               { cout << " " ; }
            if(((CirPOGate*)(*it))->_isInvert) { cout << "!"  << ((CirPOGate*)(*it))->_faninID; }
            else         { cout << ""  << ((CirPOGate*)(*it))->_faninID;}

            if((*it)->_name != "")  cout << " (" << (*it)->_name << ")";
        } else if  ((*it)->_type == "AIG"){
             //cout << " IP" ;
             cout << " " << (*it)->getID();
             //rhs1 
             if((*it)->_faninList[0]->_type == "UNDEF")  { cout << " *"; }
             else                                   { cout << " " ; }
             if(((CirAIGGate*)(*it))->_rhs1_invert) { cout << "!"  << ((CirAIGGate*)(*it))->_faninList[0]->getID(); } 
             else                                   { cout << ((CirAIGGate*)(*it))->_faninList[0]->getID(); }
             //rhs2
             if((*it)->_faninList[1]->_type == "UNDEF")  { cout << " *"; }
             else                                   { cout << " " ; }
             if(((CirAIGGate*)(*it))->_rhs2_invert) { cout << "!"  << ((CirAIGGate*)(*it))->_faninList[1]->getID(); } 
             else                                   { cout << ((CirAIGGate*)(*it))->_faninList[1]->getID(); }
        }
        cout << endl;
        ++count;
   }
 //  for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++){
 //       myDFS(*it);
 //  } 
}

void CirMgr::myDFS(CirGate* gate){
    
    gate->_isVisited = true;
    //reverse(gate->_faninList.begin(), gate->_faninList.end());
    for(vector<CirGate*>::const_iterator it = gate->_faninList.begin(); it != gate->_faninList.end(); it++)
    {
        if((*it)->_isVisited == false)  // this gate(node) , was not be visted
        {
            myDFS((*it));
        }
    }   
    _dfsList.push_back(gate);
    #ifdef debug_DFS
    cout << "DFS search now is in this gate :   " << gate << " , gate ID = " << gate->getID() << " , type = " << gate->_type << endl;
    #endif
    //reverse(gate->_faninList.begin(), gate->_faninList.end());
}
void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++){
    cout << " " <<  (*it)->getID();
    //cout << "(totalList[ID] =" << _totalList[(*it)->getID()]->getID() << ")"; 
   }
   //cout << " " <<  _piList[0]->getID();
   //cout << " " <<  _piList[1]->getID();
          //id = (*it)->_ID;

   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++)
    cout << " " <<  (*it)->getID();
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
    vector<int> floatingGate;
    vector<int> notUsedGate;
    for(vector<CirAIGGate*>::const_iterator it = _aigList.begin(); it != _aigList.end(); it++) {
        if((*it) != 0) {
          if((*it)->_faninList.size() !=2){
                cout << "fanin size error " << endl;
               // cout << "fanin size =  " << (*it)->_faninList.size()<<  endl;
               // cout << "fanout size =  " << (*it)->_fanoutList.size()<<  endl;
                floatingGate.push_back((*it)->getID());
          } else if((*it)->_faninList.size() ==2) { // if input had undef Gate
                #ifdef debug_inout
                     cout << "(*it)->getID() = " << (*it)->getID() << endl;
                     cout << "(*it)->_faninList[0]->_type = " << (*it)->_faninList[0]->_type << endl;
                     cout << "(*it)->_faninList[1]->_type = " << (*it)->_faninList[1]->_type << endl;
                #endif
                if((*it)->_faninList[0]->_type == "UNDEF") floatingGate.push_back((*it)->getID());
                else if ((*it)->_faninList[1]->_type == "UNDEF") floatingGate.push_back((*it)->getID());
          } 
          // check notUsedGate
          if((*it)->_fanoutList.size() == 0){
                notUsedGate.push_back((*it)->getID());
          }
        //  else if((*it)->_isVisited == false) {
        //        floatingGate.push_back((*it)->getID());
        //  }
             // cout << " " << (*it)->getID();
        }
    }
     
    for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++) {
        if((*it) != 0) {
          if((*it)->_fanoutList.size() ==0){
                 notUsedGate.push_back((*it)->getID());
          }
        }
    }
    for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++) {
        if((*it) != 0) {
          if((*it)->_faninList.size() ==0){
                 floatingGate.push_back((*it)->getID());
          } else {
                 if((*it)->_faninList[0]->_type == "UNDEF") floatingGate.push_back((*it)->getID());
          } 
        } 
    }
        
    if(!floatingGate.empty()) {
        sort( floatingGate.begin(), floatingGate.end());
         cout << "Gates with floating fanin(s):" ;
        for(vector<int>::iterator it = floatingGate.begin(); it != floatingGate.end(); it++) {
         cout << " " << *it; 
        }
        cout << endl;
    }
    if(!notUsedGate.empty()) {
        sort( notUsedGate.begin(), notUsedGate.end());
        cout << "Gates defined but not used  :"; 
        for(vector<int>::iterator it = notUsedGate.begin(); it != notUsedGate.end(); it++) {
         cout << " " << *it; 
        }
        cout << endl;
    }
}

void
CirMgr::printFECPairs() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
        int optA = 0;
        ostringstream  tmpOutfile;
        // use ostring record AIG output
        for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++) {
            if((*it) != 0) {
                if((*it)->_type == "AIG") {
                    if((*it)->_isVisited){
                        tmpOutfile << (*it)->getID()*2 << " ";
                        if(((CirAIGGate*)(*it))->_rhs1_invert)  tmpOutfile << (*it)->_faninList[0]->getID()*2+1 << " ";
                        else   tmpOutfile << (*it)->_faninList[0]->getID()*2 << " ";
                        
                        if(((CirAIGGate*)(*it))->_rhs2_invert)  tmpOutfile << (*it)->_faninList[1]->getID()*2+1;
                        else   tmpOutfile << (*it)->_faninList[1]->getID()*2;
                        tmpOutfile << endl;
                        ++optA;
                    }
                }
            }
        }
        outfile << "aag " << M << " "<< I << " 0 " <<  O << " " << optA  << endl;
        for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++) {
            if((*it) != 0) {
                outfile << (*it)->getID()*2 << endl;
            }
        }
        for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++) {
            if((*it) != 0) {
                if((*it)->_isInvert)  outfile << (*it)->_faninID*2+1 << endl;
                else  outfile << (*it)->_faninID*2 << endl;
            }
        }
        outfile << tmpOutfile.str();
        int symIdx = 0;
        for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++) {
            if((*it) != 0) {
                 if((*it)->_name != ""){
                     outfile << "i" << symIdx << " " << (*it)->_name << endl;
                     ++symIdx;
                 }
            }
        }
        symIdx = 0;
        for(vector<CirPOGate*>::const_iterator it = _poList.begin(); it != _poList.end(); it++) {
            if((*it) != 0) {
                 if((*it)->_name != ""){
                     outfile << "o" << symIdx << " " << (*it)->_name << endl;
                     ++symIdx;
                 }
            }
        }       
        outfile << "c" << endl;
        outfile << "AAG output by Chung-Yang (Ric) Huang" << endl;
}

