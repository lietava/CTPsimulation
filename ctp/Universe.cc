#include "Universe.h"
Universe::Universe()
{
 dets = new Det[2];
 dets[0].name="TRD";
 dets[0].SetID(0);
 dets[1].name="TPC";
 dets[1].SetID(1);
}
//-----------------------------------------------
void Universe::Setup()
{
}
//------------------------------------------
Universe::~Universe()
{
}
//---------------------------------------------
void Universe::doit(INT i,INT eventt)
{
 switch (eventt){
 //  Que Event generation
 case   1: event.ProduceTriggers(i);  break;
 case 100: ctp.ReceiveLMInps(i); break; 
 //case 101: ctp.ReceiveL0Inps(i); break;
 //case 102: ctp.ReceiveL1Inps(i);  break;
 case 150: ctp.CheckL0(i); break;
 case 151: ctp.CheckL1(i); break;
 //
 case 200: DetsGotLMTrigger(i); break;
 case 201: DetsGotL0Trigger(i); break;
 // ctp received busy from det0 ~ clust1 
 case 300: ctp.SetClstBusy(i,0); break;
 case 301: ctp.SetClstBusy(i,1); break;

 default: {cout << "exiting from doit " << eventt << endl; 
           exit(1);
          }
 }            
}
//-----------------------------------------------------
void Universe::DetsGotLMTrigger(INT i)
{
 // only trd
 dets[0].SetBusy(i);
}
//-----------------------------------------------------
void Universe::DetsGotL0Trigger(INT i)
{
 // non trd
 dets[1].SetBusy(i);
}
//----------------------------------------------------
void Universe::Collide(INT Time)
{
 //float x=(float)rand()/RAND_MAX;
 //float x=rnlx();
 //if(x < 1.1) cq.PutEntry(Time,1); 
 cq.PutEntry(Time,1); 
}
//----------------------------------------------------
void Universe::Monitor(long int time,int delta)
{
}
//----------------------------------------------
void Universe::Exist()
{
 INT eventt;
 INT Time;
 long int time;
 int delta;
 delta=100000;
// Setting times
 Setup();
 //ctp.Setup();
//-------------------------------------------------
 Time=0;
 //gdc.SendData(0);
 clock_t start=clock();
 int NCYC=201;
 for (int cycle=0; cycle<NCYC; cycle++){
 for (time=0; time < 1000000 ; time++){  //12s=500000000
//     cout << "------------------------------------------------------" << endl;
     if ( (time/delta)*delta == time) Monitor(time,delta);

     if(cycle!=(NCYC-1))Collide(Time);
     //cout << " Time=" << Time << endl;
     while (cq.DoEntry(Time,&eventt)){
           doit(Time,eventt);
     }
 //ctp.DecideL0(Time);
 Time++;
 cq.GlobTime++;
 }
 }
 ctp.printCounts();
 clock_t stop=clock();
 cout << "CPU time= " << (stop-start)/CLOCKS_PER_SEC <<" sec"<< endl;
} 
