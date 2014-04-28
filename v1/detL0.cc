#include "det.h"
//----------------------------------------
void DetL0::SendDataL0(INT i,char message)
{
 stateBUSY=1; 
 detinfo.detActive[IDdet] = 1;
 if (IDdet == 0) message = '1';
 else message = '2';
// This call is to reset detector itself if not receive L1
 CalQueue::PutEntry(i+delayL1-delayL0+1,'r',message);
// cout << " DetL0::SendDataL0 m= " << message << 
// " " << detinfo.detActive[0] <<detinfo.detActive[1] <<detinfo.detActive[2]
//<<detinfo.detActive[3] <<endl;
}

//----------------------------------------------------------
void DetL0::ReceiveData(INT i,char message)
{
 switch(message){
 case '1': data=eventinfo.inter;break;
 case '2': data=eventinfo.muon;break;
 default  : cout << " DetL0::ReceiveData: unknown detector " << endl;
 }
 if ( data != 0){  
    CalQueue::PutEntry(i+delayL0,'t',message); 
//    cout << "DetL0::ReceiveData:Detector " << message << " data=" << data << endl;
 }
}
