/****************************************************************************
  FileName     [ cirSim.cpp ]
  PackageName  [ cir ]
  Synopsis     [ Define cir simulation functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2008-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cassert>
#include "cirMgr.h"
#include "cirGate.h"
#include "util.h"

using namespace std;
#define myI  10000
#define parallelizeBits 32
//#define debug_fileSim

// TODO: Keep "CirMgr::randimSim()" and "CirMgr::fileSim()" for cir cmd.
//       Feel free to define your own variables or functions

/*******************************/
/*   Global variable and enum  */
/*******************************/
int gSimArray[parallelizeBits][myI]; // create array to store PIs

/**************************************/
/*   Static varaibles and functions   */
/**************************************/

/************************************************/
/*   Public member functions about Simulation   */
/************************************************/
void
CirMgr::randomSim()
{
}

void
CirMgr::fileSim(ifstream& patternFile)
{
    //in CirCmd , file open check is done
    //Direct read patternFile 
    string line;
    int lineCount = 0, tmpLineCount, i,j,k;
    int simArray[parallelizeBits][I]; // create array to store PIs , I: PI's nums
    int simArrayTail = parallelizeBits - 1;
    bool isReadError = false; 
    //initial simArray
    for (j = 0; j < parallelizeBits; ++j){  
        for(i = 0; i < I; ++i){
            simArray[j][i] = 0;
        }    
    }    

    while(getline(patternFile,line)){  // read all data to 2D array
        //cout << "line length = " << line.length() << endl;
        for(i = 0; i < I; ++i) {
            if( (line[i] != '0') && (line[i] != '1')  )
            {
                isReadError = true;
                cout << "Line = "<< lineCount << ", line[" << i  << "] = "<< line[i] << endl;
                break;
            }
            simArray[simArrayTail-lineCount%parallelizeBits][i] ^= line[i] - '0';
            #ifdef debug_fileSim
                cout << "line[" << i << "] = " << line[i];
                cout << "simArray[" << lineCount%parallelizeBits << "][" << i << "] = " << line[i] << ",   ";
            #endif
        }
        #ifdef debug_fileSim
            cout << endl <<"I = " << I << endl;
        #endif
        lineCount++;
        if(lineCount%parallelizeBits == 0) {
            //setPattern(simArray);
            //CirMgr::printSimArray(simArray);
            for(i = 0; i < I; ++i) {
                #ifdef debug_fileSim
                    cout << "Hi, PI" << i << " = " << i+1 << ",  pattern = ";  
                #endif
                for(j = 0; j < parallelizeBits; ++j){
                    #ifdef debug_fileSim
                        cout << "simArray[" << j <<"][" << i <<"] = " <<  simArray[j][i] << ", "; 
                        cout << simArray[j][i];
                        cout << endl;
                    #endif
                }
                    #ifdef debug_fileSim
                        cout << "simArray[" << j <<"][" << i <<"] = " <<  simArray[j][i] << ", "; 
                        cout << endl; 
                    #endif
            }
        }
    }
    if(!isReadError){
        if(lineCount%parallelizeBits != 0 ){ // last bitwise , maybe not have parallelizeBits bit unsigned value
            #ifdef debug_fileSim
                cout << endl <<"tmp I = " << I << endl;
                cout << endl <<"I = " << I << endl;
            #endif
        
            for(i = 0; i < I; ++i ){  // not assigned postion , put 0 
                tmpLineCount = lineCount;
                for(j = 0; j < parallelizeBits - lineCount; ++j) {
                    simArray[simArrayTail-tmpLineCount%parallelizeBits][i] = 0;
                    ++tmpLineCount;
                }
            }
            for(i = 0; i < I; ++i) {
                    #ifdef debug_fileSim
                        cout << ", PI" << i << " = " << i+1 << ",  pattern = ";  
                    #endif
                for(j = 0; j < parallelizeBits; ++j){
                    #ifdef debug_fileSim
                        cout << "i = " << i << ", j = " << j << ", "; 
                        cout << simArray[j][i];
                        cout << endl;
                    #endif
                }
                #ifdef debug_fileSim
                    cout << "   , notFinish" << endl; 
                #endif
            }
        }
    } else {
        cout << "Something error when get input" << endl;
    }
}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/
void
CirMgr::printSimArray(int simArray[][parallelizeBits])
{

}


