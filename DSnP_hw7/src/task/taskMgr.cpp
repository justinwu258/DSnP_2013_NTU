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
   cout << "overload operator in taskMgr.cpp" << endl;
   return os << "(" << n._name << ", " << n._load << ")";
}

TaskMgr::TaskMgr(size_t nMachines)
: _taskHeap(nMachines), _taskHash(getHashSize(nMachines))
{
   for (size_t i = 0, n = nMachines; i < n; ++i) {
      if (!_taskHash.insert(_taskHeap[i])) {
         _taskHeap.delData(i); --n; --i;
      }
      cout << "insert ok , _taskHeap[" << i << "] = " << _taskHeap[i] << endl;
   }
}
// END: DO NOT CHANGE THIS PART

// Exactly nMachines (nodes) will be added to Hash and Heap
void
TaskMgr::add(size_t nMachines)
{
   // TODO...
}

// return true TaskNode is successfully inserted
// return false if equivalent node has already existed
bool
TaskMgr::add(const string& s, size_t l)
{
   // TODO...
   return false;
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
   return true;
}

// WARNING: DO NOT CHANGE THIS FUNCTION!!
void
TaskMgr::printAll() const 
{
    TaskNode* aa = new TaskNode("aaa",555);
      cout << "Just test" << endl;
      cout << "get Name = " << aa->getName() << ", get Load = " << aa->getLoad() << endl;
      cout << "*aa = " << *aa << endl;
   HashSet<TaskNode>::iterator hi = _taskHash.begin();
        cout << "Just test2" << endl;
        cout << *hi << endl;
   for (; hi != _taskHash.end(); ++hi) 
   {
        cout << "Just test" << endl;
        cout << *hi << endl;
   }
      //cout << hi << endl;
}

