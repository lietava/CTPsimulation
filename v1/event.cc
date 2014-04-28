#include "Event.h"

Event::Event()
{
}
//------------------------------------------
Event::~Event()
{
}
//---------------------------------------------
void Event::SendEvent(INT i)
{
 double x;
 char interaction;
 x=drand48();
//  cout << x << endl;
 eventinfo.inter=0;
 eventinfo.muon=0;
 eventinfo.electron=0;
 eventinfo.mult=0;
 if(x>0.0002){ 
    interaction='n';
 }else
 {
  eventinfo.inter=1;
  x=drand48();
  if(x < 0.1625){
     x=drand48(); 
     if( x <  0.76923){     // 0.125/0.1625 
          eventinfo.muon=1;
//  cout <<CalQueue::GlobTime <<" muon" << endl;
     }
     else{  
          eventinfo.electron=1;
          eventinfo.mult=50;
//  cout << "electron" << endl;
     } 
  }
    interaction='y';
//  cout << "Event send " << i << " "<< eventinfo.inter << eventinfo.muon <<
//           eventinfo.electron << endl;
 }

  CalQueue::PutEntry(i,'d','0');
  CalQueue::PutEntry(i,'p',interaction);
} 
