/****************************************************************************
  FileName     [ cmdReader.cpp ]
  PackageName  [ cmd ]
  Synopsis     [ Define command line reader member functions ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2007-2014 LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/
#include <cassert>
#include <cstring>
#include "cmdParser.h"

using namespace std;

//----------------------------------------------------------------------
//    Extrenal funcitons
//----------------------------------------------------------------------
void mybeep();
char mygetc(istream&);
ParseChar checkChar(char, istream&);


//----------------------------------------------------------------------
//    Member Function for class Parser
//----------------------------------------------------------------------
void
CmdParser::readCmd()
{
   if (_dofile.is_open()) {
      readCmdInt(_dofile);
      _dofile.close();
   }
   else
      readCmdInt(cin);
}

void
CmdParser::readCmdInt(istream& istr)
{
   resetBufAndPrintPrompt();

   while (1) {
      char ch = mygetc(istr);
      ParseChar pch = checkChar(ch, istr);
      if (pch == INPUT_END_KEY) break;
      switch (pch) {
         case LINE_BEGIN_KEY :
         case HOME_KEY       : moveBufPtr(_readBuf); break;
         case LINE_END_KEY   :
         case END_KEY        : moveBufPtr(_readBufEnd); break;
         case BACK_SPACE_KEY : if(_readBufPtr != _readBuf){
                                   moveBufPtr(_readBufPtr-1); 
                                   deleteChar(); /* TODO */ 
                               } else {
                                   mybeep();
                               }
                               break;
         case DELETE_KEY     : deleteChar(); break;
         case NEWLINE_KEY    : addHistory();
                               cout << char(NEWLINE_KEY);
                               resetBufAndPrintPrompt(); break;
         case ARROW_UP_KEY   : moveToHistory(_historyIdx - 1); break;
         case ARROW_DOWN_KEY : moveToHistory(_historyIdx + 1); break;
         case ARROW_RIGHT_KEY: moveBufPtr(_readBufPtr+1); /* TODO */ break;
         case ARROW_LEFT_KEY : moveBufPtr(_readBufPtr-1); /* TODO */ break;  // cout << '\b';   ok,too
         case PG_UP_KEY      : moveToHistory(_historyIdx - PG_OFFSET); break;
         case PG_DOWN_KEY    : moveToHistory(_historyIdx + PG_OFFSET); break;
         case TAB_KEY        : insertChar(' ', TAB_POSITION - ((_readBufPtr - _readBuf)%TAB_POSITION));
                               /* TODO */ 
                               break;
         case INSERT_KEY     : // not yet supported; fall through to UNDEFINE
         case UNDEFINED_KEY:   mybeep(); break;
         default:  // printable character
            insertChar(char(pch)); break;
      }
      #ifdef TA_KB_SETTING
      taTestOnly();
      #endif
   }
}


// This function moves _readBufPtr to the "ptr" pointer
// It is used by left/right arrowkeys, home/end, etc.
//
// Suggested steps:
// 1. Make sure ptr is within [_readBuf, _readBufEnd].
//    If not, make a beep sound and return false. (DON'T MOVE)
// 2. Move the cursor to the left or right, depending on ptr
// 3. Update _readBufPtr accordingly. The content of the _readBuf[] will
//    not be changed
//
// [Note] This function can also be called by other member functions below
//        to move the _readBufPtr to proper position.
bool
CmdParser::moveBufPtr(char* const ptr)
{
   // TODO...
   //cout << ptr << endl; // print point's address
   // cout << "_readBuf" << _readBuf << endl;
   //  cout << "_readBufEnd" << _readBufEnd << endl;
   if ((ptr > _readBufEnd) || (ptr < _readBuf) ){
        mybeep();
        return false;
   }
   
   while(_readBufPtr > ptr){
        cout << "\b" ;
        _readBufPtr--;
   }
    
   while(_readBufPtr < ptr){  //ptr = _readBufPtr + 1  , so copy the same to screen
        cout << *_readBufPtr;
        _readBufPtr++;
   }
   //cout << "Home test" << endl;
   //cout.flush();
   return true;
}


// [Notes]
// 1. Delete the char at _readBufPtr
// 2. mybeep() and return false if at _readBufEnd
// 3. Move the remaining string left for one character
// 4. The cursor should stay at the same position
// 5. Remember to update _readBufEnd accordingly.
// 6. Don't leave the tailing character.
// 7. Call "moveBufPtr(...)" if needed.
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteChar()---
//
// cmd> This is he command
//              ^
//
bool
CmdParser::deleteChar()
{
   // TODO...
   //cout << endl <<"delete" << endl;
   if(_readBufPtr == _readBufEnd){
        mybeep();
        return false;
   }
 
   char *tmpPtr; 
   int  backtimes = 0;

   tmpPtr = _readBufPtr;

   while(_readBufPtr != _readBufEnd){
        *_readBufPtr = *(_readBufPtr+1);
        cout << *_readBufPtr;
        _readBufPtr++;
        backtimes++;
   }
   cout << " ";  // last one is delete
   *_readBufPtr = 0;
   _readBufPtr = tmpPtr;  // restore cursor position
   _readBufEnd--;  // need do all remain character , and decrease End one pointer address 
   
   for(int i = 0; i < backtimes ; i++){  // Don't forget back to origin position
        cout << "\b" ;
   }
   return true;
}

// 1. Insert character 'ch' at _readBufPtr
// 2. Move the remaining string right for one character
// 3. The cursor should move right for one position afterwards
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling insertChar('k') ---
//
// cmd> This is kthe command
//               ^
//
void
CmdParser::insertChar(char ch, int repeat)
{
   // TODO...
   while(repeat > 0){
       cout << ch ;
       char *tmpPtr;
       int  backtimes = 0;
       _readBufEnd++;
       tmpPtr = _readBufEnd;
        
       while(_readBufPtr != _readBufEnd){
           *(_readBufEnd) = *(_readBufEnd-1);
           _readBufEnd--;    
       } 
       _readBufEnd = tmpPtr;
       *_readBufPtr = ch;
       _readBufPtr++;

       tmpPtr = _readBufPtr;  // record now cursor location
       while(_readBufPtr != _readBufEnd){
           cout << *_readBufPtr;
           _readBufPtr++;
           backtimes++;
       }
       _readBufPtr = tmpPtr;
       for(int i = 0; i < backtimes ; i++){  // Don't forget back to origin position
            cout << "\b" ;
       }
       repeat--;
   }
}

// 1. Delete the line that is currently shown on the screen
// 2. Reset _readBufPtr and _readBufEnd to _readBuf
// 3. Make sure *_readBufEnd = 0
//
// For example,
//
// cmd> This is the command
//              ^                (^ is the cursor position)
//
// After calling deleteLine() ---
//
// cmd>
//      ^
//
void
CmdParser::deleteLine()
{
   // TODO...
   int positionChange = _readBufEnd - _readBufPtr;
   int count = 0;
   //cout << "positionChange = " << positionChange << endl;
   while(positionChange > 0){  // before clean all , need forward to origin end position
        cout << " ";  
        positionChange--;
   }
   while(_readBufEnd != _readBuf){  // finally _readBufEnd would point to  _readBuf
       cout << "\b" << " "<< "\b" ;
       *_readBufEnd = 0;
       _readBufEnd--;
       count++;
   }
   //cout << " " << "\b"; 
   //cout << "count = " << count <<endl;
   //cout << "cmd> "  ;
   _readBufPtr = _readBuf;
   //cout << endl <<  " delete line _reabBuf = " << _readBuf << endl; 
   *_readBufEnd = 0; 
}


// This functions moves _historyIdx to index and display _history[index]
// on the screen.
//
// Need to consider:
// If moving up... (i.e. index < _historyIdx)
// 1. If already at top (i.e. _historyIdx == 0), beep and do nothing.
// 2. If at bottom, temporarily record _readBuf to history.
//    (Do not remove spaces, and set _tempCmdStored to "true")
// 3. If index < 0, let index = 0.
//
// If moving down... (i.e. index > _historyIdx)
// 1. If already at bottom, beep and do nothing
// 2. If index >= _history.size(), let index = _history.size() - 1.
//
// Assign _historyIdx to index at the end.
//
// [Note] index should not = _historyIdx
//
void
CmdParser::moveToHistory(int index)
{
   // TODO...
    //    cout << "index = " << index << endl;
    //    cout << "_historyIdx = "  <<  _historyIdx << endl;
    //    cout << "_historyIdx.size() = " << _history.size() << endl;

   if (((index < 0  && _historyIdx == 0) || 
        ( (index > (signed) _history.size()) && (_historyIdx == _history.size()) ) ))
    {  //_history.size() is vector size, is unsigned
        mybeep();
      //  cout << "beep test " << endl;
        //cout << "_historyIdx = "  <<  _historyIdx << endl;
        //cout << "_historyIdx.size() = " << _history.size() << endl;
        return ;
   }
   //if(index >= -PG_OFFSET+1 && index <= -2){
   if(index < 0){
        index = 0;
   }
   
   //if(index >= _history.size()+2 && index <= _history.size()+PG_OFFSET-1){
   if(index > _history.size() ){
        index = _history.size()-1;
    }
    //    cout << "2. index = " << index << endl;
    //    cout << "2. _historyIdx = "  <<  _historyIdx << endl;
    //    cout << "2. _historyIdx.size() = " << _history.size() << endl;
   _historyIdx = index;
   if(_tempCmdStored == false){
       _history.push_back(_readBuf); // store tmp
       _tempCmdStored = true;
   } 
    
   //retrieveHistory();
   if(index == _history.size()-1) {
       _history.pop_back();   // in tmp command , pop history 
       _tempCmdStored = false;
   }
   retrieveHistory();
   //cout << "in range , _historyIdx = "  <<  _historyIdx << endl;
   //cout << "in range , _historyIdx.size() = " << _history.size() << endl;
}


// This function adds the string in _readBuf to the _history.
// The size of _history may or may not change. Depending on whether 
// there is a temp history string.
//
// 1. Remove ' ' at the beginning and end of _readBuf
// 2. If not a null string, add string to _history.
//    Be sure you are adding to the right entry of _history.
// 3. If it is a null string, don't add anything to _history.
// 4. Make sure to clean up "temp recorded string" (added earlier by up/pgUp,
//    and reset _tempCmdStored to false
// 5. Reset _historyIdx to _history.size() // for future insertion
// 6. Reset _readBufPtr and _readBufEnd to _readBuf
// 7. Make sure *_readBufEnd = 0 ==> _readBuf becomes null string
//

void
CmdParser::addHistory()
{
   // TODO...
   char *tmpPtr , *beginPtr;
   if(_tempCmdStored == true){
       _history.pop_back();
       _tempCmdStored = false;
   }

   if(_readBuf != _readBufEnd)
   {
        _readBufPtr = _readBuf;
        tmpPtr = _readBufPtr;
        beginPtr = _readBuf;
        while(_readBufPtr != _readBufEnd) {
            if(*_readBufPtr == ' '){
                 while(_readBufPtr != _readBufEnd){
                    *_readBufPtr = *(_readBufPtr+1);
                    _readBufPtr++; 
                }
                _readBufPtr = _readBuf;
                _readBufEnd--; // total Buf is decrease
                 //deleteChar();
            } else {
                break;
            }
        }
        //cout << endl <<  "(int)*_readBufEnd = " << (int)*(_readBufEnd) << endl;  // null = 0 , space = 32
        //cout << endl <<  "(int)*_readBufEnd-1 = " << (int)*(_readBufEnd-1) << endl;  // null = 0 , space = 32
        /*if(beginPtr == _readBufEnd){
            cout << " cmp equal" << endl;
        }*/
        while(beginPtr != _readBufEnd) {    
            if(*(_readBufEnd-1) == ' '){       //actual last element store in _readBufEnd-1
                //_readBufPtr = _readBufEnd-1;
                //cout << endl <<"(int)*_readBufPtr = " << (int)*_readBufPtr << endl;
                 *(_readBufEnd-1) = 0;
                 _readBufEnd--;
                //deleteChar();
            } else {
                break;
            }
        }
     
        if(_readBuf == _readBufEnd)  // after clean space , result is empty
        {
            //cout << "after clean space , result is empty, break" << endl;
        } else {
            _history.push_back(_readBuf);
        //    _historyIdx = _history.size(); //2014.10.20 comment out
        }
            cout << "_historyIdx = " << _historyIdx << endl;
            cout << "_history.size() = " << _history.size() << endl << endl;
            _historyIdx = _history.size();  // 2014.10.20 add , no matter push or not , need restore index
            cout << "_historyIdx = " << _historyIdx << endl;
            cout << "_history.size() = " << _history.size() << endl;
            //_historyIdx = _history.size();  // 2014.10.20 add , no matter push or not , need restore index
   }
}


// 1. Replace current line with _history[_historyIdx] on the screen
// 2. Set _readBufPtr and _readBufEnd to end of line
//
// [Note] Do not change _history.size().
//
void
CmdParser::retrieveHistory()
{
   deleteLine();
//   cout << "_history[0] = "  << _history[0] << endl;
//   cout << "_history[1] = "  << _history[1] << endl;
//   cout << "_history[2] = "  << _history[2] << endl;
//   cout << "_history[3] = "  << _history[3] << endl;
//   cout << "in retrieve , _historyIdx = "  <<  _historyIdx << endl;
//   cout << "in retrieve , _historyIdx.size() = " << _history.size() << endl;
   strcpy(_readBuf, _history[_historyIdx].c_str());
   cout << _readBuf;
   _readBufPtr = _readBufEnd = _readBuf + _history[_historyIdx].size();
}
