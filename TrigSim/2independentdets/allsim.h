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
    double_t pint=1./40.;

};

#endif // ALLSIM_H
