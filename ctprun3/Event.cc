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
  //cout << x << endl;
  if(x<10.5){
   // MB
   lminps[0]=i;
   lminps[1]=i;
   //printf("LMLM %i \n",i);
   // L0
   l0inps[0]=i; // cpv
   x=rnlx();
   if(x<10.5){
     l0inps[1]=i;  //phs
     //printf("L1L1 %i \n",i);
     x=rnlx();
     if(x<0.05)
     {
         l1inps[0]=i;
         l1inps[1]=i;
     }
   }
  }else{
   // upc
   //x=rnlx();
   //if(x<0.0001){
   //  l0inps[0]=i;
   //}
  }
  // This can be used only when NO UPC otherwise veto is important
  //if(lminps[0] || l0inps[0])
  {
  CalQueue::PutEntry(i+LMLAT,100);
  LMinps.push_back(lminps);
  L0inps.push_back(l0inps);
  L1inps.push_back(l1inps);
  }
}
