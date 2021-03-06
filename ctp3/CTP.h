// -*- C++ -*-
#ifndef  CTP_h
#define  CTP_h 1  
#include <iostream>
#include <fstream> 
#include <list> 
#include "calqueue.h"

class CTP
{
 public:
	enum {NCLST=3, NCLAS=3, NDESC=3,NDET=3};
    enum {NMINPS=1,N0INPS=2};
    enum {CTPDTIME=10,LMDTIME=16,L0BUSY=270};
	enum {T2DET=10,CLSTDTIME=10000};
	enum {LML0TIME=15,L0L1TIME=230};
private:
	INT DETBUSY[NDET];
	bool GetBusy(INT t,INT busytype);
	bool dbg;
	// Busy values
        INT CTPBusy;
	INT CTPLMBusy;
	INT CTPL0Busy;
	INT DetBusy[NDET];
	// working variable for calculating busy 
	INT BUSY;
	//
	bool trdclassor;
	INT Desc[NDESC];
	INT Cls2Clust[NCLAS];
	bool ClsVeto[NCLAS];
	double ClsDownScale[NCLAS];
	bool Clust2Det[NCLST][NDET];
	INT Cls2Desc[NCLAS];
	bool ClustTRDflag[NCLST];
	bool LMInps[NMINPS];
	int countBLM[NCLAS];
	int countALM[NCLAS];
	int countBL0[NCLAS];
	int countAL0[NCLAS];
	int countBL1[NCLAS];
	int countAL1[NCLAS];
    int countClust[NCLST];
	list<INT*> LMclasses;
	list<INT*> L0classes;
	list<INT*> L1classes;
	list<INT*> L01inpsCTP;
	list<INT*> L1inpsCTP;
 public:
 	CTP();
	~CTP();
    void SetCTPLMBusy(INT t){CTPLMBusy=t;}
    void SetCTPL0Busy(INT t){CTPL0Busy=t;}
    void SetDetBusy(INT t,INT i){DetBusy[i]=t;}
    void ResetDetBusy(INT ,INT i){DetBusy[i]=CalQueue::SizeofQue+10;}
	// Get
    bool GetCTPL0Busy(INT t);
	bool GetCTPLMBusy(INT t);
	bool GetCLSTBusy(INT t,INT icl);
	bool GetDetBusy(INT t,INT idet);
    void EvaluateLMCondition(INT t);
	void EvaluateL0Condition(INT t);
	void EvaluateL1Condition(INT t);
	bool EvaluateLMVetoes(INT t,INT icls);
	bool EvaluateL0Vetoes(INT t,INT icls);
	bool EvaluateL1Vetoes(INT t,INT icls);
	void CheckLM(INT i);
	void CheckL0(INT i);
	void CheckL1(INT i);
	void SendL0Triggers(INT t,INT* cls);
	void SendL1Triggers(INT t,INT* cls);
    void printCounts(int ncycle,int nn);
};
#endif  

