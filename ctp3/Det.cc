#include "Det.h"
Det::Det():
IDdet(0),deadtime(1000),lasttrigger(CalQueue::SizeofQue+10),name(""),stateBUSY(0)
{}
Det::~Det()
{}
void Det::SetBusy(INT i)
{
 stateBUSY=1;
 CalQueue::PutEntry(i+8,300+IDdet);
 CheckDistance(i);
}
bool Det::CheckDistance(INT t)
{
 bool close=0;
 if(lasttrigger != (CalQueue::SizeofQue+10)){
   int dist = lasttrigger + 260 - t;  // L0L1time
   if(t < lasttrigger) dist = dist - CalQueue::SizeofQue;
   if(dist>=0)close=1;
 }
 if(close){
   cout << "Triggers too close: det "<<IDdet<< " " << t-lasttrigger << " " << t << " "<< lasttrigger << endl;
   exit(3);
 }
 lasttrigger=t;
 return close;
}
