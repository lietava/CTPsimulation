#include "det.h"
//----------------------------------------
void DetL1::SendDataL0(INT i,char message)
{
// cout << "DetL0: busy= " << stateBUSY << "i=" << i << endl;
 stateBUSY=1;
 detinfo.detActive[IDdet] = 1;
// This call is to reset detector itself if not receive L1
 CalQueue::PutEntry(i+delayL1-delayL0+1,'r','3'); 
}
//-----------------------------------------
void DetL1::ReceiveData(INT i,char message)
{
// Mozno nemusi posielat zakazdym

switch(message){
 case '3': data=eventinfo.electron;break;
 default  : cout << " DetL0::ReceiveData: unknown detector " << endl;
 }
 if ( data != 0){  
    CalQueue::PutEntry(i+delayL0,'u',message);
//    cout << "DetL1::ReceiveData:Detector " << message << " data=" << data << endl;     
 }
}
