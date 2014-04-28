// -*- C++ -*-
#ifndef  CalQueue_h
#define  CalQueue_h 1  
#include <iostream>
#include <string>
#include "listtemp.h"

using namespace std;

typedef short unsigned int INT;

struct Box{ 
          list<char> eventtype;
          list<char> message;
};

class CalQueue
{
 private:
         static Box *que;
 public:
        static long int GlobTime;
        CalQueue();
        ~CalQueue();
        static int SizeofQue;
        bool DoEntry(INT i,char *c,char *m);
        static bool PutEntry(INT i,char c,char m);
};
#define SizeofQue 65536
Box* CalQueue::que=new Box[SizeofQue];
long int CalQueue::GlobTime=0;
#endif  

