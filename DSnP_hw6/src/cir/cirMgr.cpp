/****************************************************************************
  FileName     [ cirMgr.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir manager functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdio>
#include <ctype.h>
#include <cassert>
#include <cstring>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

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
enum Gates{ePI, ePO};
/**************************************************************/
/*   class CirMgr member functions for circuit construction   */
/**************************************************************/
void CirMgr::aagRecorder(string  token, size_t countLine, size_t beginAddr)
{
        if(countLine == 1) {
            if(beginAddr == 1)
                aagDebugPrint(token, countLine, beginAddr,token);
            else if(beginAddr == 5) {
                aagDebugPrint(token, countLine, beginAddr, "M");
                M = atoi(token.c_str());
            }  else if(beginAddr == 7) {
                aagDebugPrint(token, countLine, beginAddr, "I");
                I = atoi(token.c_str());
            }  else if(beginAddr == 9) {
                aagDebugPrint(token, countLine, beginAddr, "L");
                L = atoi(token.c_str());
            }  else if(beginAddr == 11) {
                aagDebugPrint(token, countLine, beginAddr, "O");
                O = atoi(token.c_str());
            }  else if(beginAddr == 13) {
                aagDebugPrint(token, countLine, beginAddr, "A");
                A = atoi(token.c_str());
            }
        } else if(countLine >=2){
           // int tmpLine = countLine;
            if(countLine <= I+1) {
                aagDebugPrint(token, countLine, beginAddr, "PI" , 1);
                CirPIGate* pi = new CirPIGate(countLine,atoi(token.c_str())/2 );
                //CirPIGate* pi = new CirPIGate();
                //pi->setLineNo(countLine);
                //pi->_lineNo = countLine;
                //cout << "*pi = " << pi << endl;
                //printf("pi = %p\n",pi);
                //cout << "getLine = " << pi->getLineNo() << endl;
                //cout << "ID = " << pi->getID() << endl;
                _piList.push_back(pi);
                //_totalList.push_back(piG);
                
                //pi->_type  = 1;
                
            } else if(countLine <= O+I+1) {
                    aagDebugPrint(token, countLine, beginAddr, "PO" , 1 );
            } else if(countLine <= A+O+I+1) {
                    aagDebugPrint(token, countLine, beginAddr, "aig" , 1 );
            } 
        } 
}

bool
CirMgr::readCircuit(const string& fileName)
{
   string line;
   size_t countLine = 0;
   ifstream myfile;
   string token;
   cout << "fileName = " << fileName << endl; 
   myfile.open(fileName,ios::in);
   if(myfile.is_open()) {
     while(getline(myfile,line)) {
        ++countLine;
        //cout << line << endl;
        size_t m = 0; //record beginAddr
        size_t n; 
       // cout << "n = " << n << endl;
        if(countLine == 1){
            n = newMyStrGetTok(line, token, m);
            while(token.size()){
              //cout << "token = " << token << endl;
             // cout << "token = " << token << ",  m+1 = " << m+1 << endl;
              aagRecorder(token,countLine,m+1);
              n = newMyStrGetTok(line, token, m,n);
             // cout << "n = " << n << endl;
            }
        } else {
            aagRecorder(line,countLine,m+1);
        }
     }
     cout << "_piList.size() = " << _piList.size() << endl;
     cout << "M = " << M << ", I = " << I << ", L = " << L<< " , O = " << O<< ", A = " << A << endl<<endl;
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
}

void
CirMgr::printNetlist() const
{
}

void
CirMgr::printPIs() const
{
   cout << "PIs of the circuit:";
   cout << endl;
}

void
CirMgr::printPOs() const
{
   cout << "POs of the circuit:";
   cout << endl;
}

void
CirMgr::printFloatGates() const
{
}

void
CirMgr::writeAag(ostream& outfile) const
{
}
