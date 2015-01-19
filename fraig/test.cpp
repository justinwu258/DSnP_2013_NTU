#include<iostream>
#include<fstream>
using namespace std;
#define gateNum 86000
int main(){
    
    ofstream myFile;
    myFile.open("doSimulSim13auto",ios::out);
    myFile << "cirr -r tests.fraig/sim13.aag" << endl;
    for(int i = 1; i < gateNum; ++i){
        if(i%150 == 0 || i%605 == 0)
            myFile << "cirg " << i << endl;
    }
    myFile << "cirp" << endl;
    myFile << "cirp -fec" << endl;
    myFile << "cirp -n" << endl;
    myFile << "cirsim -f tests.fraig/pattern.13 -o outSim13.log" << endl;
    myFile << "cirp -fec" << endl;
    for(int i = 1; i < gateNum; ++i){
        if(i%150 == 0 || i%605 == 0)
            myFile << "cirg " << i << endl;
    }
    myFile << "usage" << endl;
    myFile << "q -f" << endl;
    myFile.close();
    
    myFile.open("doSimulSim13auto_random",ios::out);
    myFile << "cirr -r tests.fraig/sim13.aag" << endl;
    for(int i = 1; i < gateNum; ++i){
        if(i%150 == 0 || i%605 == 0)
            myFile << "cirg " << i << endl;
    }
    myFile << "cirp" << endl;
    myFile << "cirp -fec" << endl;
    myFile << "cirp -n" << endl;
    myFile << "cirsim -r -o outSim13_random.log" << endl;
    myFile << "cirp -fec" << endl;
    for(int i = 1; i < gateNum; ++i){
        if(i%150 == 0 || i%605 == 0)
            myFile << "cirg " << i << endl;
    }
    myFile << "usage" << endl;
    myFile << "q -f" << endl;
    myFile.close();
}
