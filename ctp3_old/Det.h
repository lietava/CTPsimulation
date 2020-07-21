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
	 enum {T2DET=10,L0L1TIME=230};
 public:
	string name;
        int stateBUSY;
        Det();
        ~Det();
	void SetID(int i){IDdet=i;};
        void SetBusy(INT i);
        void ResetBusy(INT i);
	bool CheckDistance(INT t);

};
#endif
