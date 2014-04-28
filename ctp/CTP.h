// -*- C++ -*-
#ifndef  CTP_h
#define  CTP_h 1  
#include <iostream>
#include <fstream> 
#include <list> 

class CTP
{
 private:
	enum {NCLST=6, NCLAS=2};
	enum {NMINPS=1,N0INPS=2};
	enum {CTPDTIME=100,LMDTIME=64,L0BUSY=230};
	enum {T2DET=8,CLSTDTIME=10000};
	enum {LML0TIME=10,L0L1TIME=230};
	bool GetBusy(INT t,INT busytype);
	bool dbg;
        INT CTPBusy;
	INT CTPLMBusy;
	INT CLSTBusy[NCLST];
	INT BUSY;
	bool ClsTRDflag[NCLAS];
	bool LMInps[NMINPS];
	int countBLM[NCLAS];
	int countALM[NCLAS];
	int countBL0[NCLAS];
	int countAL0[NCLAS];
	list<INT*> LMclasses;
	list<INT*> L0classes;
 public:
 	CTP();
	~CTP();
	void SetCTPBusy(INT t){CTPBusy=t;};
	void SetCTPLMBusy(INT t){CTPLMBusy=t;};
	void SetClstBusy(INT t,INT i){CLSTBusy[i]=t;};
	//void SetClstBusy(INT t,INT i){};
	bool GetCTPBusy(INT t);
	bool GetCTPLMBusy(INT t);
	bool GetCLSTBusy(INT t,INT icl);
	void ReceiveLMInps(INT i);
	void ReceiveL0Inps(INT i);
	void ReceiveL1Inps(INT i);
	void CheckLM(INT i);
	void CheckLM2(INT i);
	void CheckL0(INT i);
	void CheckL1(INT i);
	void printCounts();
};
#endif  

