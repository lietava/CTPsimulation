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
  // 
  INT* lminps=new INT[2];
  INT* l0inps=new INT[2];
  INT* l1inps=new INT[2];
  for(int j=0;j<2;j++){
   lminps[j]=0;
   l0inps[j]=0;
   l1inps[j]=0;
  }
  double x=rnlx();
  if(x<0.025){
   // MB
   lminps[0]=i;
   lminps[1]=i;
   //printf("LMLM %i \n",i);
   //L0
   l0inps[0]=lminps[0];
   x=rnlx();
   if(x<0.05)
   {
       l0inps[1]=i;
   }
   // L1
   x=rnlx();
   if(x<0.05){
     l1inps[0]=i;
     //printf("L1L1 %i \n",i);
   }
  }
//  else
//  {
//   // upc
//   x=rnlx();
//   if(x<0.0001){
//     l0inps[0]=i;
//   }
//  }

  CalQueue::PutEntry(i+LMLAT,100);
  LMinps.push_back(lminps);
  L0inps.push_back(l0inps);
  L1inps.push_back(l1inps);
}
