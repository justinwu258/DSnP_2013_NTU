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
//#define debug_FEC

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
CirMgr::fileSim(ifstream& patternFile){

    string line;
    int lineCount = 0, tmpLineCount, i,j,k;
    int **simArray; // create array to store PIs , I: PI's nums
    int simArrayTail = parallelizeBits - 1;
    bool isReadError = false;
    unsigned patternValue = 0; 
    
    //initialize array
    simArray = new int *[32];
    for (j = 0; j < parallelizeBits; j++){
        simArray[j] = new int[I];
    }
    for (j = 0; j < parallelizeBits; ++j){  
        for(i = 0; i < I; ++i){
            *(*(simArray+j)+i) = 0;
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
            simArray[simArrayTail-lineCount%parallelizeBits][i] = line[i] - '0';
        }
        if(isReadError) break;
        lineCount++;
        if(lineCount%parallelizeBits == 0) {
            //setPattern(simArray);
            #ifdef debug_fileSim
                CirMgr::printSimArray(simArray);
            #endif
            CirMgr::setPatternValue(simArray);
            CirMgr::initFEC();
        }
    }
    #ifdef debug_fileSim
        cout << endl << endl;
    #endif 
    if(!isReadError){
        if(lineCount%parallelizeBits != 0 ){ // last bitwise , maybe not have parallelizeBits bit unsigned value
            for(i = 0; i < I; ++i ){  // not assigned postion , put 0 
                tmpLineCount = lineCount;
                for(j = 0; j < parallelizeBits - lineCount%parallelizeBits; ++j) {
                    simArray[simArrayTail-tmpLineCount%parallelizeBits][i] = 0;
                    ++tmpLineCount;
                }
            }
            #ifdef debug_fileSim
               CirMgr::printSimArray(simArray);
            #endif
            CirMgr::setPatternValue(simArray);
            CirMgr::initFEC();
        }
    } else {
        cout << "Something error when get input" << endl;
    }
}
//
//void
//CirMgr::fileSim(ifstream& patternFile)
//{
//    //in CirCmd , file open check is done
//    //Direct read patternFile 
//    string line;
//    int lineCount = 0, tmpLineCount, i,j,k;
//    int simArray[parallelizeBits][I]; // create array to store PIs , I: PI's nums
//    int simArrayTail = parallelizeBits - 1;
//    bool isReadError = false;
//    unsigned patternValue = 0; 
//    //initial simArray
//    for (j = 0; j < parallelizeBits; ++j){  
//        for(i = 0; i < I; ++i){
//            simArray[j][i] = 0;
//        }    
//    }    
//
//    while(getline(patternFile,line)){  // read all data to 2D array
//        //cout << "line length = " << line.length() << endl;
//        for(i = 0; i < I; ++i) {
//            if( (line[i] != '0') && (line[i] != '1')  )
//            {
//                isReadError = true;
//                cout << "Line = "<< lineCount << ", line[" << i  << "] = "<< line[i] << endl;
//                break;
//            }
//            simArray[simArrayTail-lineCount%parallelizeBits][i] = line[i] - '0';
//            #ifdef debug_fileSim
//                cout << "line[" << i << "] = " << line[i];
//                cout << "simArray[" << lineCount%parallelizeBits << "][" << i << "] = " << line[i] << ",   ";
//            #endif
//        }
//        #ifdef debug_fileSim
//            cout << endl <<"I = " << I << endl;
//        #endif
//        lineCount++;
//        if(lineCount%parallelizeBits == 0) {
//            //setPattern(simArray);
//            //CirMgr::printSimArray(simArray);
//            for(i = 0; i < I; ++i) {
//                #ifdef debug_fileSim
//                    cout << "Hi, PI" << i << " = " << i+1 << ",  pattern = ";  
//                #endif
//                patternValue = 0;
//                for(j = 0; j < parallelizeBits; ++j){
//                    #ifdef debug_fileSim
//                        cout << "simArray[" << j <<"][" << i <<"] = " <<  simArray[j][i] << ", "; 
//                        cout << simArray[j][i];
//                        cout << endl;
//                    #endif
//                    //bitset<sizeof(patternValue) * 8> s(patternValue);
//                    patternValue += simArray[j][i];
//                    //cout << "(1) i = " << i << "j = " << j << ",  patternValue = " << s << endl;  
//                    if(j != parallelizeBits - 1)
//                        patternValue = patternValue << 1;
//                    //cout << "  i = " << i << "j = " << j << ",  patternValue = " << s << endl;  
//                    
//                }
//                    bitset<sizeof(patternValue) * 8> s(patternValue);
//                    cout << "i = " << i << ",  patternValue = " << s << endl;  
//                    #ifdef debug_fileSim
//                        cout << "simArray[" << j <<"][" << i <<"] = " <<  simArray[j][i] << ", "; 
//                        cout << endl; 
//                    #endif
//            }
//        }
//        
//    }
//    cout << endl << endl;
//    if(!isReadError){
//        if(lineCount%parallelizeBits != 0 ){ // last bitwise , maybe not have parallelizeBits bit unsigned value
//            #ifdef debug_fileSim
//                cout << endl <<"tmp I = " << I << endl;
//                cout << endl <<"I = " << I << endl;
//            #endif
//        
//            for(i = 0; i < I; ++i ){  // not assigned postion , put 0 
//                tmpLineCount = lineCount;
//                    #ifdef debug_fileSim
//                    cout << "put zero , parallelizeBits = " << parallelizeBits <<", lineCount%32 = " << lineCount%32  << endl;
//                    #endif
//                for(j = 0; j < parallelizeBits - lineCount%32; ++j) {
//                    simArray[simArrayTail-tmpLineCount%parallelizeBits][i] = 0;
//                    ++tmpLineCount;
//                    //cout << "put zero " << endl;
//                }
//            }
//            for(i = 0; i < I; ++i) {
//                    #ifdef debug_fileSim
//                        cout << ", PI" << i << " = " << i+1 << ",  pattern = ";  
//                    #endif
//                patternValue = 0;
//                for(j = 0; j < parallelizeBits; ++j){
//                    #ifdef debug_fileSim
//                        cout << "i = " << i << ", j = " << j << ", "; 
//                        cout << simArray[j][i];
//                        cout << endl;
//                    #endif
//                    patternValue += simArray[j][i];
//                    if(j != parallelizeBits - 1)
//                        patternValue = patternValue << 1;
//                }
//                    bitset<sizeof(patternValue) * 8> s(patternValue);
//                    cout << "i = " << i << ",  patternValue = " << s << endl;  
//                #ifdef debug_fileSim
//                    cout << "   , notFinish" << endl; 
//                #endif
//            }
//        }
//    } else {
//        cout << "Something error when get input" << endl;
//    }
//}

/*************************************************/
/*   Private member functions about Simulation   */
/*************************************************/
void
CirMgr::setPatternValue(int **simArray)
{
    int i, j;
    unsigned patternValue = 0, rhs1_value , rhs2_value,poIn_value; 
    #ifdef debug_fileSim
       for(vector<CirPIGate*>::const_iterator it = _piList.begin(); it != _piList.end(); it++){
           cout << " PI = " <<  (*it)->getID() << endl;
       }
    #endif
    for(i = 0; i < I; ++i) {
        patternValue = 0;
        for(j = 0; j < parallelizeBits; ++j){
            patternValue += simArray[j][i];
            if(j != parallelizeBits - 1)
                patternValue = patternValue << 1;
            
        }
        _totalList[_piList[i]->getID()]->_patternValue = patternValue;
        #ifdef debug_fileSim
            bitset<sizeof(patternValue) * 8> s(patternValue); // bitset for debug use
            //cout << "   sizeof(patternValue) = " << sizeof(patternValue);
            //cout << "i = " << i << ",  patternValue = " << s << endl;     
        #endif 
    }
    #ifdef debug_fileSim
        cout << endl;
    #endif 

    //set _dfsList patternValue
   for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){
            if((*it)->_type == "AIG"){
                if(((CirAIGGate*)(*it))->_rhs1_invert) rhs1_value = ~(*it)->_faninList[0]->_patternValue;
                else                                   rhs1_value = (*it)->_faninList[0]->_patternValue;
                if(((CirAIGGate*)(*it))->_rhs2_invert) rhs2_value = ~(*it)->_faninList[1]->_patternValue;
                else                                   rhs2_value = (*it)->_faninList[1]->_patternValue;
                (*it)->_patternValue = rhs1_value & rhs2_value;
                    //((*it)->_faninList[0]->_patternValue ^ ((CirAIGGate*)(*it))->_rhs1_invert) & 
                    //((*it)->_faninList[1]->_patternValue ^ ((CirAIGGate*)(*it))->_rhs2_invert) ;
           // cout << ", (*it)->_patternValue = " << (*it)->_patternValue << endl ;
            #ifdef debug_fileSim
                bitset<sizeof((*it)->_patternValue) * 8> s((*it)->_patternValue); // bitset for debug use
                cout << "   sizeof(patternValue) = " << sizeof((*it)->_patternValue);
                cout << "debug ID = "<< (*it)->getID() << " , gateType = "  <<  (*it)->_type <<  ", *it= "<< (*it);
                cout << ", _isVisited = " << (*it)->_isVisited << ", _pValue = " << s  <<endl;          
                cout << "  ---- fanin ----" << endl;
                for(vector<CirGate*>::const_iterator itG = (*it)->_faninList.begin(); itG != (*it)->_faninList.end(); itG++) {
                   cout << "  *itG = " <<  (*itG) << ",  *itG->ID = " <<  (*itG)->getID() << endl;
                }
            #endif
            } else if((*it)->_type == "PO"){
                if(((CirPOGate*)(*it))->_isInvert) poIn_value = ~(*it)->_faninList[0]->_patternValue;
                else                               poIn_value = (*it)->_faninList[0]->_patternValue;
                (*it)->_patternValue = poIn_value;
                    //((*it)->_faninList[0]->_patternValue ^ ((CirPOGate*)(*it))->_isInvert);
            }
        }
   } 

}

void
CirMgr::initFEC()
{
   //HashMap<PatternKey,CirGate*> fecHash(getHashSize(_dfsList.size()));
   HashMap<PatternKey,IdList*> fecHash(getHashSize(_dfsList.size()));
   int dfs_i = 0;
   for(vector<CirGate*>::const_iterator it = _dfsList.begin(); it != _dfsList.end(); it++){
        if(*it != 0){
            if((*it)->_type == "AIG"){
                PatternKey pKey((*it)->_patternValue);
                //CirGate* d = (*it);
                IdList* tmpIdList;
                #ifdef debug_FEC
                    cout << "checks gate ID = " << (*it)->getID() << endl;
                #endif
                if(fecHash.check((*it)->_patternValue,tmpIdList)){
                    tmpIdList->push_back(_dfsList[dfs_i]->getID()); 
                    #ifdef debug_FEC
                        cout << "fecHash patternValue exist , same gate ID = " << _dfsList[dfs_i]->getID() << endl;
                        cout << "IdList.size() = " << (*tmpIdList).size() << endl; 
                    #endif
                }
                else {
                    IdList* myIdList = new IdList();
                    #ifdef debug_FEC
                        cout << "insert gate to fecHash" << endl;
                    #endif
                    fecHash.insert((pKey),myIdList);
                }
                //cout << " 2. IdList.size() = " << (*tmpIdList).size() << endl; 
            }   
        }
        ++dfs_i;
   }
   #ifdef debug_FEC
       //fecHash.myPrintAll();  
   #endif
}

void
CirMgr::checkFEC()
{

}

void
CirMgr::printSimArray(int **simArray)
{
    int i, j;
    for(i = 0; i < I; ++i) {
        for (j = 0; j < parallelizeBits; ++j){  
            cout << *(*(simArray+j)+i);
        }    
        cout << endl;
    }   
    cout << endl; 
    
}


