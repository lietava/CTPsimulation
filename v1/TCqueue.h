#ifndef TCqueue_H
#define TCqueue_H 
#include <iostream>
typedef int* pint;
class TCqueue
{
 protected:
  int QSize;
  pint *Element;
  int Head,Tail;
 public:
  bool Empty() const;
  bool Full()  const;
  int CountQue();
  int Length();
  TCqueue();
  void Setup(int i);
  virtual ~TCqueue();
  virtual bool Remove(pint& TopElem);
  virtual bool Add(const pint& NewElem);
};
#endif

