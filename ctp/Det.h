// -*- C++ -*-
#ifndef  Det_h
#define  Det_h 1  
#include <iostream>

class Det
{
 protected:
         int IDdet;
         INT deadtime;
	 INT lasttrigger;

 public:
	string name;
        int stateBUSY;
        Det();
        ~Det();
	void SetID(int i){IDdet=i;};
        void SetBusy(INT i);
        void ResetBusy(INT i,char message);
	bool CheckDistance(INT t);

};
#endif
