/****************************************************************************
  FileName     [ cirGate.h ]
  PackageName  [ cir ]
  Synopsis     [ Define basic gate data structures ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef CIR_GATE_H
#define CIR_GATE_H

#include <string>
#include <vector>
#include <iostream>
#include "cirDef.h"
#include "sat.h"

using namespace std;

// TODO: Feel free to define your own classes, variables, or functions.

class CirGate;

//------------------------------------------------------------------------
//   Define classes
//------------------------------------------------------------------------
class CirGate
{
    friend class CirMgr; // that's , CirMgr can access Protected member
public:
   CirGate(int l, int I): _lineNo(l), _ID(I), _isVisited(false), _isRecurVisited(false){};
   virtual ~CirGate() {}

   // Basic access methods
   string getTypeStr() const { 
        if(_type == "PO")
            return "input"; 
        return ""; 
   }
   unsigned getLineNo() const { return _lineNo; }
   unsigned getID() const { return _ID; }
   void recurFaninDFS(int, const CirGate* , int, bool , vector<CirAIGGate*>&) const; 
   void recurFanoutDFS(int, const CirGate* , int, bool , vector<CirAIGGate*>&) const; 
   void setVisited(bool visited)  {  _isRecurVisited = visited; }

   // Printing functions
   void printGate() const;
   void reportGate() const;
   void reportFanin(int level) const;
   void reportFanout(int level) const;

private:
    int _lineNo;
    string _type;

protected:
    int _flag;
    int _column;
    int _ID;
    string _name; 
    vector<CirGate*> _faninList;
    vector<CirGate*> _fanoutList;
    bool _isVisited;
    mutable bool _isRecurVisited; //store AIG visited or not , when recur search
};


class CirGateInfo{
    public:
        CirGateInfo(int t, int I): _type(t), _ID(I){}
        ~CirGateInfo(){}
    protected:
        int _type;
        int _ID;
    
};

class CirPIGate: public CirGate{
    public:
        //friend class CirGate;
        //CirPIGate(){}
        CirPIGate(int l, int I): CirGate(l, I){}
        ~CirPIGate() {}
        //void printGate() const;
        //void setLineNo(int number){ _lineNo = number; }
   // friend int _lineNo;
   // friend int _type;
    //void printGate();
};
class CirPOGate: public CirGate{
        friend class CirMgr;
    public:
        CirPOGate(int l, int I, int in): CirGate(l, I), _faninID(in) , _isInvert(0){};
        ~CirPOGate() {}
        
        int getIsInv()   { return _isInvert; }
        int getFaninID() { return _faninID;  }
    private:
        int _faninID;
        int _isInvert;
};

class CirAIGGate: public CirGate{
        friend class CirMgr;
    public:
        CirAIGGate(int l, int I): CirGate(l, I), _rhs1_invert(0),_rhs2_invert(0){};
        ~CirAIGGate() {}
        int getRhs1Inv()   { return _rhs1_invert; }
        int getRhs2Inv()   { return _rhs2_invert; }
    protected:
        int _rhs1_invert;
        int _rhs2_invert;
};
class CirUndefGate: public CirGate{
        friend class CirMgr;
    public:
        CirUndefGate(int l, int I): CirGate(l, I){};
        ~CirUndefGate() {}
};
class CirConstGate: public CirGate{
        friend class CirMgr;
    public:
        CirConstGate(int l, int I): CirGate(l, I), _isInvert(0) {};
        ~CirConstGate() {}
    private:
        int _isInvert;
};

#endif // CIR_GATE_H
