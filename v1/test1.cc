#include <iostream>
#include "TCqueue.cc"
class test
{
 private:
         pint pp;
         TCqueue tt;
 public:
         test();
         void vloz(int i);
         void citaj();
          
};
test::test()
{
 tt.Setup(10);
 pp=new int[3];
}
void test::vloz(int i)
{
 pint pp;
 pp=new int[3];
 pp[0]=1;pp[1]=i;pp[2]=3;
 cout <<"Vloz:"<< pp[0] << pp[1] << pp[2] << endl; 
 tt.Add(pp);
//  pp[0]=0;pp[1]=0;pp[2]=0;
}
void test::citaj()
{
 pint p;
 tt.Remove(p);
 cout << p[0] << p[1] << p[2] << endl; 
 delete [] p;
}
class uni
{
 public:test aa;
        void e();
};
void uni::e()
{
 aa.vloz(33);
 aa.vloz(44);
 aa.citaj();
 aa.citaj();
}
int main()
{
 uni test;
 test.e();
}