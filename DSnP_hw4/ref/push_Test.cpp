/****************************************************************************
  FileName     [ push_Test.cpp ]
  PackageName  [ ref ]
  Synopsis     [ Test getNext , popFront , pushFront]
  Author       [ rf0914, Justin ]
  Copyright    [ Copyleft(c) 2014 , NTUEE, Taiwan ]
  Reference    [http://www.ptt.cc/bbs/EE_DSnP/M.1321340639.A.251.html]
  Keyword      [push, recycle , first , getNext]
****************************************************************************/
#include <iostream>
using namespace std;
size_t * _first;


template <class T>
T* getNext(T* p) {
    //T*** _ptr3 = (T***) p;
//    cout << "int next p = " << p << endl;
    T** _ptr= (T**) p;
    //cout << "next **_ptr3 = " << **_ptr3 << endl;
    cout << "next *_ptr = " << *_ptr << endl;
//    cout << "next (T*) p = " << (T*) p << endl;
//    cout << "next (T*) *p = " << (T*) *p << endl;
//    cout << "next (T**) p = " << (T**) p << endl;
 //   cout << "next (T**) *p = " << (T**) *p << endl;
    return *_ptr;
}

template <class T>
T* popFront() {
    T* p = _first;
    cout << "before assign _first = " << _first << endl;
    _first = getNext(p);
    cout << "After assign _first = " << _first << endl;
    return p;
}

template <class T>
void  pushFront(T* p) {
    cout << " push  " << endl;
    T** ptr = (T**) p;
    cout << " push done " << endl;
//    cout << "**ptr = " << **ptr << endl;
    cout << "before assign *ptr = " << *ptr << endl;
    cout << "ptr = " << ptr << endl;
    cout << "p = " << p << endl;
    *ptr = _first;
    cout << "After assign *ptr = " << *ptr << endl;
    cout << "After assign ptr = " << ptr << endl;
    _first = p;

}


int main(void)
{
        size_t a[10] = {1,2,3,4,5,6,7,8,9,10};
        size_t *p[10];

        for(int i=0; i<10; i++){
            p[i]=(size_t*)&a[i];
            cout << "p[" << i << "] = " << p[i] << endl;
        }

//        _first=p[5];
        cout << "_first = " << _first << endl;
        pushFront<size_t>(p[8]);
        pushFront<size_t>(p[6]);
        pushFront<size_t>(p[4]);
        pushFront<size_t>(p[7]);
        pushFront<size_t>(p[0]);
//        pushFront<int>(p[0]);
        

        size_t *pt = popFront<size_t>();
            cout << "pt = " << pt << endl<< endl;
        size_t *pf = popFront<size_t>();
            cout << "pf = " << pf << endl << endl;
        size_t *pd = popFront<size_t>();
            cout << "pd = " << pd << endl << endl;
        size_t *pa = popFront<size_t>();
            cout << "pa = " << pa << endl << endl ;
        size_t *pb = popFront<size_t>();
            cout << "pb = " << pb << endl << endl;

        return 0;
}
