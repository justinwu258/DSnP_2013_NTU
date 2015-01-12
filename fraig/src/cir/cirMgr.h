/****************************************************************************
  FileName     [ cirMgr.h ]
  PackageName  [ cir ]
  Synopsis     [ Define circuit manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_MGR_H
#define CIR_MGR_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

#include "cirDef.h"

extern CirMgr *cirMgr;

class CirMgr
{
public:
   CirMgr(){}
   ~CirMgr() {cleanGate();}

   // Access functions
   // return '0' if "gid" corresponds to an undefined gate.
   CirGate* getGate(unsigned gid) const { 
        if(gid <= M+O)
            return _totalList[gid]; 
        else
            return 0;
   }

   // Member functions about circuit construction
   bool readCircuit(const string&);
   // Justin add member function
   void aagRecorder(string  , size_t , size_t, size_t );
   void cleanGate();
   void myDFS(CirGate*);

   // Member functions about circuit optimization
   void sweep();
   void mergeSweep();
   void optimize();
   void merge(CirGate* gate,int mergeMethod);
   void mergeMsg(string mergeType,int mergeID,bool mergeIsInv,int removeID);
   

   // Member functions about simulation
   void randomSim();
   void fileSim(ifstream&);
   void setSimLog(ofstream *logFile) { _simLog = logFile; }

   // Member functions about fraig
   void strash();
   void printFEC() const;
   void fraig();

   // Member functions about circuit reporting
   void printSummary() const;
   void printNetlist() const;
   void printPIs() const;
   void printPOs() const;
   void printFloatGates() const;
   void printFECPairs() const;
   void writeAag(ostream&) const;

private:
   ofstream           *_simLog;

   // below all , is added by Justin
   vector<CirPIGate*> _piList;
   vector<CirPOGate*> _poList;
   vector<CirAIGGate*> _aigList;
   vector<CirUndefGate*> _undefList;
   vector<CirGate*> _totalList;
   vector<CirGate*> _dfsList;
   //CirGate**        _myTotalList; 
   size_t M, I, L, O, A;
   vector<string> _tokenList; 
   //vector<CirGate*> _optList;
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
