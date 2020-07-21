#include "allsim.h"


AllSim::AllSim()
{

    const int SIZE=1024*1024*1;
    const int NDETS=5;
    bool** Universe = new bool*[NDETS];
    for(int i=0; i<NDETS;i++) Universe[i] = new bool[SIZE];
    //
    const int DTdet1=8*40;    //TRD
    const int DTdet2=33*40; // EMC
    double_t lt1=1./(1+pint*DTdet1);
    double_t lt2=1./(1+pint*DTdet2);
    int margin=std::max(DTdet1,DTdet2);
    // Counters
    int inter=0;
    int trig1=0;
    int trig2=0;
    int trig12=0;
    int trig12check=0;
    //
    std::cout << "Rate[MHz]:" << pint*40;
    std::cout << " indep lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;
    //
    for(int i=0;i<SIZE-margin-1;i++)
    {
        //std::cout << i << std::endl;
        double x=myrnlx();
        if(x<pint)
        {
            Universe[0][i]=1;
            inter++;
        }
        else Universe[0][i]=0;
        // 1 not busy
        Universe[1][i]=1;
        // 1 trigger
        Universe[2][i]=0;
        // 2 not busy
        Universe[3][i]=1;
        // 2 trigger
        Universe[4][i]=0;
    }
    for(int i=0;i<SIZE;i++)
    {
        if(Universe[0][i])
        {
            if(Universe[1][i])
            {
                // trigger
                trig1++;
                Universe[2][i]=1;
                int BUSY = i+DTdet1;
                for(int j=i+1;j<BUSY;j++)Universe[1][j]=0;
            }
            // second det
            if(Universe[3][i])
            {
                trig2++;
                Universe[4][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)Universe[3][j]=0;
            }
            trig12 += Universe[2][i] && Universe[4][i];
            if((myrnlx()<lt1)&&(myrnlx()<lt2))trig12check++;
        }

    }
    std::cout << "LT 1:" << (double_t)trig1/inter << " " << trig1<< std::endl;
    std::cout << "LT 2:" << (double_t)trig2/inter << " " << trig2 <<std::endl;
    std::cout << "LT 12:" << (double_t)trig12/inter << " " << trig12<< std::endl;
    std::cout << "LT 12c:" << (double_t)trig12check/inter << " " << trig12check<< std::endl;

}
