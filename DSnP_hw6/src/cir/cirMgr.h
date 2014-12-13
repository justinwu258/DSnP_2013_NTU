/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

//#define aagDebug

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#include "cirDef.h"

extern CirMgr *cirMgr;

// TODO: Define your own data members and member functions
class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { return 0; }
   //CirPIGate* getPIGate(unsigned gid) const { return 0; }
   
   // Member functions about circuit construction
   bool readCircuit(const string&);
   void aagRecorder(string  , size_t , size_t, size_t );
   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
   // below all , is added by Justin
   vector<CirPIGate*> _piList;
   vector<CirPOGate*> _poList;
   vector<CirAIGGate*> _aigList;
   vector<CirGate*> _totalList;
   //CirGate**        _myTotalList; 
   size_t M, I, L, O, A;
   vector<string> _tokenList; 
 
   // ---------  member function -----------   
   size_t newMyStrGetTok(const string& str, string& tok, size_t& beginAddr,           size_t pos = 0 ,const char del = ' ')
   {
      size_t begin = str.find_first_not_of(del, pos);
      if (begin == string::npos) { tok = ""; return begin; }
      size_t end = str.find_first_of(del, begin);
      tok = str.substr(begin, end - begin);
      beginAddr = begin;
      return end;
   }
   
   void aagDebugPrint(string  token, size_t countLine, size_t beginAddr, string specStr, int ID = -1) {
       #ifdef aagDebug
        string tmpToken = token;
        cout << token << " - " << specStr;
        if(ID != -1) {
            size_t m;
            size_t n = newMyStrGetTok(tmpToken, token, m);
            while(token.size()){    
                if(atoi(token.c_str())%2 == 0) {
                    cout << " " << atoi(token.c_str())/2;
                } else {
                    cout << " !" << atoi(token.c_str())/2;
                }
                n = newMyStrGetTok(tmpToken, token, m , n);
                
            }
        }
        cout << " (" << countLine << ", " << beginAddr << ")" << endl; 
       #endif
   }
};

#endif // CIR_MGR_H
