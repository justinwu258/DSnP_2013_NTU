/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#define aagDebug

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
   
   // Member functions about circuit construction
   bool readCircuit(const string&);

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void writeAag(ostream&) const;

private:
   vector<CirGate*> _PI, _PO, _totalList; 
   size_t M, I, L, O, A;
   vector<string> _tokenList;  
   
   size_t newMyStrGetTok(const string& str, string& tok, size_t& beginAddr,           size_t pos = 0 ,const char del = ' ')
   {
      size_t begin = str.find_first_not_of(del, pos);
      if (begin == string::npos) { tok = ""; return begin; }
      size_t end = str.find_first_of(del, begin);
      tok = str.substr(begin, end - begin);
      beginAddr = begin;
      return end;
   }
   
   void aagDebugPrint(string  token, size_t countLine, size_t beginAddr, string specStr, int ID = -1,bool invert = false) {
       #ifdef aagDebug
        if(ID == -1)
            cout << token << " - " << specStr << " (" << countLine << "," << beginAddr << ")" << endl; 
        else if(!invert)
            cout << token << " - " << specStr << " " << ID << " (" << countLine << "," << beginAddr << ")" << endl; 
        else
            cout << token << " - " << specStr << " !" << ID << " (" << countLine << "," << beginAddr << ")" << endl; 
       #endif
   }
   void aagRecorder(string  token, size_t countLine, size_t beginAddr)
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
                aagDebugPrint(token, countLine, beginAddr, "PI" , atoi(token.c_str())/2 );
            } else if(countLine <= O+I+1) {
                if(atoi(token.c_str())%2 == 0)
                    aagDebugPrint(token, countLine, beginAddr, "PO" , atoi(token.c_str())/2 );
                else
                    aagDebugPrint(token, countLine, beginAddr, "PO" , atoi(token.c_str())/2, true);
            } else if(countLine <= A+O+I+1) {
                if(atoi(token.c_str())%2 == 0)
                    aagDebugPrint(token, countLine, beginAddr, "aig" , atoi(token.c_str())/2 );
                else
                    aagDebugPrint(token, countLine, beginAddr, "aig" , atoi(token.c_str())/2, true);
            } 
        } 
   }
};

#endif // CIR_MGR_H
