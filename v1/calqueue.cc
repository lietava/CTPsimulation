#include "calqueue.h"
CalQueue::CalQueue()
{
// que = new string[SizeofQue];
// for (long int i=0; i < SizeofQue ; i++){
//       que[i].eventtype('0');
//       que[i].message('0');
// } 
}
//------------------------------------------
CalQueue::~CalQueue()
{
} 
//------------------------------------------
bool CalQueue::DoEntry(INT i,char *c, char *m)
{

 if ( que[i].eventtype.TakeFirst(*c) ){
    que[i].message.TakeFirst(*m);
//    que[i].eventid.TakeFirst(*eid);
     return 1;
 }
 else{
//   cout << "No entry" << endl;
     return 0;
 }
}
//---------------------------------------------
bool CalQueue::PutEntry(INT i, char c, char m)
{
    que[i].eventtype.Insert(c);
    que[i].message.Insert(m);
//    que[i].eventid.Insert(eid);
  return 1;
 
}
//----------------------------------------------------------
