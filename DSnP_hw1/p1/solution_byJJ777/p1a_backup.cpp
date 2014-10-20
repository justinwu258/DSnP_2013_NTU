#include <iostream>
#include "p1a.h"
using namespace std;
/*
class P1a 
{
    Public:
        void assign(const string& s);

}
*/
//string _str = "123";
void P1a::assign(const string& s){
    _str = s;    
}


void P1a::print() const
{
    cout << _str << std::endl ;
}

P1a& P1a::append(const P1a& p){ 
   // P1a pp;
    //pp._str.assign(p._str);
    //pp._str.print(); 
//cout << "p._str = " << p._str << endl;
//cout << "_str1 = " << _str << endl;
    //string *tmp = _str;
    _str.append(p._str);	
//cout << "_str2 = " << _str << endl;
    P1a* pp ; 
    pp = this; 
    //pp = &p;
    pp->_str =  _str;
//cout << "pp address = " <<  &pp->_str << endl;
    //pp = (P1a) malloc(sizeof(P1a)) ;
    //P1a p1 = (P1a*)malloc(sizeof(P1a));
    //pp._str = p._str;
//cout << "pp._str = " << pp->_str << endl;
    return *pp; 
}

