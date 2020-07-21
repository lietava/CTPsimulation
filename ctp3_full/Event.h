// -*- C++ -*-
#ifndef  Event_h
#define  Event_h 1  
#include <cstdlib>
#include <list>
#include <fstream>
#include "calqueue.h"

class Event
{
 private:
 	enum {LMLAT=20,L0LAT=30};
    const double P_LM=0.025;
    const double P_L0=1;
    const double P_L1=1;
 public:
        Event();
        ~Event();
        void ProduceTriggers(INT i);
};
static list<INT*> LMinps;
static list<INT*> L0inps;
static list<INT*> L1inps;

#endif  

