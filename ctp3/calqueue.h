// -*- C++ -*-
#ifndef  CalQueue_h
#define  CalQueue_h 1  
#include <iostream>
#include <cstdio>
#include <string>
#include <list> 
#include <random>

using namespace std;

typedef unsigned int INT;

struct Box{ 
          list<INT> eventtype;
};
//
//std::random_device rd;
static std::mt19937 gen(7);
static std::uniform_real_distribution<> ranlux(0, 1);
double rnlx(){return ranlux(gen);}
class CalQueue
{
 private:
         static Box *que;
	 string ename; // for debugging
 public:
        static long int GlobTime;
        CalQueue();
        ~CalQueue();
        static INT SizeofQue;
        bool DoEntry(INT i, INT* eventt);
        static bool PutEntry(INT i,INT eventt);
	void printEvent(INT t,INT i); // for debuging
};
//int CalQueue::SizeofQue=65536;
INT CalQueue::SizeofQue=1024*1024;
Box* CalQueue::que=new Box[SizeofQue];
long int CalQueue::GlobTime=0;

#endif  

