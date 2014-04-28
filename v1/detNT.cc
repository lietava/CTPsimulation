#include "det.h"
//------------------------------------------------
void DetNT::setDeadTime(INT t)
{
//   deadtime=t;
    deadtime = t + (INT) ((float)t * (detinfo.GetDEbuffer(IDdet)/3700.));
// cout << CalQueue::GlobTime<<" DetNT::setDeadTime " << deadtime << " " << 
// "DE=" << detinfo.GetDEbuffer(IDdet) << " FE=" <<  detinfo.GetFEbuffer(IDdet) << endl; 
}
//----------------------------------------
void DetNT::SendDataL0(INT i,char message)
{
//   cout << "DetNT::SendDataL0 " << i << endl;

   stateBUSY=1;
   if (detinfo.detActive[IDdet] != 0 ){ 
      cout << " Error in DetNT::SendDataL0; exiting " << endl;
      exit(2);
   }
   detinfo.detActive[IDdet] = 1;
   detinfo.SetDEbuffer(detinfo.GetFEbuffer(IDdet),IDdet);
   setDeadTime(4000);
//    cout << CalQueue::GlobTime<< " Det=" << IDdet << " DetNT::SendDataL0 detinfo.FEbuffer: " << 
//    detinfo.GetDEbuffer(IDdet) <<" " <<detinfo.GetFEbuffer(IDdet) << endl;
// This call is to reset detector itself if not receive L1
   CalQueue::PutEntry(i+delayL1-delayL0+1,'r','4'); 
}
//-----------------------------------------------------
void DetNT::ReceiveData(INT i,char message)
{
 detinfo.SetFEbuffer(eventinfo.mult,IDdet);
// cout << "DetNT::ReceiveData:Detector " << message << " data=" <<
// detinfo.GetFEbuffer()  << " " << L1received << endl;  
}

