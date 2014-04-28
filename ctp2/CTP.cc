#include "CTP.h"
// Lm,L0,L1 at CTP: 21,32,230
CTP::CTP()
:dbg(0),CTPBusy(CalQueue::SizeofQue+10),
CTPLMBusy(CalQueue::SizeofQue+10),
CTPL0Busy(CalQueue::SizeofQue+10)
{
 //for(int i=0;i<NCLST;i++)CLSTBusy[i]=CalQueue::SizeofQue+10;
 for(int i=0;i<NDET;i++)DetBusy[i]=CalQueue::SizeofQue+10;
 for(int i=0;i<NCLAS;i++){
   countBLM[i]=0;
   countALM[i]=0;
   countBL0[i]=0;
   countAL0[i]=0;
 }
 ClustTRDflag[0]=1;  // Automatic recognition of TRD classes
 ClustTRDflag[1]=0;  // for the moment it is fixed
 Cls2Clust[0]=0;
 Cls2Clust[1]=1;
 Clust2Det[0][0]=1;    //0=TRD
 Clust2Det[0][1]=1;
 Clust2Det[1][0]=0;
 Clust2Det[1][1]=1;
 Cls2Desc[0]=0;
 Cls2Desc[1]=1;
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
bool CTP::GetCTPL0Busy(INT t)
{
 BUSY=L0BUSY;
 return GetBusy(t,CTPL0Busy);
};
bool CTP::GetCTPLMBusy(INT t)
{
 BUSY=LMDTIME;
 return GetBusy(t,CTPLMBusy);
};
bool CTP::GetDetBusy(INT t,INT idet)
{
 BUSY=CLSTDTIME;
 return GetBusy(t,DetBusy[idet]);
};
bool CTP::GetCLSTBusy(INT t,INT iclu)
{
 bool busy=0;
 for(int i=0;i<NDET;i++){
   if(Clust2Det[iclu][i])busy += GetDetBusy(t,i);
 }
 return busy;
}
//---------------------------------------------------------------------------
void CTP::EvaluateLMCondition()
{
 // no inputs control in this simple level
 if(LMinps.empty()){
  cout << "Internal Error" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* lminps = LMinps.front();
 LMinps.pop_front();
 Desc[0] = lminps[0]*lminps[1];
 Desc[1] =!lminps[0] && !lminps[1]; //UPC
}
//---------------------------------------------------------------------------
void CTP::EvaluateL0Condition()
{
 // no inputs control in this simple level
 if(L0inps.empty()){
  cout << "Internal Error" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* l0inps = L0inps.front();
 L0inps.pop_front();
 Desc[0] = 1;
 Desc[1] = l0inps[0]; //UPC
}

//---------------------------------------------------------------------------
bool CTP::EvaluateLMVetoes(INT t,INT iclust)
{
 bool notveto=0;
 bool ctpbusy = GetCTPBusy(t);
 bool clstbusy= GetCLSTBusy(t,iclust);
 bool ctpl0busy= GetCTPL0Busy(t);
 if(ClustTRDflag[iclust]){
   bool ctplmb = GetCTPLMBusy(t);
   //if(!ctpl0busy && !clstbusy){  
   if(!ctplmb && !clstbusy && !ctpl0busy){
   //if(!ctpbusy && !ctplmb && !clstbusy){
   //if(!ctpl0busy && !ctplmb && !clstbusy){
     SetCTPLMBusy(t);
     notveto=1;
   }
 }else{
   if(1){
   //if(!ctpl0busy && !clstbusy){
   //if(!clstbusy1){
   //if(!ctpbusy && !clstbusy){ 
   //if(!ctpbusy && !clstbusy && !ctpl0busy){ 
     notveto=1;
   }  
 }
 if(dbg)printf("%i LM ctpbusy %i clstbusy %i ctpl0busy %i \n",t,ctpbusy,clstbusy,ctpl0busy);
 return notveto;
}
//---------------------------------------------------------------------------
bool CTP::EvaluateL0Vetoes(INT t,INT iclust)
{
 bool notveto=0;
 bool ctpbusy = GetCTPBusy(t);
 bool clstbusy= GetCLSTBusy(t,iclust);
 bool ctpl0busy= GetCTPL0Busy(t);
 if(ClustTRDflag[iclust]){
   //if(1){  
   if(!ctpbusy){
   //if(!ctpbusy && !ctpl0busy && !clstbusy){
     notveto=1;
   }
 }else{
   //if(1){
   if(!ctpbusy && !clstbusy && !ctpl0busy){;
    notveto=1;
   }
 }
 if(dbg)printf("%i L0 ctpbusy %i clstbusy %i ctpl0busy %i \n",t,ctpbusy,clstbusy,ctpl0busy);
 return notveto;
} 
//---------------------------------------------------------------------------
void CTP::ReceiveLMInps(INT i)
{
 // LM Classes init
 INT *cls = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls[j]=0;
 // Descriptors
 EvaluateLMCondition();
 // LM decision
 for(int icls=0;icls<NCLAS;icls++){
    int idesc=Cls2Desc[icls];
    int iclst=Cls2Clust[icls];
    if(Desc[idesc]){
      countBLM[icls]++;
      if(EvaluateLMVetoes(i,iclst)){
        cls[icls]=1;
        countALM[icls]++;
      }
    }
 }
 if(dbg){
    cout << i << " Desc    M: " << Desc[0] <<  " " << Desc[1] << endl;
    cout << i << " Classes M: " << cls[0] << " " << cls[1] << endl;
 }   
 INT sum=0;
 for(int j=0;j<NCLAS;j++)sum+=cls[j];
 if(sum){
    LMclasses.push_back(cls);
    //SetCTPBusy(i);
    //SetCTPL0Busy(i);
    CalQueue::PutEntry(i+LML0TIME,150);
 }   
 else{
  L0inps.pop_front();
  delete [] cls;
 } 
 //if(dbg)cout << "lXinps: " << LMinps.size() << " " << L0inps.size() << endl;
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
 // L0 classes
 INT* cls0 = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls0[j]=0;
 // L0 inputs
 EvaluateL0Condition();
 // L0 decision
 for(int icls=0;icls<NCLAS;icls++){
    if(!clsm[icls]) continue;
    int idesc=Cls2Desc[icls];
    int iclst=Cls2Clust[icls];
    if(Desc[idesc]){
      countBL0[icls]++;
      if(EvaluateL0Vetoes(i,iclst)){
        cls0[icls]=1;
        countAL0[icls]++;
      }
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
    SetCTPBusy(i);
    SetCTPL0Busy(i);
    // Send trigger both for TRD and nonTRD cluster to detectors except TRD
    SendL0Triggers(i,cls0);
 }else{
  //L1inps.pop_front();
  delete [] cls0;
 } 
}
//----------------------------------------------------
void CTP::SendL0Triggers(INT i,INT* cls0)
{
 bool clusttrigs[NDET];
 for(int j=0;j<NDET;j++)clusttrigs[j]=0;
 // Find dets for trigs
 for(int  icls=0;icls<NCLAS;icls++){
  if(cls0[icls]){
    INT iclust=Cls2Clust[icls];
    for(int j=0;j<NDET;j++)if(Clust2Det[iclust][j])clusttrigs[j] |=1;
  } 
 }
 // Send triggers
 for(int j=0;j<NDET;j++)if(clusttrigs[j])CalQueue::PutEntry(i+T2DET,200+j);
 // Register L0 trigger at L0 time
 CalQueue::PutEntry(i+L0L1TIME,151);
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
