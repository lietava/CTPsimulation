// -*- C++ -*-
#ifndef  trig_h
#define  trig_h 1  
#include <iostream>
#include <fstream> 
#include "intqueue.cc"
#include "TCqueue.cc"
#include "counters.cc"

class Trig
{
 private:
         DetInfo detinfo;
         intqueue PFque;
         int *trigpat;
         TCqueue afterL0classes;
         TCqueue afterL1classes;
         TCqueue trigpatatL0;
         INT trigDT;
         INT delayL2,delayL1,delayL0;
      
         int *TrigClass;      //Define active trigger classes in interaction 
         int **TrigClassDef;  // Define Trigger Classes
         pint *DetCluster;     // Association of DET -> Clusters
         int *TCla2DClu;       // T calsses -> Det clusters
         int *detBusy;
         int *PF2DClu;         // D Cluster -> PF circuit
         int *PF;
         bool getTrigClassesL0();
         bool getTrigClassesL1();
         bool removeBusyClasses();
         bool removePFClassesL0();
         bool removePFClassesL1();
         bool removePFClassesL2();
         void setDetBusy(INT i, char message);
         int numofCLU,numofTC,numofDET,numofTRINP,numofTRINPL0;
         Counters counters;  
         std::ofstream trigmon;
         
 public:
        int stateBUSY;
        int stateDAQfull;
        int stateRARE;
        Trig();
        ~Trig();

        void Setup();
        void Monit();
        void ReceiveL0(INT i, char message);
        void ReceiveL1(INT i, char message);
        void DecideL0(INT i); 
        void DecideL1(INT i, char message);
        void DecideL2(INT i, char message);
        bool SendL1(INT i,char message);        
        void SendL2(INT i,char message);
        void PastFuture(char message);
        void SetBusy();
        void ResetBusy();
        void setDelayL0(INT t);
        void setDelayL1(INT t);
        void setDelayL2(INT t);
        void setTrigDT(INT t);
};
#endif  

