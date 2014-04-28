#include "intqueue.h"
bool intqueue::Empty() const
{
 return Head == Tail ? 1 : 0;
}
//-----------------------------
bool intqueue::Full() const
{
// (condition) ? exp1 : exp2  (str 91)
// % = modulo
 return (Tail+1) % QSize == Head ? 1 : 0;
}
//-----------------------------------
intqueue::intqueue():
Head(0),Tail(0)
{}
//--------------------------------------
void intqueue::Setup(int quesize)
{
// cout << "setup " << quesize << endl;
 QSize=quesize+1;
 Element = new int[QSize];
 for (int i=0; i<QSize-1; i++){
//     cout << "i=" << i << endl;
     Add(0);
//     cout << Length() << endl;
 }
}
//--------------------------------------
intqueue::~intqueue()
{
 delete [] Element;
}
//-----------------------------------------
bool intqueue::Remove(int& TopElem)
{
 if ( !Empty() ) {
   TopElem = Element[Head];
//   cout << "Removing at " << Head << " =" << TopElem << endl;
   Head = (Head+1) % (QSize);
   return 1;
 } else {
   cout << "Que is emty: Remove failed."<< endl;
   exit(1);
   return 0;
 }
}
//-------------------------------------------
bool intqueue::Add(const int& NewElem)
{
 if( !Full() ){
   Element[Tail] = NewElem;
//   cout << "Adding at" << Tail << "=" << NewElem << endl;
   Tail = (Tail+1) % (QSize) ;
   return 1;
 } else {
   cout << "Queue is full; Add failed." << endl;
   exit(1);
   return 0;
 }
}
//--------------------------------------------------------
int intqueue::back(int ll)
{
 // i od tailu
 int mark=Head;
 int l=Length();
 for (int i=0; i <  (l-ll-1); i++){
     mark = (mark+1)%QSize;
 }
//  Element[mark] = -122;
 return mark; 
}
//--------------------------------------------------------
int intqueue::CountQue(int l1, int l2)
{
 if( Empty() ){
   cout << "intqueue::CountQue: attempt to count empty que" << endl;
   exit(1);
 }
 if(l1 < 0 || l2 < 0 || l2 >  QSize || l1 > QSize ){
    cout << "intqueue::CountQue: problem , exiting" << endl;
    exit(1);
 }
 int mark=Head;
 for (int i=0; i < l1; i++) mark=(mark+1)%QSize;

 int sum=0;
 for (int i=0; i < l2; i++){
     sum=sum+Element[i];
     mark=(mark+1)%QSize;
//     cout << i << " " << Element[i] << endl;
 }
 return sum; 
}
//-----------------------------------------------------
int intqueue::Length()
{
 int l;
// To be checked
 if ( Head < Tail) l=Tail-Head;
 else l= QSize-Head+Tail; 
 return l;
}
//-----------------------------------------------------
void intqueue::Print()
{
// To be checked
 int ll,k;
 cout << "head= " << Head << endl;
 cout << "tail= " << Tail << endl;
 ll = Length();
 cout << "Length= " << ll << endl;
 for (int i=Head; i != Tail; i=(i+1)%QSize ){
     cout << i << " " << Element[i] << endl;;
 }
}
//-----------------------------------------------------
int intqueue::CountQueAll()
{
// To be checked
 if( Empty() ){
   cout << "intqueue::CountQue: attempt to count empty que" << endl;
   exit(1);
 }
 int sum=0;
 for (int i=Head; i != Tail; i=(i+1)%QSize ){
     cout << i << " " << Element[i] << endl;
 sum=sum+Element[i];
 }
 return sum;
}
