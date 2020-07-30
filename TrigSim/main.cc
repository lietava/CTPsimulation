#include "allsim.cpp"
int main()
{
 AllSim ctp;
 ctp.setNLOOP(100);
 ctp.indepDets_2();
 ctp.clustDets_1();
 ctp.clustDets_2a();
 //ctp.PlorFormulas();
 return 0;
}
