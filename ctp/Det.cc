#include "Det.h"
Det::Det():
IDdet(0),deadtime(1000),lasttrigger(CalQueue::SizeofQue+10),name(""),stateBUSY(0)
{}
Det::~Det()
{}
void Det::SetBusy(INT i)
{
 stateBUSY=1;
 if(IDdet==0)
 CalQueue::PutEntry(i+8,300);
 else 
 CalQueue::PutEntry(i+8,301);
 CheckDistance(i);
}
bool Det::CheckDistance(INT t)
{
 bool close=0;
 if(lasttrigger != (CalQueue::SizeofQue+10)){
   int dist = lasttrigger + 260 - t;  // L0L1time
   if(t < lasttrigger) dist = dist - CalQueue::SizeofQue;
   if(close<0)close=1;
 }
 if(close){
   cout << "Triggers too close: " << t-lasttrigger << endl;
   exit(3);
 }
 lasttrigger=t;
 return close;
}
