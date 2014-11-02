/****************************************************************************
  FileName     [ calcGNum.cpp ]
  PackageName  [ calc ]
  Synopsis     [ Define member functions for class GNum ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <iostream>
#include <ctype.h>
#include <cassert>
#include "util.h"
#include "calcGNum.h"

// TODO: Initialize the static data members of class GNum
//       (Note: let default _base = 10)
// TODO: Define the member functions of class GNum

int GNum::_base = 10;
CalcMap     _varMap;


string decimal_base(int n , int base ){
    int negFlag = 0 , rem_n;
    string ansStr = "";
    string tmp;
 
    //cout << endl <<"n = " << n << endl;
    if(n == 0){
        ansStr.append("0");
    }
    else if(n < 0){
        n *= -1;
        negFlag = 1;
    }

    while(n!=0){
        rem_n = n%base;
        if(rem_n >= 10){
                tmp = (rem_n-10) + 'a';
                ansStr.insert(0,tmp);
        } else {
                tmp = rem_n + '0';
                ansStr.insert(0,tmp);
        }
        n /= base;

    }
    if(negFlag){
        ansStr.insert(0,"-");
    }
        //ansStr.append("\0"); // terminal string
        //cout << "ansStr = " << ansStr << endl;
    return ansStr;
}

ostream& operator << (ostream& os, const GNum& n)
{
   string baseStr = "";
   if(GNum::getBase() != 10){
          baseStr = decimal_base(n._num , GNum::getBase());
          os << "#" << baseStr; 
    } else { 
        os << "#" <<n._num;
    }
   return os;
}

GNum GNum::operator + (const GNum& n) const { 
    return GNum(_num + n._num); 
}
GNum& GNum::operator += (const GNum& n) {
    _num += n._num;
    return (*this); 
}
GNum GNum::operator - (const GNum& n) const { 
    return GNum(_num - n._num);
}
GNum& GNum::operator -= (const GNum& n) { 
    _num -= n._num;
    return (*this); 
}
GNum GNum::operator * (const GNum& n) const { 
    return GNum(_num * n._num);; 
}
GNum& GNum::operator *= (const GNum& n) { 
    _num *= n._num;
    return (*this); 
}
bool GNum::operator == (const GNum& n) const { 
    if(_num == n._num)
        return true;
    else
        return false;  
}
bool GNum::operator != (const GNum& n) const { 
    if(_num != n._num)
        return true;
    else
        return false;
}
GNum& GNum::operator = (const GNum& n) { 
    _num = n._num;
    return (*this); 
}
