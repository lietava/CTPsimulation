#include "det.h"

Det::Det()
{
 stateBUSY=0;
 L1received=0;
}
//------------------------------------------
Det::~Det()
{
}
//-----------------------------------------
void Det::SetBusy()
{
 stateBUSY=1;
} 
//-----------------------------------------
void Det::ResetBusy(INT i, char message)
{
 
// cout << CalQueue::GlobTime <<" Det::ResetBusy 1x " << IDdet << " L1received= " << L1received <<
//endl;
 
 if(!L1received){
   stateBUSY=0;
   detinfo.detActive[IDdet]=0;
   detinfo.SetDEbuffer(0,IDdet);
 }
 else{ 
 L1received=0;
// Dostal si received trigger, resetni sa po plnom deadtime.
  CalQueue::PutEntry(i+deadtime-(delayL1-delayL0),'r',message); 
 }
// cout << " DetL0::ResetBusy 2 " << IDdet << " L1received= " << L1received <<
//endl;

}
//-----------------------------------------------
void Det::setDeadTime(INT t)
{
 deadtime=t;
} 
//-----------------------------------------------
void Det::setDelayL0(INT t)
{
 delayL0=t;
} 
//-----------------------------------------------
void Det::setDelayL1(INT t)
{
 delayL1=t;
} 

//------------------------------------------------
void Det::setIDdet(int i)
{
 IDdet=i;
}
