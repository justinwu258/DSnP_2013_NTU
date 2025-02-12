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
CalcMap GNum::_varMap;


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
  // [TODO] Set the variable 's' in the _varMap to value 'n',
   // no matter the variable 's' exists in _varMap or not
void GNum::setVarVal(const string& s, const GNum& n)
{

    //Example format :  map[key] = newValue;
    //_varMap.insert(pair<string,GNum>(s,n));
    _varMap[s] = n;    
    //cout << "s = " << s << endl;
    //cout << "n._num = " << n._num << endl;
}
   // [TODO] Get the value of variable 's’.
   // If 's' can be found, store the value in 'n' and return true.
   // Otherwise ('s' not found), return false.
bool GNum::getVarVal(const string& s, GNum& n) //{ return true; }
{
    if(!isValidVarName(s))
    {   
       // cout << "string is illegal" << endl;
        return false;
    }
    CalcMap::iterator it;
    it = _varMap.find(s);
    if(it != _varMap.end())
    {
     //   cout << "this key is in Map" << endl;
     //   cout << "_varMap[s]._num" << it->second << endl;
        n._num = it->second._num;
        return true;
    } else {
       // cout << "not find this key in Map" << endl;
        return false;
    }
    
}

   //  
   // [TODO] If 's' is a valid variable name, return "getVarVal(s, n)";
   // else if 's' is a valid number, convert it to GNum and assign to 'n'
bool GNum::getStrVal(const string& s, GNum& n) //{ return true; }
{
    bool neg = false;
    size_t i = 1;
    if(s[0] != '#'){
        return getVarVal(s,n);
    } else if(s[0] == '#' && s.length()==1){ //single #  is not legal
        return false;
    }
   
    if(s[1] == '-' && s.length()<=2){
        return false; 
    }  else if (s[1] == '-'){
        neg = true;
        ++i;
    }
    
    string subStr;
    subStr = s.substr(i); // i = 1 or 2(negtive) 
    for ( ; i < s.length() ; ++i){
        if (!isalnum(s[i]) || s[i] == '_')  // can't allow + , - , * blablabla 
            return false;     // no matter what's base , s[i] can't equal to '_' as digit char

       if (!isdigit(s[i]) )   //not digit
       {//cout << "s[i]-'a' = " << s[i]-'a' << endl;
           if (s[i]-'a'+10 >= GNum::getBase())  // alphet - 'a' lager than base
           {    
                 //cout << "s[i] = " << s[i] << endl;
                 return getVarVal(s,n);
           }
       } 
       else if(s[i]-'0' >= GNum::getBase())  // digit , but digit - '0' larger than base 
       {
                 //cout << "s[i] = " << s[i] << endl;
             return getVarVal(s,n);
       }
       
    }
    
    int tmp_num = 0;
    int mybase = 1;
    for(int i=subStr.length()-1;i >= 0 ;i--)
    {
        //cout << "subStr[" << i << "] = " << subStr[i] << endl;
        if(isdigit(subStr[i]))
            tmp_num += (subStr[i]-'0')*mybase;
        else 
            tmp_num += (subStr[i]-'a'+10)*mybase;
            
        //cout << "mybase = " << mybase << endl;
        //cout << "tmp_num = " << tmp_num << endl;
        mybase *= GNum::getBase();
    }
    if(neg){
        tmp_num *= -1;
    }
    n._num = tmp_num;
    //n._num = stoi(subStr);
    return true;
}

   // [TODO] Print out all the variables in _varMap, one variable per line,
   // in the following format (assume _base = 16) ---
   // a = #9
   // b = #1a
   // kkk = #f1c
void GNum::printVars()   
{
   // cout << "my print" << endl;
   for(CalcMap::iterator it = _varMap.begin(); it != _varMap.end(); ++it){
        cout <<  it->first <<  " = " << it->second << endl;
   }
}
void GNum::resetVarMap() 
{
    _varMap.clear();
}
