#include <iostream>
#include "TCqueue.cc"
int main()
{
 pint a,b;
 TCqueue test;
  test.Setup(10);
// test.Add(0);
// test.Remove(a);
 a=new int[3];
 a[0]=1;a[1]=2;a[2]=3;
 for (int i=0; i<1 ;i++){
     cout << i << endl;
     test.Add(a);
 }
 test.Remove(b);
 cout << b[0] << b[1] << b[2] <<endl;
}