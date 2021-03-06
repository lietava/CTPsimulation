#include "calqueue.h"
CalQueue::CalQueue()
{
 //cout << "ULONG_MAX= " << ULONG_MAX << endl;
 //cout << "unsigned int " << sizeof(unsigned int) << endl;

}
//------------------------------------------
CalQueue::~CalQueue()
{
} 
//------------------------------------------
bool CalQueue::DoEntry(INT i, INT* eventt)
{
 i=i%SizeofQue;
 if ( !(que[i].eventtype.empty()) ){
    *eventt=que[i].eventtype.front();
    que[i].eventtype.pop_front();
    //printEvent(i,*eventt);
    return 1;
 }
 else{
//   cout << "No entry" << endl;
     return 0;
 }
}
//---------------------------------------------
bool CalQueue::PutEntry(INT i, INT eventt)
{
  i=i%SizeofQue;
  que[i].eventtype.push_back(eventt);
  return 1;
 
}
//----------------------------------------------------------
void CalQueue::printEvent(INT t,INT i)
{
 switch(i){
  case 1: ename="COL"; return;break;
  case 100: ename="Check LM at CTP"; break;
  case 101: ename="L0 inps at CTP"; break;
  case 102: ename="L1 inps at CTP"; break;
  case 150: ename="Check L0 at CTP"; break;
  case 151: ename="Check L1 at CTP"; break;
  case 200: ename="L0 at TRD"; break;
  case 201: ename="L0 at nonTRD"; break;
  case 202: ename="L0 at 3rd"; break;
  case 300: ename="TRD busy up"; break;
  case 301: ename="nonTRD busy up"; break;
  default: ename = "UNknown";
 } 
 printf("%06i %03i %s\n",t,i,ename.c_str());
}
