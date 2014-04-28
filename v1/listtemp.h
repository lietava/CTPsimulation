#ifndef LISTTEMP_H
#define LISTTEMP_H
#include "lists.h"

template <class Type>
list<Type>::~list() {
    ListNode *Temp = Head->Next;
    
    while (Temp) {
         Head->Next = Temp->Next;
         delete Temp;
         Temp = Head->Next;
    }
    delete Head;
}

template <class Type>
BOOL list<Type>::Empty() const
{
 return Head->Next == 0 ? TRUE : FALSE;
}  

template <class Type>
void list<Type>::Insert(const Type& NewElement)
{
//zmena
 ListNode *Marker;
 for(Marker = Head; Marker->Next; Marker = Marker->Next);
 Marker->Next = new ListNode(NewElement,Marker->Next);
}

template <class Type>
BOOL list<Type>::DeleteElement(const Type& DelElement)
{
 ListNode *Marker= Head, *Temp;
 
 for(;Marker->Next && Marker->Next->Element != DelElement; Marker = Marker->Next);
 if((Temp = Marker->Next) && Marker->Next->Element == DelElement){
    Marker->Next = Marker->Next->Next;
    delete Temp;
    return TRUE;
 }else
  return FALSE;
}

template <class Type>
void list<Type>::Print() const
{
 cout << "Traversal of list:";
 ListNode *Marker = Head->Next;
 for (;Marker;Marker=Marker->Next)
     cout << Marker->Element << ' ';
 cout << endl;
}

template <class Type>
void list<Type>::Read()
{
 Type NewElement;
 cout << "Input list elemnts. End with " << DefaultVal << "\n";
 cout << "Enter element: ";
 cin >> NewElement;
 while(NewElement != DefaultVal)
 {
  Insert(NewElement);
  cout << "Enter element:";
  cin >> NewElement;
 } 
}

template <class Type>
BOOL list<Type>::TakeFirst(Type& FirstElement)
{
// cout << " 0 next" << Head->Next << endl;  
 if (Head->Next != 0){
   ListNode *First = Head->Next;
   FirstElement= First->Element;
   Head->Next=Head->Next->Next;
   delete First;
//   cout << "2 next" << Head->Next<< endl;
   return TRUE;
 }else
 return FALSE;  
}
#endif

