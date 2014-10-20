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
   cout << "Hi , I am cleaner" << endl;
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
	int tmp_size;
//	_amap(str, A*); // cpp 找map count 
     	if(_amap.count(str)>0)
	{
		//map<string,A*>::iterator it;
		//it = _amap.find(str);
//		cout << "elements in _amap" << '\n';
//  	        cout << "amap => " << _amap.find(str)->first << '\n';
		_amap.find(str)->second->addOccurence(lineNo,wordNo);
//		cout << "_amap lineNo = " << lineNo << " , wordNo = " << wordNo << endl;		
	}
	else {
//		cout << "string input is " << str << endl;
		A* hi = new A(str);
		_amap.insert(make_pair(str,hi));
		_alist.push_back(hi);
		tmp_size = _alist.size();
//		cout << "tmp_size = " << tmp_size << endl;
//		cout << "vector = " << _alist[tmp_size] << endl;
		//cout << "map = " << _amap[tmp_size] << endl;
					
		hi->addOccurence(lineNo,wordNo);
/*	
		for ( map<string,A*>::const_iterator iter = _amap.begin();
      			iter != _amap.end(); ++iter )
      		{ cout << iter->first << '\t' << iter->second << '\n'; }
  */ 	
	//	cout << endl;	
//		cout << "lineNo = " << lineNo << " , wordNo = " << wordNo << endl;
	}	
	//hi->addOccurence(lineNo,wordNo);
}

// TODO: Implement the ostream& operator<< for AMgr and A.
// A sample input/output can be found at p3.in and p3.out.ref
// 
ostream& operator << (ostream& os, const AMgr& am)
{
	//A* hello = NULL;
	A hello;
	os << hello ;
	A& yes;
	//os << "Total distinct words: " << hello->getCount() << endl;	
/*
	for(int i = 0; i < am._amap.size() ; i++)
	{
		os << "[" << i  <<  "] " << am._amap->first << endl;
	//	cout << "i = " << i << ", _amap.size() = " << am._amap.size() << endl;
	}
*/
/*
	int i = 0;
	for ( map<string,A*>::const_iterator iter = am._amap.begin();
                        iter != am._amap.end(); ++iter )
        {	 
		os << "[" << i  <<  "] " << iter->second << endl;
		i++;
	}
*/
	int i = 0;
        for ( vector<A*>::const_iterator iter = am._alist.begin();
                        iter != am._alist.end(); ++iter )
        {    
		hello = am._alist[i]*;
                //os << "[" << i  <<  "] " << am._alist[i] ;
                os << "[" << i  <<  "] " << hello ;
                i++;
        }   	
	return os;	
}

ostream& operator << (ostream& os, const A& a)
{
	os << "Total distinct words: " << a._count << endl;	
	os << a << endl;
/*	for ( vector<Occurence>::const_iterator iter = _occurence.begin();
                        iter != _occurence.end(); ++iter )
        {    
                os << "( " << _occurence.first  <<  << ", " << _occurence.second  << " )" << "  " ;
                
        } */  
	return os;
}

