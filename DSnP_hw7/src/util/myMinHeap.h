/****************************************************************************
  FileName     [ myMinHeap.h ]
  PackageName  [ util ]
  Synopsis     [ Define MinHeap ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_MIN_HEAP_H
#define MY_MIN_HEAP_H

#include <algorithm>
#include <vector>

template <class Data>
class MinHeap
{
public:
   MinHeap() : _data(0) {}
   MinHeap(size_t s) {
      _data.reserve(s);
      for (size_t i = 0; i < s; ++i) {
            //cout << "Heap push";
            _data.push_back(Data());
            //cout << ", _data[" << i << "] = " << _data[i] << endl;
      }
      sort(_data.begin(), _data.end());
   }
   ~MinHeap() {}

   // For the following member functions,
   // We don't respond for the case vector "_data" is empty!
   const Data& operator [] (size_t i) const { return _data[i]; }   
   Data& operator [] (size_t i) { return _data[i]; }

   size_t size() const { return _data.size(); }

   // TODO
   const Data& min() const { 
        cout << "get min() in MinHeap" << endl;
        int tmpMin = 999999999, minIdx = -1;
        for(int i = 0; i < _data.size() ; i++) {
            if(tmpMin > _data[i].getLoad()){
                tmpMin = _data[i].getLoad();
                minIdx = i;
                cout << "Now Min node = _data[" << i << "] = " << _data[i];
            }
            
        }
        return _data[minIdx]; 
   }
   void insert(const Data& d) {
      cout << "minHeap insert success " << endl; 
   }
   void delMin() { }
   void delData(size_t i) { 
      cout << "minHeap delData success " << endl; 
     _data.erase(_data.begin()+i); 
   } // remove _data[i]

private:
   // DO NOT add or change data members
   vector<Data>   _data;
};

#endif // MY_MIN_HEAP_H
