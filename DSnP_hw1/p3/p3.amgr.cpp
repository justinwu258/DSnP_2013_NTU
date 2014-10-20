/****************************************************************************
  FileName     [ p3.amgr.cpp ]
  PackageName  [ HW1 ]
  Synopsis     [ For problem 3 of HW1 in DSnP class ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include "p3.amgr.h"

using namespace std;

// TODO: Initialize static data variable in class A
// TODO: Implement class A's constructors and desctructor
unsigned A::_count = 0;

A::A(){
   //_count ;
   //_data = 1;
   //_occurence = 1;
}

A::A(const string& s){
   _count += 1;
   _data = s;
   //_occurence = 1 ;
}

A::~A(){
   _count = _count - 1;
   
}

void AMgr::cleanAll()
{
   _amap.clear();
   _alist.clear(); 
   // TODO: clean up all the data in AMgr::_amap and AMgr::_alist
}

// If _amap or _alist is not empty, it will be cleaned anyway.
void AMgr::import(ifstream& inf)
{
   if (!checkEmpty()) {
      cout << "Warning: Database is not empty. Erasing..."; cout.flush();
      cleanAll();
      cout << " done." << endl;
   }

   #define BUF_LEN 65536
   char buf[BUF_LEN];
   unsigned lineNo = 0;
   do {
      // Read one line at a time, and parse this line into string tokens
      // The parsed string tokens are stored in vector<string> strs
      inf.getline(buf, BUF_LEN);
      stringstream ss((string(buf)));
      istream_iterator<string> bb(ss);
      istream_iterator<string> ee;
      vector<string> strs(bb, ee);

      // Store the parsed string tokens to both _amap and _alist
      for (size_t i = 0, s = strs.size(); i < s; ++i) {
         newEntry(strs[i], lineNo, i);
      }

      lineNo++;
   } while (!inf.eof());
}

// TODO:
// 1. Check is there is already an entry (str, A*) in _amap
// 2. If not, create a new A*, map (str, A*), and store this A* to _alist
// 3. Record the (lineNo, wordNo) to A::_occurence
void AMgr::newEntry(const string& str, unsigned lineNo, unsigned wordNo)
{
     	if(_amap.count(str)>0)
	{
		_amap.find(str)->second->addOccurence(lineNo,wordNo);
	}
	else {
		A* hi = new A(str);
		_amap.insert(make_pair(str,hi));
		_alist.push_back(hi);
		hi->addOccurence(lineNo,wordNo);
	}	
}

// TODO: Implement the ostream& operator<< for AMgr and A.
// A sample input/output can be found at p3.in and p3.out.ref
// 
ostream& operator << (ostream& os, const AMgr& am)
{
	os << "Total distinct words: " << am._amap.size()  << endl;	
	int i = 0;
        for ( vector<A*>::const_iterator iter = am._alist.begin();
                        iter != am._alist.end(); ++iter )
        {    
                os << "[" << i  <<  "] " << (**iter)  << endl ;
		            i++;
        }   	
	return os;	
}

ostream& operator << (ostream& os, const A& a)
{
	os << a._data << endl;
	for ( vector<Occurence>::const_iterator iter = a._occurence.begin();
                        iter != a._occurence.end(); ++iter )
        {    
                os << "( " << iter->first   << ", " << iter->second  << " )" << "  " ;
                
        }   
	cout << endl;
	return os;
}

