#include "allsim.h"


AllSim::AllSim()
{

    const int SIZE=1024*1024*1;
    const int NDETS=6;
    bool** Universe = new bool*[NDETS];
    for(int i=0; i<NDETS;i++) Universe[i] = new bool[SIZE];
    //
    const int DTdet1=8*40;    //TRD
    const int DTdet2=33*40; // EMC
    double_t lt1=1./(1+lm_pint*DTdet1);
    double_t lt2=1./(1+lm_pint*DTdet2);
    int margin=std::max(DTdet1,DTdet2);

    //
    std::cout << "Rate[MHz]:" << lm_pint*40;
    std::cout << " indep lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;
    //
    for(int i=0;i<SIZE;i++)
    {
        Universe[0][i]=0;
        Universe[1][i]=0;
    }
    for(int i=0;i<SIZE-margin-1;i++)
    {
        if(myrnlx()<lm_pint)
        {
            Universe[0][i]=1;
            lm_inter++;
            if(myrnlx()<l0_pint)
            {
                l0_inter++;
                Universe[1][i+LML0time]=1;
            }
        }
        // 1 not busy
        Universe[2][i]=1;
        // 1 trigger
        Universe[3][i]=0;
        // 2 not busy
        Universe[4][i]=1;
        // 2 trigger
        Universe[5][i]=0;
    }
    for(int i=0;i<SIZE;i++)
    {
        // LM level
        if(Universe[0][i])
        {
            if(Universe[2][i])
            {
                // trigger
                lm_trig1++;
                Universe[3][i]=1;
                //int BUSY = i+LMDT;
                int BUSY = i+DTdet1;
                for(int j=i+1;j<BUSY;j++)Universe[2][j]=0;
            }
        }
        // L0level
        if(Universe[1][i])
        {
            // second det
            if(Universe[4][i])
            {
                trig2++;
                Universe[5][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)Universe[4][j]=0;
            }
            trig12 += Universe[5][i] && Universe[3][i-LML0time];
            if((myrnlx()<lt1)&&(myrnlx()<lt2))trig12check++;
        }

    }
    std::cout << "LT 1:" << (double_t)lm_trig1/lm_inter << " " << lm_trig1<< std::endl;
    std::cout << "LT 2:" << (double_t)trig2/lm_inter << " " << trig2 <<std::endl;
    std::cout << "LT 12:" << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
    std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;

}
