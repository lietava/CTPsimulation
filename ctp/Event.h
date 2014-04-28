// -*- C++ -*-
#ifndef  Event_h
#define  Event_h 1  
#include <cstdlib>
#include <list>
#include <fstream>

class Event
{
 private:
 	enum {LMLAT=20,L0LAT=30};
 public:
        Event();
        ~Event();
        void ProduceTriggers(INT i);
};
list<INT*> LMinps;
list<INT*> L0inps;
list<INT*> L1inps;
#endif  

