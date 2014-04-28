// -*- C++ -*-
#ifndef  Universe_h
#define  Universe_h 1  
#include <cstdlib>
#include <ctime>

class Universe
{
 private:
         //DetInfo detinfo;
         Event event;
         Det *dets;
         //DetL1 detl1;
         //DetNT detTPC;
         CTP ctp;
         //GDC gdc;         
         CalQueue cq;
	 void doit(INT i, INT eventt);
	 //void ResetDetectors(INT i,char message);
	 //void DetReceiveData(INT i);
	 //void SendL1(INT i, char message); 
 public:
        Universe();
        ~Universe();
        void Setup();
        void Exist();
        void Collide(INT Time);
	void DetsGotLMTrigger(INT i);
	void DetsGotL0Trigger(INT i);
        void Monitor(long int time, int delta);
};
#endif  

