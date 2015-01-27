/****************************************************************************
  FileName     [ taskMgr.cpp ]
  PackageName  [ task ]
  Synopsis     [ Define member functions for task Manager ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <iostream>
#include <string>
#include <cassert>
#include "taskMgr.h"
#include "rnGen.h"
#include "util.h"
//#define delMinDebug
//#define delDataDebug

using namespace std;

TaskMgr *taskMgr = 0;

// BEGIN: DO NOT CHANGE THIS PART
TaskNode::TaskNode() 
{
   _name.resize(NAME_LEN);
   for (int i = 0; i < NAME_LEN; ++i)
      _name[i] = 'a' + rnGen(26);
   _load = rnGen(10000);
}

size_t
TaskNode::operator () () const 
{
   size_t k = 0, n = (_name.length() <= 5)? _name.length(): 5;
   for (size_t i = 0; i < n; ++i)
      k ^= (_name[i] << (i*6));
   return k;
}

ostream& operator << (ostream& os, const TaskNode& n)
{
   return os << "(" << n._name << ", " << n._load << ")";
}

TaskMgr::TaskMgr(size_t nMachines)
: _taskHeap(nMachines), _taskHash(getHashSize(nMachines))
{
   for (size_t i = 0, n = nMachines; i < n; ++i)
      if (!_taskHash.insert(_taskHeap[i])) {
         _taskHeap.delData(i); --n; --i;
      }
}
// END: DO NOT CHANGE THIS PART

// Exactly nMachines (nodes) will be added to Hash and Heap
void
TaskMgr::add(size_t nMachines)
{
   int heapIdx = -1; //, initSize;
   for (size_t i = 0, n = nMachines; i < n; ++i) {
        TaskNode newNode;
        heapIdx = _taskHeap.insert(newNode);
        //cout << "New task node: " << *newNode << endl;  // ",  i = " << i << endl;
        if (!_taskHash.insert(newNode)) {
            _taskHeap.delData(heapIdx);  --i;
        } else {
            cout << "New task node: " << newNode << endl;  // ",  i = " << i << endl;
        }
   }
   // TODO...
}

// return true TaskNode is successfully inserted
// return false if equivalent node has already existed
bool
TaskMgr::add(const string& s, size_t l)
{
   // TODO...
   int heapIdx = -1 ;
   TaskNode newNode(s,l);
   heapIdx = _taskHeap.insert(newNode);
   //cout << "New task node: " << *newNode << endl;  // ",  i = " << i << endl;
   if (!_taskHash.insert(newNode)) {
       _taskHeap.delData(heapIdx); 
      return false;
   } else {
        cout << "New task node: " << newNode << endl;  // ",  i = " << i << endl;
   }
   return true;
}

// Assign the min task node with 'l' extra load.
// That is, the load of the min node will be increased by 'l'.
// The min node in the heap should be updated accordingly.
// The corresponding node in the hash should be updated, too.
// return false if taskMgr is empty
// otherwise, return true.
bool
TaskMgr::assign(size_t l)
{
   // TODO... 
   #ifdef delMinDebug
        delMinPrint();  
   #endif 
   #ifdef delDataDebug
        delDataPrint();
   #endif 
   
   if(size() == 0) {
        cout << "taskMgr is empty" << endl;
        return false;
   } 
   TaskNode tmpNode;
   tmpNode  = _taskHeap[0];
   //cout << "tmpNode = " << *tmpNode << endl;  
  
   (tmpNode) += -(tmpNode).getLoad();
   (tmpNode) += l;
   if(!_taskHash.update(tmpNode)) {
   } else {
        (tmpNode) = _taskHeap[0];
        (tmpNode) += l; 
        _taskHeap.delMin();
        //cout << "*tmpNode = " << *tmpNode << endl;
        _taskHeap.insert(tmpNode);
   } 
    
    
   return true;
}

// WARNING: DO NOT CHANGE THIS FUNCTION!!
void
TaskMgr::printAll() const 
{
   HashSet<TaskNode>::iterator hi = _taskHash.begin();
   for (; hi != _taskHash.end(); ++hi)
      cout << *hi << endl;
     //_taskHash.myPrintAll();
}

//help functions
void TaskMgr::delMinPrint() 
{
   int count = 0;
        //cout << "heap Size = " << _taskHeap.size() << endl;
   while(_taskHeap.size() > 0) {
        cout << endl <<"------ now count == " << count <<  ",  heapSize = " << _taskHeap.size() <<" ------"<< endl;
        printHeap();
        //cout << "  heap Size = " << _taskHeap.size() << endl;
        _taskHeap.delMin();
        count++;
   }
}

void TaskMgr::delDataPrint() {
    cout << "  --- Before delData ---" << endl;
    printHeap();
    int delIdx = 1;
    for(int i = 0; i < 100 ; i++) {
        delIdx = (i+1)*3;
        if( delIdx > (_taskHeap.size()-1)) {
            cout << "  --- break delData debug --- " << endl;
            break;
        }
        _taskHeap.delData(delIdx);
        cout << "  --- After delData ---" << endl;
        printHeap();
    }
}
void TaskMgr::printHeap() const
{
            for(int i = 0; i < _taskHeap.size() ; i++){
                cout << "  _taskHeap[" << i << "] = " << _taskHeap[i] << endl;
            }
}
