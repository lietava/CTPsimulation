#include "CTP.h"
// Lm,L0,L1 at CTP: 21,32,230
CTP::CTP()
:dbg(0),CTPBusy(CalQueue::SizeofQue+10),
CTPLMBusy(CalQueue::SizeofQue+10),
CTPL0Busy(CalQueue::SizeofQue+10)
{
 //for(int i=0;i<NCLST;i++)CLSTBusy[i]=CalQueue::SizeofQue+10;
 for(int i=0;i<NDET;i++)DetBusy[i]=CalQueue::SizeofQue+10;
 DETBUSY[0]=10000;    // TRD
 DETBUSY[1]=20000;    // TPC
 DETBUSY[2]=10000;    //
 for(int i=0;i<NCLAS;i++){
   countBLM[i]=0;
   countALM[i]=0;
   countBL0[i]=0;
   countAL0[i]=0;
   countBL1[i]=0;
   countAL1[i]=0;
   ClsDownScale[i]=2.;
 }
 //ClsDownScale[0]=0.001;
 ClustTRDflag[0]=1;  // Automatic recognition of TRD classes
 ClustTRDflag[1]=0;  // for the moment it is fixed
 ClustTRDflag[2]=1;
 Cls2Clust[0]=0;
 Cls2Clust[1]=1;
 Cls2Clust[2]=2;
 Clust2Det[0][0]=1;    // TRD
 Clust2Det[0][1]=1;
 Clust2Det[0][2]=1;
 Clust2Det[1][0]=0;     //no TRD
 Clust2Det[1][1]=1;
 Clust2Det[1][2]=1;
 Clust2Det[2][0]=1;
 Clust2Det[2][1]=1;
 Clust2Det[2][2]=1;
 Cls2Desc[0]=0;
 Cls2Desc[1]=1;
 Cls2Desc[2]=2;
 ClsVeto[0]=1;
 ClsVeto[1]=1;
 ClsVeto[2]=1;
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
 //BUSY=CLSTDTIME;
 BUSY=DETBUSY[idet];
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
void CTP::EvaluateLMCondition(INT t)
{
 // no inputs control in this simple level
 if(LMinps.empty()){
  cout << "Internal Error" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* lminps = LMinps.front();
 LMinps.pop_front();
 Desc[0] = lminps[0]*lminps[1];     //MB
 Desc[1] =!lminps[0] && !lminps[1]; //UPC
 Desc[2] = Desc[0];                 //MB
 //for(int i=0;i<2;i++)printf("LM %i ",lminps[i]);printf("%i \n",t);
 //if(Desc[0])printf("LM %i\n",t-lminps[0]);
 delete [] lminps;
}
//---------------------------------------------------------------------------
void CTP::EvaluateL0Condition(INT t)
{
 // no inputs control in this simple level
 if(L01inpsCTP.empty()){
  cout << "Internal Error L0 inputs" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* l0inps = L01inpsCTP.front();
 L01inpsCTP.pop_front();
 Desc[0] = 1;         
 Desc[1] = l0inps[0]; 
 Desc[2] = 1;
 //for(int i=0;i<2;i++)printf("L0 %i ",l0inps[i]);printf("%i \n",t);
 //if(Desc[1])printf("L0: %i\n",t-l0inps[0]);
 delete [] l0inps;
}
//---------------------------------------------------------------------------
void CTP::EvaluateL1Condition(INT t)
{
 if(L1inpsCTP.empty()){
  cout << "Internal Error L1 inputs" << endl;
  exit(1);
 }
 // Inputs and descriptors
 INT* l1inps = L1inpsCTP.front();
 L1inpsCTP.pop_front();
 Desc[0] = 1;        
 Desc[1] = 1;         //UPC
 Desc[2] = l1inps[0];
 //for(int i=0;i<2;i++)printf("L1 %i ",l1inps[i]);printf("%i \n",t);
 //if(Desc[2])printf("L1: %i\n",t-l1inps[0]);
 delete [] l1inps;
}
//---------------------------------------------------------------------------
bool CTP::EvaluateLMVetoes(INT t,INT icls)
{
 if(!ClsVeto[icls]) return 0;
 if((ClsDownScale[icls]< rnlx())) return 0;

 int iclust=Cls2Clust[icls];
 bool notveto=0;
 bool ctpbusy = GetCTPBusy(t);
 bool clstbusy= GetCLSTBusy(t,iclust);
 bool ctpl0busy= GetCTPL0Busy(t);
 if(ClustTRDflag[iclust]){
   bool ctplmb = GetCTPLMBusy(t);
   //if(!ctpl0busy && !clstbusy){  
   if(!ctplmb && !clstbusy && !ctpl0busy && !ctpbusy){
   //if(!ctpbusy && !ctplmb && !clstbusy){
   //if(!ctpl0busy && !ctplmb && !clstbusy){
     trdclassor=1;
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
bool CTP::EvaluateL0Vetoes(INT t,INT icls)
{
 int iclust=Cls2Clust[icls];
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
bool CTP::EvaluateL1Vetoes(INT t,INT icls)
{
 return 1;
}
//---------------------------------------------------------------------------
void CTP::CheckLM(INT i)
{
 // LM Classes init
 INT *cls = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls[j]=0;
 // Descriptors
 EvaluateLMCondition(i);
 // LM decision
 bool classor=0;
 trdclassor=0;
 for(int icls=0;icls<NCLAS;icls++){
    int idesc=Cls2Desc[icls];
    if(Desc[idesc]){
      countBLM[icls]++;
      if(EvaluateLMVetoes(i,icls)){
        cls[icls]=1;
    	classor=1;
        countALM[icls]++;
      }
    }
 }
 if(trdclassor){
   SetCTPLMBusy(i);
 }
 INT* l0inps=L0inps.front();
 L0inps.pop_front();
 INT* l1inps=L1inps.front();
 L1inps.pop_front();
 if(classor){
    LMclasses.push_back(cls);
    //SetCTPBusy(i);
    //SetCTPL0Busy(i);
    CalQueue::PutEntry(i+LML0TIME,150);
    L01inpsCTP.push_back(l0inps);
    L01inpsCTP.push_back(l1inps);
 }else{
  //printf("L1 delete");
  delete [] l0inps;
  delete [] l1inps;
  delete [] cls;
 } 
 if(dbg){
    cout << i << " Desc    M: " << Desc[0] <<  " " << Desc[1] << endl;
    cout << i << " Classes M: " << cls[0] << " " << cls[1] << endl;
 }   
 //if(dbg)cout << "lXinps: " << LMinps.size() << " " << L0inps.size() << " " << L1inps.size() << endl;
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
 EvaluateL0Condition(i);
 // L0 decision
 bool classor=0;
 for(int icls=0;icls<NCLAS;icls++){
    if(!clsm[icls]) continue;
    int idesc=Cls2Desc[icls];
    if(Desc[idesc]){
      countBL0[icls]++;
      if(EvaluateL0Vetoes(i,icls)){
        classor=1;
        cls0[icls]=1;
        countAL0[icls]++;
      }
    }
 }
 INT* l1inps=L01inpsCTP.front();
 L01inpsCTP.pop_front();
 if(classor){
    L0classes.push_back(cls0);
    SetCTPBusy(i);
    SetCTPL0Busy(i);
    // Send trigger both for TRD and nonTRD cluster to detectors except TRD
    SendL0Triggers(i,cls0);
    L1inpsCTP.push_back(l1inps);
 }else{
  delete [] l1inps;
  delete [] cls0;
 } 
 delete [] clsm;
 if(dbg){
    cout << i << " Classes M: " << clsm[0] << " " << clsm[1] << endl;
    cout << i << " Classes 0: " << cls0[0] << " " << cls0[1] << endl;
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
//---------------------------------------------------------------
void CTP::CheckL1(INT i)
{
 // L0 classes
 INT* cls0=L0classes.front();
 L0classes.pop_front();
 // L1 classes
 INT* cls1 = new INT[NCLAS];
 for(int j=0;j<NCLAS;j++)cls1[j]=0;
 // L1 inputs
 EvaluateL1Condition(i);
 // L1 decision
 bool classor=0;
 for(int icls=0;icls<NCLAS;icls++){
    if(!cls0[icls]) continue;
    int idesc=Cls2Desc[icls];
    if(Desc[idesc]){
      countBL1[icls]++;
      if(EvaluateL1Vetoes(i,icls)){
        classor=1;
        cls1[icls]=1;
        countAL1[icls]++;
      }
    }
 }
 if(classor){
   //L1classes.push_back(cls1);
 }else{
  //L2inps.pop_front();
 } 
 SendL1Triggers(i,cls1);
 delete [] cls0;
 delete [] cls1;
 if(dbg){
    cout << i << " Classes M: " << cls0[0] << " " << cls0[1] << endl;
    cout << i << " Classes 0: " << cls1[0] << " " << cls1[1] << endl;
 }

}
//----------------------------------------------------
void CTP::SendL1Triggers(INT i,INT* cls1)
{
 bool clusttrigs[NDET];
 for(int j=0;j<NDET;j++)clusttrigs[j]=0;
 // Find dets for trigs
 for(int  icls=0;icls<NCLAS;icls++){
  if(cls1[icls]){
    INT iclust=Cls2Clust[icls];
    for(int j=0;j<NDET;j++)if(Clust2Det[iclust][j])clusttrigs[j] = 1;
  } 
 }
 // Send triggers
 for(int j=0;j<NDET;j++){
    if(clusttrigs[j]){
       continue; 
       //CalQueue::PutEntry(i+T2DET,400+j);
       // Dont need to send L1 trigger
       // Det already busy from L0
    }else{
       // This is trick to avod time alignement of inputs and L1 in ClQueue
       // Instead 'timout signal' is send
       CalQueue::PutEntry(i+T2DET,500+j);
    }   
 }   
 // Register L2 trigger at L2 time
 //CalQueue::PutEntry(i+L0L1TIME,151);
}
void CTP::printCounts()
{
 for(int i=0;i<NCLAS;i++){
  printf("\n Class %3i: \n",i);
  printf("LM B A %8i %8i \n",countBLM[i],countALM[i]);
  printf("L0 B A %8i %8i \n",countBL0[i],countAL0[i]);
  printf("L1 B A %8i %8i \n",countBL1[i],countAL1[i]);
 }
}
