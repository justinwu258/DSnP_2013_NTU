#include <iostream>
#include <vector>
using namespace std;


class Compare
{
public:
	virtual	bool operator()(int,int) const=0;
};

class Less : public Compare
{
   bool operator()(int a1, int a2) const {
   	if(a1 > a2)     // if a1 larger than a2 , change pivot
                return 0;  // pivot will have a minimum  
        else 
                return 1;
   }
};


class Greater : public Compare
{
   bool operator()(int a1, int a2) const {
   	if(a1 < a2)     // if a1 small than a2 , change pivot
                return 0;  // pivot will have a maximum  
        else 
                return 1;
   }
};

void mySwap(int& a1, int& a2){  // use reference , to change input vector value
	int tmp;             
	tmp = a1;
	a1 = a2;
	a2 = tmp;          // change minimum/maxmum to first place

}

void selectionSort(vector<int>& array, const Compare& compare)
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

int main(){
	int i, input;
	vector<int> V;
	cout << "How many numbers? ";

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
        selectionSort(V,Less());	
	cout << "Ascending sort:" << endl;
	for(int j = 0; j<i ;j++)
        {   
                cout << V[j] << " ";    
        }   
	cout << endl;

	selectionSort(V,Greater());    
        cout << "Descending sort:" << endl;
        for(int j = 0; j<i ;j++)
        {   
                cout << V[j] << " ";        
        }   
        //cout << endl;


 

}
