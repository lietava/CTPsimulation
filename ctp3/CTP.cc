#include "CTP.h"
#include "Event.h"
// Lm,L0,L1 at CTP: 21,32,230
CTP::CTP()
:dbg(0),CTPBusy(CalQueue::SizeofQue+10),
CTPLMBusy(CalQueue::SizeofQue+10),
CTPL0Busy(CalQueue::SizeofQue+10)
{
 //for(int i=0;i<NCLST;i++)CLSTBusy[i]=CalQueue::SizeofQue+10;
 // All dets busy ?
 for(int i=0;i<NDET;i++)DetBusy[i]=CalQueue::SizeofQue+10;
 DETBUSY[0]= 500;    // CPV
 DETBUSY[1]=1000;    // PHS
 DETBUSY[2]=1000;    //
 for(int i=0;i<NCLAS;i++){
   countBLM[i]=0;
   countALM[i]=0;
   countBL0[i]=0;
   countAL0[i]=0;
   countBL1[i]=0;
   countAL1[i]=0;
   ClsDownScale[i]=2.;
 }
 for(int i=0;i<NCLST;i++)countClust[i]=0;
 //ClsDownScale[0]=0.001;
 ClustTRDflag[0]=0;  // Automatic recognition of TRD classes
 ClustTRDflag[1]=1;  // for the moment it is fixed
 ClustTRDflag[2]=1;
 // Assignment of classes to clusters
 Cls2Clust[0]=0;
 Cls2Clust[1]=1;
 Cls2Clust[2]=1;
 // Assigment of detectors to clusters
 Clust2Det[0][0]=0;    // CPV
 Clust2Det[0][1]=1;     // PHS
 Clust2Det[0][2]=0;
 Clust2Det[1][0]=1;     //CPV only
 Clust2Det[1][1]=0;
 Clust2Det[1][2]=0;
 Clust2Det[2][0]=1;
 Clust2Det[2][1]=0;
 Clust2Det[2][2]=0;
 // Assignment ofdescriptor to class
 Cls2Desc[0]=0;
 Cls2Desc[1]=1;
 Cls2Desc[2]=2;
 // Class mask like veto
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
 if(busytype != (CalQueue::SizeofQue+10)){  // Detector is not busy
   int busyint = int(busytype + BUSY - t);
   if(t < busytype) busyint = busyint - int(CalQueue::SizeofQue);
   busy = (busyint>0);
 }
 if(1)cout << "busy bool:" << busy << " busytype:" << busytype <<" BUSY:"<<BUSY<<" t " << t << endl;
 return busy;
}

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
 if(1) printf("GetDetBusy: Time %u Det: %u DetBusy: %u\n",t,idet,DetBusy[idet]);
 return GetBusy(t,DetBusy[idet]);
};
bool CTP::GetCLSTBusy(INT t,INT iclu)
{
 if(1)printf("GetClstBusy clst: %u time: %u \n",iclu,t);
 bool busy=0;
 for(INT i=0;i<NDET;i++){
   if(Clust2Det[iclu][i])busy = busy || GetDetBusy(t,i);
 }
 if(1)printf("CLST %u busy: %i Time: %u \n",iclu,busy,t);
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
 Desc[0] = lminps[0];     //MB
 Desc[1] = lminps[0]; //!lminps[0] && !lminps[1]; //UPC
 Desc[2] = 0;//lminps[0];                 //MB
 if(dbg)
 {
 for(int i=0;i<2;i++)printf("LM %i ",lminps[i]);
 printf("%i \n",t);
 //if(Desc[0])printf("LM %i\n",t-lminps[0]);
}
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
 Desc[0] = l0inps[0];
 Desc[1] = l0inps[1];
 Desc[2] = l0inps[0];
 if(dbg)
 {
     for(int i=0;i<2;i++)printf("L0 %i ",l0inps[i]);
     printf("%i \n",t);
    //if(Desc[1])printf("L0: %i\n",t-l0inps[0]);
 }
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
 Desc[2] = 1;
 if(dbg)
 {
     for(int i=0;i<2;i++)printf("L1 %i ",l1inps[i]);
     printf("%i \n",t);
    //if(Desc[2])printf("L1: %i\n",t-l1inps[0]);
 }
 delete [] l1inps;
}
//---------------------------------------------------------------------------
bool CTP::EvaluateLMVetoes(INT t,INT icls)
{
 if(!ClsVeto[icls]) return 0;
 if((ClsDownScale[icls]< rnlx())) return 0;

 INT iclust=Cls2Clust[icls];
 bool notveto=0;
 bool clstbusy= GetCLSTBusy(t,iclust);
 bool ctpl0busy= GetCTPL0Busy(t);
 if(ClustTRDflag[iclust]){
   bool ctplmb = GetCTPLMBusy(t);
   //if(!ctpl0busy && !clstbusy){  
   if(!ctplmb && !clstbusy && !ctpl0busy){
     trdclassor=1;
     notveto=1;
   }
 }else{
   if(1){
   //if(!ctpl0busy && !clstbusy){
   //if(!clstbusy1){
   //if(!ctpbusy && !clstbusy && !ctpl0busy){ 
     notveto=1;
   }  
 }
 if(dbg)printf("%i LM clstbusy %i ctpl0busy %i \n",t,clstbusy,ctpl0busy);
 return notveto;
}
//---------------------------------------------------------------------------
bool CTP::EvaluateL0Vetoes(INT t,INT icls)
{
 INT iclust=Cls2Clust[icls];
 bool notveto=0;
 bool clstbusy= GetCLSTBusy(t,iclust);
 bool ctpl0busy= GetCTPL0Busy(t);
 if(ClustTRDflag[iclust]){
   if(1){  
   //if(!ctpbusy){
   //if(!ctpbusy && !ctpl0busy && !clstbusy){
     notveto=1;
   }
 }else{
   //if(1){
   if(!clstbusy && !ctpl0busy){;
    notveto=1;
   }
 }
 if(dbg)printf("%i  clstbusy %i ctpl0busy %i \n",t,clstbusy,ctpl0busy);
 return notveto;
} 
//---------------------------------------------------------------------------
bool CTP::EvaluateL1Vetoes(INT t,INT icls)
{
 if(dbg)printf("Evaluate L1: time %u clasu:%u :always 1\n",t,icls);
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
 for(INT icls=0;icls<NCLAS;icls++){
    INT idesc=Cls2Desc[icls];
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
 trdclassor=0;
 for(INT icls=0;icls<NCLAS;icls++){
    if(!clsm[icls]) continue;
    INT idesc=Cls2Desc[icls];
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
 int clst_fired[NCLST];
 for(int i=0;i<NCLST;i++)clst_fired[i]=0;
 for(int  icls=0;icls<NCLAS;icls++){
  if(cls0[icls]){
    INT iclust=Cls2Clust[icls];
    clst_fired[iclust]++;
    for(int j=0;j<NDET;j++)if(Clust2Det[iclust][j])clusttrigs[j] |=1;
  } 
 }
 for(int i=0;i<NCLST;i++)if(clst_fired[i]>0)countClust[i]++;
 // Send triggers
 for(INT j=0;j<NDET;j++)if(clusttrigs[j])CalQueue::PutEntry(i+T2DET,200+j);
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
 for(INT icls=0;icls<NCLAS;icls++){
    if(!cls0[icls]) continue;
    INT idesc=Cls2Desc[icls];
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
 for(INT j=0;j<NDET;j++){
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
void CTP::printCounts(int ncycle, int nn)
{
 double bc=(ncycle-1)*nn;
 for(int i=0;i<NCLAS;i++){
  printf("\n Class %3i: \n",i);
  printf("LM B A %8i %8i      %f %f kHz\n",countBLM[i],countALM[i],40000.*countBLM[i]/bc,40000.*countALM[i]/bc);
  printf("L0 B A %8i %8i      %f %f kHz\n",countBL0[i],countAL0[i],40000.*countBL0[i]/bc,40000.*countAL0[i]/bc);
  printf("L1 B A %8i %8i      %f %f kHz\n",countBL1[i],countAL1[i],40000.*countBL1[i]/bc,40000.*countAL1[i]/bc);
 }
 for(int i=0;i <NCLST;i++)
 {
     printf("CLUSTER %2i: ",i);
     printf(" L0: %i \n",countClust[i]);
 }
}
