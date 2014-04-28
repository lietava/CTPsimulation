// -*- C++ -*-
#ifndef  Event_h
#define  Event_h 1  
#include <cstdlib>
#include <math.h>
#include <fstream>
#include "EventInfo.cc"

class Event
{
 private:
        int participants[416];
        double xsection[416];
        int part; 
        void SetupPhys();
        float generMult();
        int generPart();
 public:
        EventInfo eventinfo;
        Event();
        ~Event();
        void SendEvent(INT i);
};
#endif  

