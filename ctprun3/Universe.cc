#include "Universe.h"
Universe::Universe()
{
 dets = new Det[3];
 dets[0].name="TRD";
 dets[0].SetID(0);
 dets[1].name="TPC";
 dets[1].SetID(1);
 dets[2].name="SPD";
 dets[2].SetID(2);
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
 case 100: ctp.CheckLM(i); break; 
 //case 101: ctp.ReceiveL0Inps(i); break;
 //case 102: ctp.ReceiveL1Inps(i);  break;
 case 150: ctp.CheckL0(i); break;
 case 151: ctp.CheckL1(i); break;
 //
 case 200: dets[0].SetBusy(i); break;
 case 201: dets[1].SetBusy(i); break;
 case 202: dets[2].SetBusy(i); break;
 // ctp received busy from det0 ~ clust1 
 case 300: ctp.SetDetBusy(i,0); break;
 case 301: ctp.SetDetBusy(i,1); break;
 case 302: ctp.SetDetBusy(i,2); break;
 // L1 triggers
 // L1 trigger timeouts
 case 500: dets[0].ResetBusy(i);break;
 case 501: dets[1].ResetBusy(i);break;
 case 502: dets[2].ResetBusy(i);break;
 // CTP receives busy
 case 600: ctp.ResetDetBusy(i,0); break;
 case 601: ctp.ResetDetBusy(i,1); break;
 case 602: ctp.ResetDetBusy(i,2); break;

 default: {cout << "exiting from doit " << eventt << endl; 
           exit(1);
          }
 }            
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
 int NCYC=2;
 int NBASE=10000;
 for (int cycle=0; cycle<NCYC; cycle++){
 for (time=0; time < NBASE ; time++){  //12s=500000000
//     cout << "------------------------------------------------------" << endl;
     if ( (time/delta)*delta == time) Monitor(time,delta);

     if(cycle!=(NCYC-1))Collide(Time);
     //cout << " Time=" << Time << endl;
     while (cq.DoEntry(Time,&eventt)){
           doit(Time,eventt);
     }
 //ctp.DecideL0(Time);
 //Time++;
 Time=(Time+1) % CalQueue::SizeofQue;
 cq.GlobTime++;
 }
 }
 ctp.printCounts();
 clock_t stop=clock();
 cout << "CPU time= " << (stop-start)/CLOCKS_PER_SEC <<" sec"<< endl;
} 
