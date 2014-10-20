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
//    cout << "_str1 = " << _str << endl;
    _str.append(p._str);	
//    cout << "_str2 = " << _str << endl;
    P1a pp ; 
    pp = p; 
    pp._str = p._str;
    return *this; 
}

