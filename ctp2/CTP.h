// -*- C++ -*-
#ifndef  CTP_h
#define  CTP_h 1  
#include <iostream>
#include <fstream> 
#include <list> 

class CTP
{
 private:
	enum {NCLST=2, NCLAS=2, NDESC=2,NDET=2};
	enum {NMINPS=1,N0INPS=2};
	enum {CTPDTIME=100,LMDTIME=64,L0BUSY=270};
	enum {T2DET=10,CLSTDTIME=10000};
	enum {LML0TIME=15,L0L1TIME=230};
	bool GetBusy(INT t,INT busytype);
	bool dbg;
        INT CTPBusy;
	INT CTPLMBusy;
	INT CTPL0Busy;
	//INT CLSTBusy[NCLST];
	INT DetBusy[NDET];
	INT BUSY;
	INT Desc[NDESC];
	INT Cls2Clust[NCLAS];
	bool Clust2Det[NCLST][NDET];
	INT Cls2Desc[NCLAS];
	bool ClustTRDflag[NCLST];
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
	void SetCTPL0Busy(INT t){CTPL0Busy=t;};
	//void SetClstBusy(INT t,INT i){CLSTBusy[i]=t;};
	void SetDetBusy(INT t,INT i){DetBusy[i]=t;};
	//void SetClstBusy(INT t,INT i){};
	bool GetCTPBusy(INT t);
	bool GetCTPL0Busy(INT t);
	bool GetCTPLMBusy(INT t);
	bool GetCLSTBusy(INT t,INT icl);
	bool GetDetBusy(INT t,INT idet);
	void EvaluateLMCondition();
	void EvaluateL0Condition();
	bool EvaluateLMVetoes(INT t,INT iclust);
	bool EvaluateL0Vetoes(INT t,INT iclust);
	void ReceiveLMInps(INT i);
	void ReceiveL0Inps(INT i);
	void ReceiveL1Inps(INT i);
	void CheckLM(INT i);
	void CheckLM2(INT i);
	void CheckL0(INT i);
	void CheckL1(INT i);
	void SendL0Triggers(INT t,INT* cls);
	void printCounts();
};
#endif  

