#include "allsim.h"
#include <iomanip>
#include <ctime>
#include <cmath>

AllSim::AllSim()
{
    lm_inputs = new bool*[NLMINPS];
    l0_inputs = new bool*[NL0INPS];
    lm_triggers = new bool*[NLMTRGS];
    l0_triggers = new bool*[NL0TRGS];
    det_busy = new bool*[NDETBUS];
    for(int i=0;i<NLMINPS;i++) lm_inputs[i] = new bool[SIZE];
    for(int i=0;i<NL0INPS;i++) l0_inputs[i] = new bool[SIZE];
    for(int i=0;i<NLMTRGS;i++) lm_triggers[i] = new bool[SIZE];
    for(int i=0;i<NL0TRGS;i++) l0_triggers[i] = new bool[SIZE];
    for(int i=0;i<NDETBUS;i++) det_busy[i] = new bool[SIZE];
    //
    lml0busy= new bool[SIZE];
    lmdeadtime=new bool[SIZE];
}
void AllSim::cleanMemFull()
{
    //
    for(int j=0;j<SIZE;j++)
    {
        lml0busy[j]=0;
        lmdeadtime[j]=0;
        for(int i=0;i<NLMINPS;i++) lm_inputs[i][j] = 0;
        for(int i=0;i<NL0INPS;i++) l0_inputs[i][j] = 0;
        for(int i=0;i<NLMTRGS;i++) lm_triggers[i][j] = 0;
        for(int i=0;i<NL0TRGS;i++) l0_triggers[i][j] = 0;
        for(int i=0;i<NDETBUS;i++) det_busy[i][j] = 0;

    }
}

void AllSim::cleanMem(int j)
{
    j = (j%SIZE);
    lml0busy[j]=0;
    lmdeadtime[j]=0;
    for(int i=0;i<NLMINPS;i++) lm_inputs[i][j] = 0;
    for(int i=0;i<NL0INPS;i++) l0_inputs[i][j] = 0;
    for(int i=0;i<NLMTRGS;i++) lm_triggers[i][j] = 0;
    for(int i=0;i<NL0TRGS;i++) l0_triggers[i][j] = 0;
    for(int i=0;i<NDETBUS;i++) det_busy[i][j] = 0;
}

void AllSim::clearCounters()
{
    lm_inter=0;
    l0_inter=0;
    lm_trig=0;
    l0_trig=0;
    trig12=0;
    trig12check=0;
    lm_rejected=0;
    lm_after=0;
}

int AllSim::indepDets_1()
{
    clearCounters();
    bool** Universe = new bool*[6+3];
    for(int i=0; i< (6+3);i++) Universe[i] = new bool[SIZE];
    //
    double_t lt1=1./(1+lm_pint*DTdet1);
    double_t lt2=1./(1+lm_pint*DTdet2);
    int margin=std::max(DTdet1,DTdet2);

    //
    std::cout << "Rate[MHz]:" << lm_pint*40;
    std::cout << " indep lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;

    for(int i=0;i<SIZE-margin-1;i++)
    {
        if(rnlx()<lm_pint)
        {
            Universe[0][i]=1;
            lm_inter++;
            if(rnlx()<l0_pint)
            {
                l0_inter++;
                Universe[1][i+LML0time]=1;
            }
        }
    }
    for(int i=0;i<SIZE;i++)
    {
        // LM level
        if(Universe[0][i])
        {
            if(!Universe[2][i])
            {
                // trigger
                lm_trig++;
                Universe[3][i]=1;
                //int BUSY = i+LMDT;
                int BUSY = i+DTdet1;
                for(int j=i+1;j<BUSY;j++)Universe[2][j]=1;
            }
        }
        // L0level
        if(Universe[1][i])
        {
            // second det
            if(!Universe[4][i])
            {
                l0_trig++;
                Universe[5][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)Universe[4][j]=1;
            }
            trig12 += Universe[5][i] && Universe[3][i-LML0time];
            if((rnlx()<lt1)&&(rnlx()<lt2))trig12check++;
        }


    }
    std::cout << "LT 1:" << (double_t)lm_trig/lm_inter << " " << lm_trig<< std::endl;
    std::cout << "LT 2:" << (double_t)l0_trig/lm_inter << " " << l0_trig <<std::endl;
    std::cout << "LT 12:" << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
    std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;
    return 0;
}
/**
 * @brief AllSim::indepDets_2
 * @return
 * as indep_dets_1 but with more code structure
 */
int AllSim::indepDets_2()
{
    clock_t begin=clock();
    clearCounters();
    cleanMemFull();
    //
    double_t lt1=1./(1+lm_pint*DTdet1);
    double_t lt2=l0_pint/(1+lm_pint*l0_pint*DTdet2);
    //int margin=std::max(DTdet1,DTdet2);
    int NLOOP=Nloop*SIZE;
    //
    std::cout << "Independent dets. Rate[MHz]:" << lm_pint*40;
    std::cout << " independent rates lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;
    std::cout << "NLOOP:"<<NLOOP<<std::endl;
    for(int i=0;i<NLOOP;i++)
    {
        if(rnlx()<lm_pint)
        {
            lm_inputs[0][i%SIZE]=1;
            lm_inter++;
            if(rnlx()<l0_pint)
            {
                l0_inter++;
                l0_inputs[0][(i+LML0time)%SIZE]=1;
            }
            else
                l0_inputs[0][(i+LML0time)%SIZE]=0;

        }
        else
            lm_inputs[0][i%SIZE]=0;
        //int BUSYclst = det_busy[0][i%SIZE]  || det_busy[1][i%SIZE];
        int BUSYclst1 = det_busy[0][i%SIZE];
        int BUSYclst2 = det_busy[1][i%SIZE];
        // LM level
        if(lm_inputs[0][i%SIZE])
        {
            if(!BUSYclst1)
            {
                // TRD trigger
                lm_trig++;
                lm_triggers[0][(i+LML0time)%SIZE]=1;
                //int BUSY = i+LML0time;
                int BUSY = i+DTdet1;
                for(int j=i+1;j<BUSY;j++)det_busy[0][j%SIZE]=1;
            }
        }
        // L0levelq
        if(l0_inputs[0][i%SIZE])
        {
            // second det
            if(!BUSYclst2)
            {
                l0_trig++;
                l0_triggers[0][i%SIZE]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)det_busy[1][j%SIZE]=1;
            }
            //std::cout <<"New L0:"<< i << std::endl;
            //for(int k=i-5-LML0time;k<i+10;k++) std::cout << k << " " <<lm_inputs[0][k-LML0time] << l0_inputs[0][k] << lm_triggers[0][k]<<l0_triggers[0][k]<<std::endl;
            trig12 += l0_triggers[0][i%SIZE] && lm_triggers[0][i%SIZE];
            //if((rnlx()<lt1)&&(rnlx()<lt2))trig12check++;
        }
        cleanMem(i-1);
    }
    clock_t end=clock();
    double_t elapsed_secs=double_t(end-begin)/CLOCKS_PER_SEC;
    std::cout << "Time[secs]:"<< elapsed_secs<<std::endl;
    std::cout << "% TRD of LM rate " << (double_t)lm_trig/lm_inter << " " << lm_trig<< std::endl;
    std::cout << "% EMC of LM rate " << (double_t)l0_trig/lm_inter << " " << l0_trig <<std::endl;
    std::cout << "% T&E of LM rate " << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
   // std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;
    return 0;
}
/**
 * @brief AllSim::clustDets_1
 * @return
 * One cluster with TRD and EMC
 */
int AllSim::clustDets_1()
{
    cleanMemFull();
    clearCounters();
    //
    double_t lt1=1./(1+lm_pint*DTdet1);
    double_t lt2=l0_pint/(1+lm_pint*l0_pint*(DTdet2+0));
    //double_t lt3=l0_pint*(1.-lm_pint*LMDT)/(1+lm_pint*l0_pint*(DTdet2-LMDT));
    double_t f=1./(1+lm_pint*LMDT);
    double_t lt3=f*l0_pint/(1+f*lm_pint*l0_pint*(DTdet2));
    double_t lt4=l0_pint/(1+l0_pint*lm_pint*(DTdet2+LML0time)+(1-l0_pint)*lm_pint*LMDT);

    //int margin=std::max(DTdet1,DTdet2);

    //
    std::cout << "One cluster Rate[MHz]:" << lm_pint*40;
    std::cout << " independent rates lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;
    std::cout << "lt with 2 levels:" << lt3 << " lt with DTLM" << lt4 << std::endl;
    int i=0;
    for(int i2=0;i2<Nloop;i2++)
    for(int i1=0;i1<SIZE;i1++)
    {
        i=(i+1)%SIZE;
        //std::cout << i << std::endl;
        if(rnlx()<lm_pint)
        {
            lm_inputs[0][i]=1;
            lm_inter++;
            if(rnlx()<l0_pint)
            {
                l0_inter++;
                l0_inputs[0][(i+LML0time)%SIZE]=1;
            }
            else
                l0_inputs[0][(i+LML0time)%SIZE]=0;
        }
        int BUSYclst = det_busy[0][i]  || det_busy[1][i];
        //int BUSYclst = det_busy[0][i];
        //int BUSYclst2 = det_busy[1][i];
        // LM level
        if(lm_inputs[0][i])
        {
            if(!BUSYclst && !lmdeadtime[i])
            //if(!BUSYclst)
            {
                // TRD trigger
                //lm_trig++;
                lm_triggers[0][(i+LML0time)%SIZE]=1;
                // pretect TRD
                int BUSY = i+LMDT;
                for(int j=i+1;j<BUSY;j++)lmdeadtime[j%SIZE]=1;
                // forbid triggers for emc
                //BUSY=i+LML0time;
                //for(int j=i+1;j<BUSY;j++)lml0busy[j]=1;
            }
            else
                lm_triggers[0][(i+LML0time)%SIZE]=0;

        }
        // L0level

        if(l0_inputs[0][i] && lm_triggers[0][i])
        {
            //if(!BUSYclst)
            {
                l0_trig++;
                trig12++;
                l0_triggers[0][i]=1;
                //l0_triggers[1][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)det_busy[1][j%SIZE]=1;
                BUSY=i+DTdet1;
                for(int j=i+1;j<BUSY;j++)det_busy[0][j%SIZE]=1;
            }
            //trig12 += Universe[5][i] && Universe[3][i-LML0time];
            if((rnlx()<lt1)&&(rnlx()<lt2))trig12check++;
        }
        cleanMem(i-1);
        //std::cout << std::setw(5) << i <<std::dec<< " INPS:"<< lm_inputs[0][i]<<l0_inputs[0][i];
        //std::cout <<" Trigs:"<< lm_triggers[0][i]<<l0_triggers[0][i];
        //std::cout <<" busy:"<<!BUSYclst<<(!lml0busy[i])<< std::endl;

    }
    std::cout << "% TRD of LM rate " << (double_t)lm_trig/lm_inter << " " << lm_trig<< std::endl;
    std::cout << "% EMC of LM rate " << (double_t)l0_trig/lm_inter << " " << l0_trig <<std::endl;
    std::cout << "% T&E of LM rate " << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
    //std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;
    return 0;
}
/**
 * @brief AllSim::clustDets_2
 * @return
 * Two clusters:
 * - TRD and EMC
 * - TRD
 */
int AllSim::clustDets_2()
{
    cleanMemFull();
    clearCounters();
    clock_t begin=clock();
    //
    double_t lt1=1./(1+lm_pint*DTdet1);
    // Remc= lt2*lm_pint*40
    // DT[usec]=DT[BC]/40
    double_t lt2=l0_pint/(1+lm_pint*l0_pint*DTdet2);
    //
    int margin=std::max(DTdet1,DTdet2);

    //
    std::cout << "Two clusters. Rate[MHz]:" << lm_pint*40;
    std::cout << " independent rates lt1:"<< lt1<<" lt2:"<< lt2 << " lt1*lt2:"<< lt1*lt2<< std::endl;

    for(int i=0;i<SIZE-margin-1;i++)
    {
        //std::cout << i << std::endl;
        if(rnlx()<lm_pint)
        {
            lm_inputs[0][i]=1;
            lm_inter++;
            if(rnlx()<l0_pint)
            {
                l0_inter++;
                l0_inputs[0][i+LML0time]=1;
            }
        }
        int BUSYclst = det_busy[0][i]  || det_busy[1][i];
        int BUSYclst2 = det_busy[0][i];
        //int BUSYclst2 = det_busy[1][i];
        // LM level
        int state=0;
        if(lm_inputs[0][i])
        {
            if(!BUSYclst && !lmdeadtime[i])
            {
                // TRD EMC
                lm_triggers[0][i+LML0time]=1;
                state=1;

            }
            if(!BUSYclst2 && !lmdeadtime[i])
            {
                //TRD
                lm_triggers[1][i+LML0time]=1;
                state=1;
            }
            if(state)
            {
                //trig12++;
                // pretect TRD
                int BUSY = i+LMDT;
                for(int j=i+1;j<BUSY;j++)lmdeadtime[j]=1;
                // forbid triggers for emc
                //BUSY=i+LML0time;
                //for(int j=i+1;j<BUSY;j++)lml0busy[j]=1;
            }
        }
        // L0level
        //int cor=0;
        if(l0_inputs[0][i] && lm_triggers[0][i])
        {
            //if(!BUSYclst && !lml0busy[i])
            {
                //TRD EMC
                //cor=1;
                trig12++;
                l0_triggers[0][i]=1;
                //l0_triggers[1][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)det_busy[1][j]=1;
                BUSY=i+DTdet1;
                for(int j=i+1;j<BUSY;j++)det_busy[0][j]=1;
            }
        }
        if(lm_triggers[1][i])
        {
            //if(!BUSYclst2 && !lml0busy[i])
            {
            //lm_trig1+=1-cor;
            lm_trig++;
            l0_triggers[1][i]=1;
            int BUSY=i+DTdet1;
            for(int j=i+1;j<BUSY;j++)det_busy[0][j]=1;
            }
        }
        //std::cout << std::setw(5) << i <<std::dec<< " INPS:"<< lm_inputs[0][i]<<l0_inputs[0][i];
        //std::cout <<" Trigs:"<< lm_triggers[0][i]<<l0_triggers[0][i];
        //std::cout <<" busy:"<<!BUSYclst<<(!lml0busy[i])<< std::endl;

    }
    clock_t end=clock();
    double_t elapsed_secs=double_t(end-begin)/CLOCKS_PER_SEC;
    std::cout << "Time[secs]:"<< elapsed_secs<<std::endl;
    std::cout << "% TRD of LM rate " << (double_t)lm_trig/lm_inter << " " << lm_trig<< std::endl;
    std::cout << "% EMC of LM rate " << (double_t)l0_trig/lm_inter << " " << l0_trig <<std::endl;
    std::cout << "% TRD&EMCofLM rate " << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
    //std::cout << "% TRD of LM rate formula " << rtrd << std::endl;
    //std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;
    return 0;
}

int AllSim::clustDets_2a()
{
    cleanMemFull();
    clearCounters();
    clock_t  begin=clock();
    //
    double_t lt1=1./(1+lm_pint*DTdet1);
    // Remc= lt2*lm_pint*40
    // DT[usec]=DT[BC]/40
    double_t lt2=l0_pint/(1+lm_pint*l0_pint*DTdet2);
    //
    double_t f=1./(1.+lm_pint*LMDT);
    double_t d12=DTdet1*lm_pint*f;
    d12=0;
    double_t d21=DTdet2*lm_pint*l0_pint*f;
    double_t d11=1+d21;
    double_t d22=1+d12;
    double_t DET=d11*d22-d12*d21;
    lt1=d11*lm_pint*f-f*lm_pint*l0_pint*d21;
    lt2=f*lm_pint*l0_pint*d22-f*lm_pint*d12;
    lt1=lt1/DET/lm_pint;
    lt2=f*l0_pint/(1+f*l0_pint*lm_pint*DTdet2+f*lm_pint*DTdet1);
    lt1=f*(1-DTdet2*l0_pint*lt2*lm_pint)/(1+f*lm_pint*DTdet1);
    //int margin=std::max(DTdet1,DTdet2);

    //
    std::cout << "Two clusters. Rate[MHz]:" << lm_pint*40;
    std::cout << " Rates lt1:"<< lt1<<" lt2:"<< lt2 << std::endl;
    std::cout << "NLOOP:"<< Nloop << " "<< SIZE<< std::endl;
    int i=0;
    for(int i2=0;i2<Nloop;i2++)
    for(int i1=0;i1<SIZE;i1++)
    {
        //std::cout << i << std::endl;
        // clean memory in front ?
        //
        i=(i+1)%SIZE;
        if(rnlx()<lm_pint)
        {
            lm_inputs[0][i]=1;
            lm_inter++;
            if(rnlx()<l0_pint)
            {
                l0_inter++;
                l0_inputs[0][(i+LML0time)%SIZE]=1;
            }
            else
                l0_inputs[0][(i+LML0time)%SIZE]=0;
        }
        int BUSYclst = det_busy[0][i]  || det_busy[1][i];
        int BUSYclst2 = det_busy[0][i];
        //int BUSYclst2 = det_busy[1][i];
        // LM level
        int state=0;
        if(lm_inputs[0][i])
        {
            if(!BUSYclst && !lmdeadtime[i])
            {
                // TRD EMC
                lm_triggers[0][(i+LML0time)%SIZE]=1;
                state=1;

            }
            else
                lm_triggers[0][(i+LML0time)%SIZE]=0;

            if(!BUSYclst2 && !lmdeadtime[i])
            {
                //TRD
                lm_triggers[1][(i+LML0time)%SIZE]=1;
                state=1;
            }
            else
                lm_triggers[1][(i+LML0time)%SIZE]=0;
            if(state)
            {
                //trig12++;
                // pretect TRD
                int BUSY = i+LMDT;
                for(int j=i+1;j<BUSY;j++)lmdeadtime[j%SIZE]=1;
                // forbid triggers for emc
                BUSY=i+LML0time;
                for(int j=i+1;j<BUSY;j++)lml0busy[j%SIZE]=1;
            }
        }
        // L0level
        int not_rejected=0;
        if(l0_inputs[0][i] && lm_triggers[0][i])
        {
            if(!BUSYclst && !lml0busy[i])
            {
                not_rejected=1;
                //TRD EMC
                //cor=1;
                trig12++;
                l0_triggers[0][i]=1;
                //l0_triggers[1][i]=1;
                int BUSY=i+DTdet2;
                for(int j=i+1;j<BUSY;j++)det_busy[1][j%SIZE]=1;
                BUSY=i+DTdet1;
                for(int j=i+1;j<BUSY;j++)det_busy[0][j%SIZE]=1;
            }
        }
        if(lm_triggers[1][i])
        {
            if(!BUSYclst2 && !lml0busy[i])
            {
            not_rejected=1;
            //lm_trig1+=1-cor;
            lm_trig++;
            l0_triggers[1][i]=1;
            int BUSY=i+DTdet1;
            for(int j=i+1;j<BUSY;j++)det_busy[0][j%SIZE]=1;
            }
        }
        if((lm_triggers[0][i] || lm_triggers[1][i]))
        {
            lm_after++;
            if(not_rejected==0)lm_rejected++;
        }
        cleanMem(i-1);
        //std::cout << std::setw(5) << i <<std::dec<< " INPS:"<< lm_inputs[0][i%SIZE]<<l0_inputs[0][i%SIZE];
        //std::cout <<" Trigs:"<< lm_triggers[0][i%SIZE]<<l0_triggers[0][i%SIZE];
        //std::cout <<" busy:"<<!BUSYclst<<(!lml0busy[i%SIZE])<< std::endl;

    }
    clock_t end=clock();
    double_t elapsed_secs=double_t(end-begin)/CLOCKS_PER_SEC;
    std::cout << "Time[secs]:"<< elapsed_secs<<std::endl;
    std::cout << "% TRD of LM rate " << (double_t)lm_trig/lm_inter << " " << lm_trig<< std::endl;
    std::cout << "% EMC of LM rate " << (double_t)l0_trig/lm_inter << " " << l0_trig <<std::endl;
    std::cout << "% TRD&EMCofLM rate " << (double_t)trig12/lm_inter << " " << trig12<< std::endl;
    std::cout << "LM rejected/ LM after:" << (double_t)lm_rejected/lm_after << " lm_rejected:"<< lm_rejected << " lm_after:"<<lm_after <<std::endl;
    //std::cout << "% TRD of LM rate formula " << rtrd << std::endl;
    //std::cout << "LT 12c:" << (double_t)trig12check/lm_inter << " " << trig12check<< std::endl;
    return 0;
}

void AllSim::PlorFormulas()
{
    std::cout << "LMDT effect:" << std::endl;
    int Ndiv1=100;
    int Ndiv2=100;
    double_t f;
    double_t lt3;
    double_t lt4;
    l0_pint=0.;
    double_t del=1./Ndiv1;
    lm_pint=0;
    std::cout << "DTLM:"<< LMDT << " LML0time:"<< LML0time<<std::endl;
    //double_t maxav=0;
    double_t maxdiff=0;
    for(int j=0;j<Ndiv1;j++)
    {
        l0_pint=0;
        for(int i=0;i<Ndiv2;i++)
        {
            f=1./(1+lm_pint*LMDT);
            lt3=f*l0_pint/(1+f*lm_pint*l0_pint*(DTdet2));
            lt4=l0_pint/(1+l0_pint*lm_pint*(DTdet2+LML0time)+(1-l0_pint)*lm_pint*LMDT);
            //std::cout<< "lm_pint:"<< lm_pint<<" l0_pint:"<< l0_pint << " f:"<<f<<" lt3:"<<lt3<<" lt4:"<<lt4<<std::endl;
            //printf("lm_pint: %5.3f l0_pint:%5.3f f:%8.6f lt3:%10.8f lt4:%10.8f diff:%10.8f\n",lm_pint,l0_pint,f,lt3,lt4,(lt3-lt4)/lt3);
            //lm_pint+=del;
            double_t diff=std::abs(lt3-lt4);
            //double_t av=diff/lt4;
            //if(av>0.0075)printf("lm_pint: %5.3f l0_pint:%5.3f f:%8.6f lt3:%10.8f lt4:%10.8f diff:%10.8f\n",lm_pint,l0_pint,f,lt3,lt4,(lt3-lt4)/lt3);
            l0_pint+=del;
            if(maxdiff<diff)
            {
                maxdiff=diff;
                printf("lm_pint: %5.3f l0_pint:%5.3f f:%8.6f lt3:%10.8f lt4:%10.8f diff:%10.8f\n",lm_pint,l0_pint,f,lt3,lt4,(lt3-lt4)/lt3);
            }
        }
        lm_pint+=del;
    }
}

double AllSim::rnlx(){return ranlux(gen);}
