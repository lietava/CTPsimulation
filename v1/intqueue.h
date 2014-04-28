#ifndef INTQUEUE_H
#define INTQUEUE_H 
#include <iostream>

class intqueue
{
 protected:
  int QSize;
  int *Element;
  int Head,Tail;
 public:
  bool Empty() const;
  bool Full()  const;
  int CountQue(int l1,int l2);
  int CountQueAll();
  int Length();
  void Print();
  intqueue();
  void Setup(int i);
  int back(int i);
  virtual ~intqueue();
  virtual bool Remove(int& TopElem);
  virtual bool Add(const int& NewElem);
};
#endif

