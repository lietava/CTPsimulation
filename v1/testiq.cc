#include <iostream>
#include "intqueue.cc"
int main()
{
 int d;
 intqueue test;
 test.Setup(10);

 for (int i=0; i<5; i++){ 
  test.Remove(d);
  test.Add(10+i);
 }
 test.CountQue(5,7);
// test.back(4);
 test.Print();
}