#ifndef ALLSIM_H
#define ALLSIM_H
#include <random>
#include <iostream>
using namespace std;


static std::mt19937 gen(7);
static std::uniform_real_distribution<> ranlux(0, 1);
double myrnlx(){return ranlux(gen);}

class AllSim
{
public:
    AllSim();
private:

    //double_t pint=1./800.;
    double_t lm_pint=1./40.;
    double_t l0_pint = 1.;///100.; // relative to lm
    const int LML0time=16;
    const int LMDT=26;
    //
    // Counters
    int lm_inter=0;
    int l0_inter=0;
    int lm_trig1=0;
    int trig2=0;
    int trig12=0;
    int trig12check=0;

};

#endif // ALLSIM_H
