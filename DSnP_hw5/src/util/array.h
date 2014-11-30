/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   Array() : _data(0), _size(0), _capacity(0) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      const T& operator * () const { 
            cout << "const T& operator * () " << endl;
            return (*this); 
      }
      T& operator * () { 
        //cout << "T& operator * () " << endl;
        return (*_node); }
      iterator& operator ++ () {
        //cout << "call iterator& ++" << endl;
        ++_node;   
        return (*this); 
      }
      iterator operator ++ (int) {
        cout << "call iterator ++" << endl;
        iterator iter = (*this);
        ++_node;
        return iter; 
      }
      iterator& operator -- () { 
        --_node;
        return (*this); 
      }
      iterator operator -- (int) {
                
        iterator iter = (*this);
        --_node;
        return iter; 
      }

      iterator operator + (int i) const { 
        cout << "call iterator +" << endl;
        //this->_node += i; 
        return (this->_node + i); 
      }
      iterator& operator += (int i) { 
        cout << "call iterator +=" << endl;
        _node = _node+i;
        return (*this); 
      }

      iterator& operator = (const iterator& i) { 
        cout << "call iterator& =" << endl;
        _node = _node+i; 
        return (*this); 
      }

      bool operator != (const iterator& i) const { 
        //cout << "call iterator& !=" << endl;
            return _node!=i._node; 
      }
      bool operator == (const iterator& i) const { 
        cout << "call iterator& ==" << endl;
            return _node==i._node; 
      }

   private:
      T*    _node;
   };

   // TODO: implement these functions
   iterator begin() const { return iterator(_data); }
   iterator end() const { return iterator(_data+_size); }
   bool empty() const { return (_size == 0); }
   size_t size() const { return _size; }

   T& operator [] (size_t i) { return _data[i]; }
   const T& operator [] (size_t i) const { return _data[i]; }

   void push_back(const T& x) {

        //size_t i = 0;
        if(_size == _capacity)
            expand();
        //start expand
       // if(_capacity == 0) _capacity = 1;
       // else if(_size+1 > _capacity) _capacity *= 2;
       // T* dataSpace = new T[_capacity];
       // //_data = dataSpace;
       // 
       // if(_capacity >= 2){
       //       for(i=0;i < _size;i++) {
       //           dataSpace[i] = _data[i];
       //           //cout << "_data[" << i <<"] = " << _data[i] << endl;
       //       }
       //       //printf("\n\n");
       //       delete [] _data;
       //       _data = dataSpace;
       // } 
       // else{
       //     _data = dataSpace;
       // }
        //end expand 
        //cout << "x = " << x << endl;
              _data[_size++] = x;
        //printf("_capacity = %d , _size = %d\n",_capacity,_size); 
        //++_size;
   }
   void pop_front() { 
        erase(begin());
        /*size_t i = 0;
        if(_size!=0) {
            for(i = 0; i < _size-1 ; i++){
                _data[i] = _data[i+3];
            }
            //--_size;
        } */
   }
   void pop_back() { 
        if(_size!=0) {
            //_data[_size--] = 0;
            --_size;
        }
   }

   bool erase(iterator pos) { 
        if(_size == 0 || pos == end())
            return false;
        for (;pos != _data + _size - 1 ; ++pos){
            *(pos) = *(pos+1);
        }
        --_size;
        return true; 
   }
   bool erase(const T& x) { 
        if(_size == 0)
            return false;
        for(iterator i = _data;i!=_data + _size; i++ ) {

            if(*i._node == x) {
                erase(i);
                return true;
            }
        }
        return false;  
   }

   void clear() {_size=0; }

   // This is done. DO NOT change this one.
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   T*           _data;
   size_t       _size;       // number of valid elements
   size_t       _capacity;   // max number of elements

   // [OPTIONAL TODO] Helper functions; called by public member functions
    void expand(){
        size_t i = 0;
        if(_capacity == 0) _capacity = 1;
        else if(_size+1 > _capacity) _capacity *= 2;
        
        T* dataSpace = new T[_capacity];
        //_data = dataSpace;
            
        if(_capacity >= 2){ 
              for(i=0;i < _size;i++) {
                  dataSpace[i] = _data[i];
                  //cout << "_data[" << i <<"] = " << _data[i] << endl;
              }   
              //printf("\n\n");
              delete [] _data;
              _data = dataSpace;
        }   
        else{
            _data = dataSpace;
        }  
    }
};

#endif // ARRAY_H
