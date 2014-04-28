#include "TCqueue.h"
bool TCqueue::Empty() const
{
 return Head == Tail ? 1 : 0;
}
//-----------------------------
bool TCqueue::Full() const
{
// (condition) ? exp1 : exp2  (str 91)
// % = modulo
 return (Tail+1) % (QSize+1) == Head ? 1 : 0;
}
//-----------------------------------
TCqueue::TCqueue():
Head(0),Tail(0)
{}
//--------------------------------------
void TCqueue::Setup(int quesize)
{
// cout << "setup " << quesize << endl;
 QSize=quesize;
 Element = new pint[QSize+1];
// for (int i=0; i<QSize; i++){
//     cout << "i=" << i << endl;
//     Add(0);
//     cout << Length() << endl;
// }
}
//--------------------------------------
TCqueue::~TCqueue()
{
 delete [] Element;
}
//-----------------------------------------
bool TCqueue::Remove(pint& TopElem)
{
 if ( !Empty() ) {
   TopElem = Element[Head];
   Head = (Head+1) % (QSize+1);
   return 1;
 } else {
   cout << "Que is emty: Remove failed."<< endl;
   exit(2);
   return 0;
 }
}
//-------------------------------------------
bool TCqueue::Add(const pint& NewElem)
{
 if( !Full() ){
   Element[Tail] = NewElem;
   Tail = (Tail+1) % (QSize+1) ;
   return 1;
 } else {
   cout << "Queue is full; Add failed." << endl;
   exit(2);
   return 0;
 }
}
//--------------------------------------------------------
int TCqueue::CountQue()
{
 int sum=0;
  for (int i=0; i<QSize ; i++) if (Element[i] !=0) sum=sum+1;
 return sum; 
}
//-----------------------------------------------------
int TCqueue::Length()
{
 int l;
// To be checked
 if ( Head < Tail) l=Tail-Head;
 else l= QSize+1-Head+Tail; 
 return l;
}

