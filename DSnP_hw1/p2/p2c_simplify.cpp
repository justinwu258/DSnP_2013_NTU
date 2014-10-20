#include <iostream>
#include <vector>
using namespace std;



template <class T>
class Compare
{
public:
	virtual	bool operator()(const T&,const T&) const=0;
};

template <class T>
class Less : public Compare<T>
{
   bool operator()(const T& a1,const T& a2) const {
   	if(a1 > a2)     // if a1 larger than a2 , change pivot
                return 0;  // pivot will have a minimum  
        else 
                return 1;
   }
};


template <class T>
class Greater : public Compare<T>
{
   bool operator()(const T& a1,const T& a2) const {
   	if(a1 < a2)     // if a1 small than a2 , change pivot
                return 0;  // pivot will have a maximum  
        else 
                return 1;
   }
};

template <class T>
void mySwap(T& a1, T& a2){  // use reference , to change input vector value
	T tmp;             
	tmp = a1;
	a1 = a2;
	a2 = tmp;          // change minimum/maxmum to first place

}

template <class T>
void selectionSort(vector<T>& array, const Compare<T>& compare)
{
	for (size_t i = 0, n = array.size(); i < n - 1 ; ++i){
		size_t pivot = i;
		for (size_t j = i+1; j < n; ++j){
			if(!compare(array[pivot], array[j]))
				pivot = j;   
		}
		if (pivot != i)
			mySwap(array[pivot], array[i]);
	}
}

template <class T>
void inputFunc(int i,int type,T input, vector<T> V){

	for(int j = 0; j<i ;j++)
        {    
                while( ! ( cin >> input )){ 
                   cout << "input is not valid , please try again " << endl;
                   cin.clear();
                   cin.ignore( numeric_limits<streamsize>::max(), '\n' );
                }   
                V.push_back(input);
        }   
        cout << "Before sort:" << endl;
        for(int j = 0; j<i ;j++)
        {   
                cout << V[j] << " ";    
        }   
        cout <<  endl;
	
	if(type==1){
		selectionSort(&V,Less<string>());
        	cout << "Ascending sort:" << endl;
	}
	else if(type==2){
        	selectionSort(&V,Greater<double>());    
        	cout << "Descending sort:" << endl;
	}
        for(int j = 0; j<i ;j++)
        {   
                cout << V[j] << " ";    
        }   
        cout << endl;	
}

int main(){
	int i;
	double  input;
	string input_str;
	vector<double> V ;
	vector<string> V_str;
	cout << "How many strings? ";

	while( ! ( cin >> i ) || i < 1){ 
          cout << "input is not valid , please try again " << endl;
          cin.clear();
          cin.ignore( numeric_limits<streamsize>::max(), '\n' );
        }  
	inputFunc(i,1,input_str, V_str);
	/*
        for(int j = 0; j<i ;j++)
        {    
                cin >> input_str;
                V_str.push_back(input_str);
        }   
	cout << "Before sort:" << endl;
        for(int j = 0; j<i ;j++)
        {
                cout << V_str[j] << " ";     
        }
	cout <<  endl;
        selectionSort(V_str,Less<string>());     
        cout << "Ascending sort:" << endl;
        for(int j = 0; j<i ;j++)
        {
                cout << V_str[j] << " ";    
        }   
        cout << endl;*/

	
	cout << "How many doubles? ";
	inputFunc(i,2,input,V);
	/*
	while( ! ( cin >> i ) || i < 1){
    	  cout << "input is not valid , please try again " << endl;
    	  cin.clear();
	  cin.ignore( numeric_limits<streamsize>::max(), '\n' );
   	}
	for(int j = 0; j<i ;j++)
	{	
		while( ! ( cin >> input )){ 
 	           cout << "input is not valid , please try again " << endl;
                   cin.clear();
                   cin.ignore( numeric_limits<streamsize>::max(), '\n' );
                }   
		V.push_back(input);
	}
	cout << "Before sort:" << endl;
	for(int j = 0; j<i ;j++)
	{
		cout << V[j] << " ";	                   
	}
	cout <<  endl;
	selectionSort(V,Greater<double>());    
        cout << "Descending sort:" << endl;
        for(int j = 0; j<i ;j++)
        {   
                cout << V[j] << " ";        
        }   
        cout << endl;*/

}
