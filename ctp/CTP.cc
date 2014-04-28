#include "CTP.h"
// Lm,L0,L1 at CTP: 21,32,230
CTP::CTP()
:dbg(0),CTPBusy(CalQueue::SizeofQue+10),CTPLMBusy(CalQueue::SizeofQue+10)
{
 for(int i=0;i<NCLST;i++)CLSTBusy[i]=CalQueue::SizeofQue+10;
 for(int i=0;i<NCLAS;i++){
   countBLM[i]=0;
   countALM[i]=0;
   countBL0[i]=0;
   countAL0[i]=0;
 }
 ClsTRDflag[0]=1;  // Automatic recognition of TRD classes
 ClsTRDflag[1]=1;  // for the moment it is fixed
}
//------------------------------------------
CTP::~CTP()
{
}
bool CTP::GetBusy(INT t,INT busytype)
{ 
 bool busy=0;
 if(busytype != (CalQueue::SizeofQue+10)){
   int busyint = busytype + BUSY - t;
   if(t < busytype) busyint = (busyint - CalQueue::SizeofQue);
   busy = (busyint>0);
 }
 //if(dbg)cout << "busy bool= " << busy << " busy " << busytype << " t " << t << endl;
 return busy;
}
bool CTP::GetCTPBusy(INT t)
{
 BUSY=CTPDTIME;
 return GetBusy(t,CTPBusy);
};
bool CTP::GetCTPLMBusy(INT t)
{
 BUSY=LMDTIME;
 return GetBusy(t,CTPLMBusy);
};
bool CTP::GetCLSTBusy(INT t,INT icl)
{
 BUSY=CLSTDTIME;
 return GetBusy(t,CLSTBusy[icl]);
};

//---------------------------------------------------------------------------
void CTP::ReceiveLMInps(INT i)
{
 // no inputs control in this simple level
 if(LMinps.empty()){
  cout << "Internal Error" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* lminps = LMinps.front();
 LMinps.pop_front();
 bool desc0=lminps[0]*lminps[1];
 //bool desc1=lminps[0]*lminps[1];
 bool desc1=!lminps[0] && !lminps[1]; //UPC
 // LM Classes init
 INT *cls = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls[j]=0;
 // Busy
 bool ctpbusy = GetCTPBusy(i);
 bool clstbusy0 = GetCLSTBusy(i,0);
 // LM decision
 if(desc0){
 //if(0){
   countBLM[0]++;
   bool ctplmb = GetCTPLMBusy(i);
   if(dbg){
     cout << i<< "0ctpbusy " << ctpbusy << " " << CTPBusy << " clst " << clstbusy0 <<" "<<CLSTBusy[0];
     cout << " lm " << ctplmb << " " << CTPLMBusy << endl;
   }  
   //if(1){  
   if(!ctplmb && !clstbusy0){
   //if(!ctpbusy && !ctplmb && !clstbusy0){
     //CalQueue::PutEntry(i+T2DET,200);
     SetCTPLMBusy(i);
     cls[0]=1;
     countALM[0]++;
   }
 }  
 // Cluster Busy at L0 level, nonTRD always survive
 bool clstbusy1 = GetCLSTBusy(i,1);
 if(desc1){
   countBLM[1]++;
   if(dbg)cout <<i << "1ctpbusy " << ctpbusy << " "<< CTPBusy <<" clst " << clstbusy1 << " " << CLSTBusy[1]<< endl;
   //if(1){
   //if(!clstbusy1){
   if(!ctpbusy && !clstbusy1){
     //CalQueue::PutEntry(i+T2DET,201);
     cls[1]=1;
     countALM[1]++;
   }  
 }  
 if(dbg){
    cout << i << " Desc    M: " << desc0 <<  " " << desc1 << endl;
    cout << i << " Classes M: " << cls[0] << " " << cls[1] << endl;
 }   
 INT sum=0;
 for(int j=0;j<NCLAS;j++)sum+=cls[j];
 if(sum){
    LMclasses.push_back(cls);
    SetCTPBusy(i);
    //SetCTPLMBusy(i);
    CalQueue::PutEntry(i+LML0TIME,150);
 }   
 else{
  L0inps.pop_front();
  delete [] cls;
 } 
 if(dbg)cout << "lXinps: " << LMinps.size() << " " << L0inps.size() << endl;
}
//---------------------------------------------------------------------------------------
void CTP::CheckL0(INT i)
{
 if(LMclasses.empty()){
  cout << "CheckL0 internal error" << endl;
  exit(3);
 }
 // LM classes
 INT* clsm=LMclasses.front();
 LMclasses.pop_front();
 // L0 inputs
 INT* l0inps = L0inps.front();
 L0inps.pop_front();
 bool desc0=1;
 bool desc1=l0inps[1]; //UPC
 // L0 classes
 INT* cls0 = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls0[j]=0;
 // Busy
 bool ctpbusy = GetCTPBusy(i);
 //ctpbusy=0;
 // L0 decision
 if(dbg)cout <<i << "ctpbusy " << ctpbusy  << " "<< CTPBusy << endl;
 
 if(clsm[0] && desc0){
   countBL0[0]++;
   bool clstbusy0 = GetCLSTBusy(i,0);
   //if(!ctpbusy){
   //if(!ctpbusy && !clstbusy0){
   if(1){
     cls0[0]=1;
     countAL0[0]++;
   }else{
     //cout << "LM killed at L0" << endl;
   }
 }
 if(clsm[1] && desc1){
   countBL0[1]++;
   bool clstbusy1 = GetCLSTBusy(i,1);
   //if(!ctpbusy && !clstbusy1){
   //if(!ctpbusy){
   if(1){
     cls0[1]=1;
     countAL0[1]++;
   }
 }
 if(dbg){
    cout << i << " Classes M: " << clsm[0] << " " << clsm[1] << endl;
    cout << i << " Classes 0: " << cls0[0] << " " << cls0[1] << endl;
 }
 INT sum=0;
 for(int j=0;j<NCLAS;j++)sum+=cls0[j];
 if(sum){
    L0classes.push_back(cls0);
    //SetCTPBusy(i);
    // Send trigger both for TRD and nonTRD cluster to detectors except TRD
    CalQueue::PutEntry(i+T2DET,200);
    CalQueue::PutEntry(i+T2DET,201);
    // Register L0 trigger at L0 time
    CalQueue::PutEntry(i+L0L1TIME,151);
 }else{
  //L1inps.pop_front();
  delete [] cls0;
 } 
}
void CTP::CheckL1(INT i)
{
 //printCounts();
}
void CTP::printCounts()
{
 for(int i=0;i<NCLAS;i++){
  cout << "Class: " << i << endl;
  cout << countBLM[i] << " B LM A " << countALM[i] << endl; 
  cout << countBL0[i] << " B L0 A " << countAL0[i] << endl; 
 }
}
