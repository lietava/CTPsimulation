// -*- C++ -*-
#ifndef  Universe_h
#define  Universe_h 1  

class Universe
{
 private:
         DetInfo detinfo;
         Event event;
         DetL0 detl01,detl02;
         DetL1 detl1;
         DetNT detTPC;
         Trig trig;
         GDC gdc;         
         CalQueue cq;
	 void doit(INT i, char c, char message);
	 void ResetDetectors(INT i,char message);
	 void DetReceiveData(INT i);
	 void SendL1(INT i, char message); 
 public:
        Universe();
        ~Universe();
        void Setup();
        void Exist();
        void Monitor(long int time, int delta);
};
#endif  

