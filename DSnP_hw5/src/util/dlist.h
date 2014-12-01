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
      const T& operator * () const { 
            return _node->_data; 
      }
      T& operator * () { 
            // cout << "T& operator * ()" << endl; 
            return _node->_data; 
      }
      iterator& operator ++ () {
         cout << " iterator& operator ++ ()" << endl; 
         _node = _node->_next;
         return *(this); 
      }
      iterator operator ++ (int) {
        cout << " iterator operator ++ (int)" << endl; 
        iterator iter = *(this);
        _node = _node->_next; 
        return iter;
      }
      iterator& operator -- () { 
        //cout << " iterator& operator -- ()" << endl; 
        _node = _node->_prev;
        return *(this); 
        }
      iterator operator -- (int) { 
        //cout << " iterator operator -- (int)" << endl;
        iterator iter = *(this);
        //cout << "(int --) iter._node = " << iter._node << endl;
        _node = _node->_prev; 
         return iter; 
      }

      iterator& operator = (const iterator& i) { 
            _node = i._node;
            return *(this); 
      }

      bool operator != (const iterator& i) const {
            ///usleep(1000000; 
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
        return iterator(_head); 
   }
   iterator end() const { 
        // ***
        //iterator(_next) = begin();
        //iterator(_prev) = _node->_data;
        return iterator(_head->_prev); 
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
   size_t size() const {  
        size_t count = 0;
        iterator iter;
        for(iter = begin(); iter!= end(); ++iter) {
            count++;
        }
        return count; 
   }

   void push_back(const T& x) {
        //DListNode<T>* dummy;  // origin _head is point to dummy
        //DListNode<T>* t = new DListNode<T>(x,dummy,dummy);
        if(empty()){
            //dummy = new DListNode<T>(x,_head,_head);  // origin _head is point to dummy
            cout << "push in empty" << endl;
            DListNode<T>* t = new DListNode<T>(x,_head,_head);
            _head->_next = t; // dummy->_next = t
            _head->_prev = t; // dummy->_prev = t
            _head = t;        //_head point to "actual first data"        
        } else {
            cout << "push in else" << endl;
            iterator iter;
            
            for(iter = _head->_prev->_prev; iter != end() ; --iter) // start from "dummy->_prev" , end in _head->prev = "dummy"
            {
                if(iter._node->_next == _head->_prev){
                    DListNode<T>* v = new DListNode<T>(x,iter._node,_head->_prev);
                    iter._node->_next = v;   // t->_next = v
                    _head->_prev->_prev = v;          // 
                }
            }
           // if(iter == begin())
          //  {

          //      if(iter._node->_next == _head->_prev){
          //          DListNode<T>* v = new DListNode<T>(x,iter._node,_head->_prev);
          //          iter._node->_next = v;   // t->_next = v
          //          _head->_prev->_prev = v;          //
          //      } 
          //  }
            /* 
            iterator iter;
            
            for(iter = end(); iter != begin(); --iter){  //step1 end() = _head->_prev = dummy 
                DListNode<T>* t = new DListNode<T>(x,it->_node,dummy);  // step2 dummy->_prev = t , begin = head = t
            }*/
        } 
   }
   void pop_front() { }
   void pop_back() { }

   // return false if nothing to erase
   bool erase(iterator pos) { return false; }
   bool erase(const T& x) { return false; }

   void clear() { 
        iterator iter;
        for(iter = _head->_next; iter != _head->_prev;){
            DListNode<T>* delNode = iter._node;
            iter._node->_prev->_next = iter._node->_next;
            iter._node->_next->_prev = iter._node->_prev;
            ++iter; 
            delete delNode;
        }
        _head->_prev = _head->_next = _head; // _head is a dummy node 
   }  // delete all nodes except for the dummy node

   void sort() const { }

private:
   DListNode<T>*  _head;  // = dummy node if list is empty

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
