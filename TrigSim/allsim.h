#ifndef ALLSIM_H
#define ALLSIM_H
#include <random>
#include <iostream>
static std::mt19937 gen(7);
static std::uniform_real_distribution<> ranlux(0, 1);

class AllSim
{
public:
    AllSim();
    void cleanMemFull();
    void cleanMem(int j);
    void clearCounters();
    int indepDets_1();
    int indepDets_2();
    int clustDets_1();
    int clustDets_2();
    int clustDets_2a();
    void PlorFormulas();
    double rnlx();
    void setNLOOP(int loop){Nloop=loop;};
private:
    int Nloop=100;
    const int SIZE=1024*1024*10;
    const int NDETS=2;
    const int NLMINPS=1;
    const int NL0INPS=1;
    const int NLMTRGS=2;
    const int NL0TRGS=2;
    const int NDETBUS=NDETS;
    // Dead Times
    const int DTdet1=8*40;    //TRD
    const int DTdet2=33*40;//33*40; // EMC
    const int LML0time=16;
    const int LMDT=26;//26;
    //
    // Rates 50kHz  10 kHz
    double_t lm_pint=1./800.;
    double_t l0_pint=1./5.;
    // 1 MHz 10 kHz
    //double_t lm_pint=1./40.;
    //double_t l0_pint = 1.0/100.;///100.; // relative to lm
    // 300 kHz
    //double_t lm_pint=1./133.;
    //double_t l0_pint=1./30.;
    //bool** Universe;
    //
    bool** lm_inputs;
    bool** l0_inputs;
    bool** lm_triggers;
    bool** l0_triggers;
    bool** det_busy;
    bool* lml0busy;
    bool* lmdeadtime;
    //
    // Counters
    int lm_inter=0;
    int l0_inter=0;
    int lm_trig=0;
    int l0_trig=0;
    int trig12=0;
    int trig12check=0;
    int lm_rejected=0;
    int lm_after=0;

};

#endif // ALLSIM_H
