/****************************************************************************
  FileName     [ myHashSet.h ]
  PackageName  [ util ]
  Synopsis     [ Define HashSet ADT ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>
//#include "taskMgr.h"
using namespace std;

//---------------------
// Define HashSet class
//---------------------
// To use HashSet ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ()" is to check whether there has already been
// an equivalent "Data" object in the HashSet.
// Note that HashSet does not allow equivalent nodes to be inserted
//
class Data
{
    public:
        Data() {cout << "init data" << endl;}
        ~Data(){}
        size_t operator() () {
            cout << "overload data()" << endl;
            return 1;
        }   
        const size_t operator() () const {
            cout << "overload data() const" << endl;
            return 1;
        }   
        bool operator == (const Data& d) const {
            cout << "overload data == " << endl;
            return true;
        }

    private:
};


template <class Data>
class HashSet
{
public:
   HashSet() : _numBuckets(0), _buckets(0) {}
   HashSet(size_t b) : _numBuckets(0), _buckets(0) { 
        cout << "b = " << b << endl;
        init(b); 
   }
   ~HashSet() { reset(); }
  // typedef pair<Data ,Data> HashNode;
   // TODO: implement the HashSet<Data>::iterator
   // o An iterator should be able to go through all the valid Data
   //   in the Hash
   // o Functions to be implemented:
   //   - constructor(s), destructor
   //   - operator '*': return the HashNode
   //   - ++/--iterator, iterator++/--
   //   - operators '=', '==', !="
   //
   
   class iterator
   {
      friend class HashSet<Data>;
   public:
      iterator(vector<Data>* myN=0): _node(myN), _dataIdx(0),_buckIdx(0) {}
      //iterator(const iterator& i) : _node(i._node) {}
      //iterator(const iterator& i) : _node(i._node) {}
      //iterator(const vector<Data> i) : _node(i._node) {}
      ~iterator(){}
      const Data& operator * () const {
            cout << "const T& operator * ()" << endl; 
            return *_node;
      }
      Data& operator * () {
            cout << "T& operator * ()" << endl; 
            cout << "_buckIdx = " << _buckIdx << endl; 
            return (*_node)[_buckIdx];
      } 
      iterator& operator ++ () {
         cout << " iterator& operator ++ ()" << endl; 
        // _node = ++_node;
        if((*_node)[_buckIdx].getLoad() != 0) {

             ++_buckIdx;
        } else {
             _node+1;
             _buckIdx = 0;
        }
             //++_dataIdx;
     //    }   else {
     //        //_dataIdx = 0;
     //        ++_buckIdx;
     //    }
         return *(this); 
      }   
      iterator operator ++ (int) {
        cout << " iterator operator ++ (int)" << endl; 
        iterator iter = *(this);
        //_node = _node+1; 
        if(_node[_buckIdx][_dataIdx+1] != 0) {
            ++_buckIdx;
            //++_dataIdx;
        }   else {
            //_dataIdx = 0;
            ++_buckIdx;
        }
        return iter;
      }
      bool operator != (const iterator& i) const {
            cout << "bool operator != " << endl;
            if(_buckIdx == 20)
                return false; 
            return _node!=i._node;
      }
       
      bool operator == (const iterator& i) const {
            cout << "bool operator == " << endl; 
            return _node==i._node;
      }
      //friend ostream& operator << (ostream& os, const TaskNode& n);
   private:
     vector<Data>* _node;
     size_t      _dataIdx , _buckIdx; 
   };

   void init(size_t b) {
      reset(); _numBuckets = b; _buckets = new vector<Data>[b]; }
   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = 0; }
   }
   size_t numBuckets() const { return _numBuckets; }

   vector<Data>& operator [] (size_t i) { return _buckets[i]; }
   const vector<Data>& operator [](size_t i) const { return _buckets[i]; }

   // TODO: implement these functions
   //
   // Point to the first valid data
   iterator begin() const { 
        cout << "hash begin" << endl;
        return iterator(_buckets); 
        //return iterator(_buckets->begin()); 
   }
   // Pass the end
   iterator end() const { 
        cout << "hash end" << endl;
        return iterator(); 
   }
   // return true if no valid data
   bool empty() const { return true; }
   // number of valid data
   size_t size() const { 
       cout << "hash size" << endl; 
       size_t s = 0; return s; 
   }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const Data& d) const { return false; }

   // query if d is in the hash...
   // if yes, replace d with the data in the hash and return true;
   // else return false;
   bool query(Data& d) const { return false; }

   // update the entry in hash that is equal to d
   // if found, update that entry with d and return true;
   // else insert d into hash as a new entry and return false;
   bool update(const Data& d) { return false; }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const Data& d) {
        size_t bucketIdx = bucketNum(d);
           // cout << "hash insert success , d = " << d  << endl;
        cout << "bucketIdx = " << bucketIdx << endl;
            cout << "**_buckets = " << &(_buckets[0])  << endl;
            cout << "***_buckets = " << &(_buckets[bucketIdx])  << endl;
        size_t bucketSize = _buckets[bucketIdx].size();
        cout << "bucketSize = " << bucketSize << endl;
        cout << "bucketCapacity = " << _buckets[bucketIdx].capacity() << endl;
        //vector<Data>::iterator it;
        // it = (*_buckets).begin();
            //cout << "_buckets[" << bucketSize << "][" << 0 << "] = " << _buckets[bucketSize][0] << endl;
  //      for(int i = 0; i < bucketSize ; i++) {
  //          if(!_buckets[bucketIdx].empty()){
  //              cout << "_buckets[" << bucketSize << "][" << i << "] = " << _buckets[bucketSize][i] << endl;
  //              if(_buckets[bucketIdx][i] == d){
  //                  cout << "d is already in the hash" << endl;
  //                  return false;
  //              }
  //          }
  //      }
    //    if(d == d) {
    //        cout << "d is already in the hash" << endl;
    //        return false;
    //    } else {
        cout << "2. bucketSize = " << bucketSize << endl;
            _buckets[bucketIdx].push_back(d);
        ++bucketSize;
        cout << "3. bucketSize = " << bucketSize << endl;
           cout << "_buckets[" << bucketIdx << "][" << bucketSize-1 << "] = " << _buckets[bucketIdx][bucketSize-1] << endl;
            //TaskNode taskNode(d.getName(),d.getLoad());
            cout << "hash insert success , d = " << d  << endl;
            cout << "_buckets = " << &(_buckets[bucketIdx])  << endl;
            //cout << "_buckets->begin() = " << _buckets->begin()  << endl;
            cout << "bucketIdx = " << bucketIdx  << endl;
            cout << "bucketSize = " << bucketSize  << endl;
        //    cout << "numBuckets = " << _numBuckets  << endl;
            return true; 
    //    }
   }
  
   void myPrintAll() const {
        for(int i=0 ; i <  numBuckets() ; i++) {
            if( !_buckets[i].empty()){
                for(typename vector<Data>::iterator it =  _buckets[i].begin(); it !=  _buckets[i].end() ; it++ ) {
                    cout << "Just print" << endl;
                    cout << (*it) << endl;
                }   
            }   

        }
   }
   vector<Data>*     _buckets;

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   //vector<Data>*     _buckets;
   Data* _first;

   size_t bucketNum(const Data& d) const {
      //cout << "d() = " << d() << endl;
      //cout << " d == d? " << (d == d) << endl;
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
