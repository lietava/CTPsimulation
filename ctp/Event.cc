#include "Event.h"

Event::Event()
{
}
//------------------------------------------
Event::~Event()
{
}
//---------------------------------------------
void Event::ProduceTriggers(INT i)
// This should be done by detectors
{
  // LM trigger
  INT* lminps=new INT[2];
  double x=rnlx();
  if(x<0.01){
   lminps[0]=1;
   lminps[1]=1;
  }else{
   lminps[0]=0;
   lminps[1]=0;
  }
  LMinps.push_back(lminps);
  CalQueue::PutEntry(i+LMLAT,100);
  // L0 trigger
  //float x=rand()/RAND_MAX;
  x=rnlx();
  INT* l0inps=new INT[2];
  if(x>0.0001){
   l0inps[0]=0;
   l0inps[1]=0;
  }else{
   l0inps[0]=1;
   l0inps[1]=1;
  } 
  L0inps.push_back(l0inps);
  //CalQueue::PutEntry(i+L0LAT,101);
  // L1 trigger
  //x=rand()/RAND_MAX;
  //if(x>1.) return;
  //CalQueue::PutEntry(i+230,102);
}
