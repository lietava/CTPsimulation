#ifndef LISTS_H
#define LISTS_H
#include <iostream>
enum {FALSE,TRUE};
typedef int BOOL;

template <class Type>
class list
{
 protected:
  struct ListNode
  {
   Type Element;
   ListNode *Next;
   ListNode() : Next(0) {}
   ListNode(Type Data, ListNode *Node = 0) :
      Element(Data), Next(Node) {}
  };
 ListNode *Head;
 Type DefaultVal;
public:
       list(): Head(new ListNode) {}
       list(Type defaultval):
           DefaultVal(defaultval), Head(new ListNode) {}
       virtual ~list();
       BOOL Empty() const;
       virtual void Insert(const Type& NewElement);
       virtual BOOL DeleteElement(const Type& DelElement);
       virtual BOOL TakeFirst(Type& FirstElement);
       void Print() const;
       void Read();
};
#endif

