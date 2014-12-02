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
         //cout << " iterator& operator ++ ()" << endl; 
         _node = _node->_next;
         return *(this); 
      }
      iterator operator ++ (int) {
        //cout << " iterator operator ++ (int)" << endl; 
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
         //   cout << "bool operator == " << endl; 
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
            //cout << "empty " << endl;
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
            //cout << "push in empty" << endl;
            DListNode<T>* t = new DListNode<T>(x,_head,_head);
            _head->_next = t; // dummy->_next = t
            _head->_prev = t; // dummy->_prev = t
            _head = t;        //_head point to "actual first data"        
        } else {
            //cout << "push in else" << endl;
            iterator iter;
            
            for(iter = _head->_prev->_prev; iter != end() ; --iter) // start from "dummy->_prev" , end in _head->prev = "dummy"
            {
                if(iter._node->_next == _head->_prev){
                    DListNode<T>* v = new DListNode<T>(x,iter._node,_head->_prev);
                    iter._node->_next = v;   // t->_next = v
                    _head->_prev->_prev = v;          // 
                }
            }
        } 
   }
   void pop_front() {
        if(!empty()){
           iterator iter = begin(); //_head
           DListNode<T>* delNode = iter._node;
           iter._node->_prev->_next = iter._node->_next;
           iter._node->_next->_prev = iter._node->_prev;
           ++iter;
           _head = iter._node; // ** important!! can't delete  _head 
           delete delNode;
        }
   }
   void pop_back() {
        if(!empty()){
           iterator iter = _head->_prev->_prev; //dummy->prev
           DListNode<T>* delNode = iter._node;
           if(iter._node == _head) //means now is point to _head
                _head = iter._node->_next; // let _head point to dummy 
           iter._node->_prev->_next = iter._node->_next;
           iter._node->_next->_prev = iter._node->_prev;
           ++iter;
           delete delNode;
        }
   }

   // return false if nothing to erase
   bool erase(iterator pos) { 
        if(!empty()){
           DListNode<T>* delNode = pos._node;
           if(pos._node == _head) //means now is point to _head
                _head = pos._node->_next; // let _head point to dummy 
           pos._node->_prev->_next = pos._node->_next;
           pos._node->_next->_prev = pos._node->_prev;
           ++pos;
           delete delNode;
           return true;
        }  
        else {        
            return false; 
        }
   }
   bool erase(const T& x) { 
         if(empty())
            return false;
        for(iterator i = _head ; i!= _head->_prev; ++i ) { 
           // cout << "*i.node = " << i._node << " , x = " << x << endl;
            if(i._node->_data == x) {
                //cout << "i._node->_data" << i._node->_data << endl;
                //cout << "i._node->_data" << *i._node->_data << endl;
                //cout << "i._node->_data" << *i._node << endl;
                erase(i);
                return true;
            }   
        }   
        return false; 
   }

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

   void sort() { 
          size_t count = 0 , nowNode = 0;
          if(!empty()){
              iterator i = _head , tmpIter = _head , tmpPrev , tmpNext;
              //DListNode<T>* minNode = i._node;
              for(i = _head ; i!= _head->_prev; ++i ) { 
                  DListNode<T>* minNode = i._node ,  *tmpNode = _head->_prev;
                  tmpIter = i;
            //      cout << "_head = " << _head << endl;
            //      cout << "_head = " << _head->_data << endl;
            //      cout << "_head->_next = " << _head->_next << endl;
            //      cout << "_head->_prev = " << _head->_prev << endl;
            //      cout << "i._node = " << i._node << endl;
            //      cout << "i._node->_next = " << i._node->_next << endl;
            //      cout << "i._node->_prev = " << i._node->_prev << endl;
                  for(iterator j = i._node->_next ; j!= _head->_prev; ++j ) {
                           // usleep(20000);
                        //cout << "sort count = " << count << endl; 
                        //if( !( minNode->_data < j._node->_data )){
                        if( (j._node->_data) < (minNode->_data )){
                           // usleep(30000);
                     //       cout << "find minNode , i._node->_data = " << i._node->_data << endl;
                     //       cout << "find minNode , j._node->_data = " << j._node->_data << endl;
                            minNode = j._node;
                            tmpIter = j;
                        }
                       //if(i._node->_data != j._node->_data) {
                       //  compare(i,j);
                       // }
                       ++count;    
                  }
                   //    cout << "1. sort count = " << count << endl; 
                   //    cout << "1. nowNode = " << nowNode << endl; 
                        ++nowNode;
                   //    cout << "final minNode = " << minNode->_data << endl << endl;
                  //tmpNode = i._node;
                  tmpPrev = (i._node->_prev);   
                  tmpNext = i._node->_next;   
                  if(minNode == tmpNext._node){
                      tmpNext = i._node;
                  }
                  //tmpNode->_prev = (i._node->_prev);   
                  //tmpNode->_next = i._node->_next;   
                  //cout << "1 tmpNode._node = " << tmpNode << endl;
                  //cout << "1 tmpPrev = " << tmpPrev << endl;
                  //cout << "1 tmpNext = " << tmpNext << endl;
                  //cout << "1 tmpNode->_next = " << tmpNode->_next << endl;
                  //cout << "1 tmpNode->_prev = " << tmpNode->_prev << endl;
                  // change start
                  if(i._node->_next == minNode){
                      i._node->_prev = minNode;
                      i._node->_next = minNode->_next;
                      minNode->_next = i._node;
                      minNode->_prev = tmpPrev._node;
                  } else {
                      i._node->_prev = minNode->_prev;
                      minNode->_prev->_next = i._node;
                      i._node->_next = minNode->_next;
                      minNode->_next->_prev = i._node;
                  }
                  //i._node = minNode;             
     //             cout << "1-1 tmpNode._node = " << tmpNode << endl;
     //             cout << "2 i._node = " << i._node << endl;
     //             cout << "_head = " << _head << endl;
     //             cout << "1-1 tmpNode->_next = " << tmpNode->_next << endl;
     //             cout << "1-1 tmpNode->_prev = " << tmpNode->_prev << endl;
     //             cout << "1-1 tmpPrev = " << tmpPrev._node << endl;
     //             cout << "1-1 tmpNext = " << tmpNext._node << endl;
     //             cout << "1-1 minNode._node = " << minNode << endl;
     //             cout << "1-1 minNode->_next = " << minNode->_next << endl;
     //             cout << "1-1 minNode->_prev = " << minNode->_prev << endl;
     //
                  minNode->_prev = tmpPrev._node; 
                  tmpPrev._node->_next = minNode;
                  minNode->_next = tmpNext._node;   
                  tmpNext._node->_prev = minNode;
          //        cout << "2 i._node = " << i._node << endl;
          //        cout << "2 i._node->_next = " << i._node->_next << endl;
          //        cout << "2 i._node->_prev = " << i._node->_prev << endl;
          //        cout << "2 tmpNode._node = " << tmpNode << endl;
          //        cout << "2 tmpNode->_next = " << tmpNode->_next << endl;
          //        cout << "2 tmpNode->_prev = " << tmpNode->_prev << endl;
                  if(i == _head)
                  {
                      //cout << "i == _head" << endl;
                      _head = minNode;
                      //_head->_prev= minNode->_prev;
                      //_head->_next = minNode->_next;
                  }
                  i = minNode;
          //        cout << "_head = " << _head << endl;
          //        cout << "_head = " << _head->_data << endl;
          //        cout << "_head->_next = " << _head->_next << endl;
          //        cout << "_head->_prev = " << _head->_prev << endl;
          //        cout << "minNode = " << minNode << endl;
          //        cout << "_head = " << minNode->_data << endl;
          //        cout << "_head->_next = " << minNode->_next << endl;
          //        cout << "_head->_prev = " << minNode->_prev << endl;
          //        cout << "3 i._node = " << i._node << endl;
          //        cout << "3 i._node->_next = " << i._node->_next << endl;
          //        cout << "3 i._node->_prev = " << i._node->_prev << endl;
                  //minNode = tmpNode;
                  //i = tmpIter;
                  //_head = minNode; 
                  //minNode = _head;
                  //i = minNode;
                  //i._node = minNode;
                  // change done
                  //i._node = minNode; //recover i 
              }
                       //cout << "2. sort count = " << count << endl; 
          }

   
   }

private:
   DListNode<T>*  _head;  // = dummy node if list is empty
     template <class InputIterator1, class InputIterator2>
     bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
                                   InputIterator2 first2, InputIterator2 last2)
    {
      while (first1!=last1)
      {
        if (first2==last2 || *first2<*first1) return false;
        else if (*first1<*first2) return true;
        ++first1; ++first2;
      }
      return (first2!=last2);
    }
       void compare(iterator i , iterator j ){
    //      l=size(i);
    //      l2=size(j);
    //      for(k=0;k<min{size(i),size(j)},k++)
    //      {   
    //          compare(i[l-k],j[l2-k]);//if(same do next, else do swap)
    //          if(l==k) //do swap
    //          else if(l2==k)//do nothing
    //      }   
       } 

   // [OPTIONAL TODO] helper functions; called by public member functions
};

#endif // DLIST_H
