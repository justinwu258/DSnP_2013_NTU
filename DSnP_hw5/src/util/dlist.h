/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>
#include <unistd.h>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { return *(this); }
      T& operator * () { 
            // cout << "T& operator * ()" << endl; 
            return _node->_data; 
      }
      iterator& operator ++ () {
         //cout << " iterator& operator ++ ()" << endl; 
         _node = _node->_next;
         return *(this); 
      }
      iterator operator ++ (int) { return *(this); }
      iterator& operator -- () { 
        //cout << " iterator& operator -- ()" << endl; 
        _node = _node->_prev;
        return *(this); 
        }
      iterator operator -- (int) { 
        cout << " iterator operator -- (int)" << endl;
        _node = _node->_prev; 
         return *(this); 
      }

      iterator& operator = (const iterator& i) { 
            _node = i._node;
            return *(this); 
      }

      bool operator != (const iterator& i) const {
            //usleep(1*10^5); 
            //cout << "bool operator != " << endl; 
            return _node!=i._node; 
      }
      bool operator == (const iterator& i) const { 
            cout << "bool operator == " << endl; 
            return _node==i._node;
      }

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const { 
        return iterator(_head->_next); 
   }
   iterator end() const { 
        // ***
        //iterator(_next) = begin();
        //iterator(_prev) = _node->_data;
        return iterator(_head); 
   }
   bool empty() const { 
        if (_head->_next == _head)
        {   
            cout << "empty " << endl;
            return true;
        }
        else     
            return false; 
   }
   size_t size() const {  return 0; }

   void push_back(const T& x) {
      //if(_head->_prev == _head) { 
      if(empty()) { 
                    cout << "a. "<<endl;
            DListNode<T>* t = new DListNode<T>(x,_head,_head);
            _head->_next = t;
            _head->_prev = t;
            //_head->_prev = _head;  // not necessary , but equal to this 
            //_head = t;
        }
 //       else if(_head->_prev == _head->_next){    // end().prev == begin()
 //                   cout << "b." <<endl;
 //           iterator iter = _head->_prev;
 //           if(iter._node->_next == _head) {
 //                   cout << "5. iter._node" << iter._node <<endl;
 //                   cout << "add new node"  << endl;
 //                   DListNode<T>* t = new DListNode<T>(x,iter._node,_head);
 //                   //iter._node->_next = t;  //_head->_prev->_next
 //                   //iter._node = t;         //_head->_prev 
 //                   _head->_prev->_next = t;
 //                   _head->_prev= t;
 //           }
 //       }
        else {
            iterator iter = end();
                    //cout << "0. iter._node->_data = " << iter._node->_data <<endl;
            for(iter = end(); (iter != begin()) || (_head->_prev == _head->_next); --iter) { //(_head->_prev == _head->_next) means only 
                    //cout << "1. iter._node->_data = " << iter._node->_data <<endl;         //dummy node & another one node
                    //cout << "1. iter._node = " << iter._node <<endl;
               if(iter._node->_next == _head) {
                    //cout << "2. iter._node" << iter._node <<endl;
                    //cout << "add new node"  << endl;
                    DListNode<T>* t = new DListNode<T>(x,iter._node,_head);
                    _head->_prev->_next = t;
                    _head->_prev= t;
               }
            }
            
        }
   }
   void pop_front() { }
   void pop_back() { }

   // return false if nothing to erase
   bool erase(iterator pos) { return false; }
   bool erase(const T& x) { return false; }

   void clear() { }  // delete all nodes except for the dummy node

   void sort() const { }

private:
   DListNode<T>*  _head;  // = dummy node if list is empty

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
