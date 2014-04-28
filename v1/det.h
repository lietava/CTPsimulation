// -*- C++ -*-
#ifndef  det
#define  det 1  
#include <iostream>

class Det
{
 protected:
         int IDdet;
         int data;
         DetInfo detinfo;
         EventInfo eventinfo;
         INT deadtime;  
         INT delayL0;
         INT delayL1;

 public:

        bool L1received;
        int stateBUSY;
        Det();
        ~Det();
        void SetBusy();
        void ResetBusy(INT i,char message);
        
        void setIDdet(int i);
        void setDeadTime(INT t);
        void setDelayL0(INT t);
        void setDelayL1(INT t); 
        

};
//--------------------------------------------------------
//   ReceiveData should be renamed - it receives and send
//   L0 signal
//   SendDataL0 - should be same for all detectors
//                do it in future !
//--------------------------------------------------------
//  Detector L0
class DetL0: public Det
{

 public:
         void ReceiveData(INT i,char message);
         void SendDataL0(INT i, char message);         

};
// Detector L1
class DetL1: public Det
{
 public:
         void ReceiveData(INT i,char message);
         void SendDataL0(INT i, char message);         
};
// Detector NT
class DetNT: public Det
{
 public:
         void setDeadTime(INT t);
         void ReceiveData(INT i,char message);
         void SendDataL0(INT i, char message);         
};

#endif  

