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
        size_t operator() () const {
            return 1;
        }   
        bool operator == (const Data& d) const {
            return true;
        }

    private:
};

class TaskNode
{
#define NAME_LEN  5
#define LOAD_RN   10000

public:
   TaskNode();
   TaskNode(const string& n, size_t l) : _name(n), _load(l) {}
   ~TaskNode() {}

   void operator += (size_t l) { _load += l; }
   bool operator == (const TaskNode& n) const { return _name == n._name; }
   bool operator < (const TaskNode& n) const { return _load < n._load; }
   size_t operator () () const;

   const string& getName() const { return _name; }
   size_t getLoad() const { return _load; }

   friend ostream& operator << (ostream& os, const TaskNode& n); 

private:
   string   _name;
   size_t   _load;
};

template <class Data>
class HashSet
{
public:
   HashSet() : _numBuckets(0), _buckets(0) {}
   HashSet(size_t b) : _numBuckets(0), _buckets(0) { init(b); }
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
      iterator(TaskNode* myN=0): _node(myN) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator(){}
      const TaskNode& operator * () const {
            return *_node;
      }
      TaskNode& operator * () {
            // cout << "T& operator * ()" << endl; 
            return *_node;
      } 
      iterator& operator ++ () {
         //cout << " iterator& operator ++ ()" << endl; 
         _node = ++_node;
         return *(this); 
      }   
      iterator operator ++ (int) {
        //cout << " iterator operator ++ (int)" << endl; 
        iterator iter = *(this);
        _node = _node+1; 
        return iter;
      }
      bool operator != (const iterator& i) const {
            cout << "bool operator != " << endl; 
            return _node!=i._node;
      }
       
      bool operator == (const iterator& i) const {
            cout << "bool operator == " << endl; 
            return _node==i._node;
      }
      //friend ostream& operator << (ostream& os, const TaskNode& n);
   private:
     TaskNode* _node; 
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
        iterator(); 
   }
   // Pass the end
   iterator end() const { 
        cout << "hash end" << endl;
        iterator(); 
   }
   // return true if no valid data
   bool empty() const { return true; }
   // number of valid data
   size_t size() const { size_t s = 0; return s; }

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
        size_t bucketSize = _buckets[bucketIdx].size();
        _buckets[bucketIdx].push_back(d);
        TaskNode taskNode(d.getName(),d.getLoad());
        cout << "hash insert success , d = " << d  << endl;
        cout << "bucketIdx = " << bucketIdx  << endl;
        cout << "bucketSize = " << bucketSize  << endl;
        return true; 
   }

private:
   // Do not add any extra data member
   size_t            _numBuckets;
   vector<Data>*     _buckets;

   size_t bucketNum(const Data& d) const {
      return (d() % _numBuckets); }
};

#endif // MY_HASH_SET_H
