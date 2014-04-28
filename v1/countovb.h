// -*- C++ -*-
#ifndef  counovb_h
#define  countovb_h 1  

class Counters
{
 private:
         int numofDCl;
         int *L0,*L0DT,*L0DTPF;
         int *L1,*L1PF;
         int *L2,*L2PF;
 public:
        Counters();
        ~Counters();
        void Setup(int numofDcl);
        void CountL0(int *TC);
        void CountL0DT(int *TC);
        void CountL0DTPF(int *TC);
        void CountL1(int *TC);
        void CountL2(int *TC);
        void CountL2PF(int *TC);
        void CountL1PF(int *TC);
        void getL0(int **l0, int **l0dt, int **l0dtpf);
        void getL1(int **l1, int **l1pf);
        void getL2(int **l2, int **l2pf);
};
#endif  

