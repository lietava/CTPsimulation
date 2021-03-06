#include "counters.h"   

Counters::Counters()
{
}
//----------------------------------------
Counters::~Counters()
{
 delete [] L0;
 delete [] L0DT;
 delete [] L0DTPF;
 delete [] L1;
 delete [] L1PF;
 delete [] L2;
 delete [] L2PF;
}
//-------------------------------------------
void Counters::Setup(int numoftc)
{
 numofTC=numoftc;
 L0 = new int[numofTC];
 L0DT = new int[numofTC];
 L0DTPF = new int[numofTC]; 

 L1 = new int[numofTC];
 L1PF = new int[numofTC]; 

 L2 = new int[numofTC];
 L2PF = new int[numofTC];

 for (int i=0; i<numofTC; i++){
     L0[i]=0;L0DT[i]=0;L0DTPF[i]=0;
     L1[i]=0;L1PF[i]=0;
     L2[i]=0;L2PF[i]=0;
 }

}
//---------------------------------------------
void Counters::CountL0(int *TC)
{
 
// int *tc = new int[numofTC];
//      tc=TC;
  for (int i=0; i<numofTC; i++) if(TC[i]) L0[i]++;
//  for (int i=0; i<numofTC; i++) cout << TC[i] << " " << L0[i] << "  ";  cout << endl; 
}
//---------------------------------------------
void Counters::CountL0DT(int *TC)
{
   for (int i=0; i<numofTC; i++) if(TC[i]) L0DT[i]++;
//  for (int i=0; i<numofTC; i++) cout << TC[i] << " " << L0[i] << "  ";  cout << endl; 
}
//---------------------------------------------
void Counters::CountL0DTPF(int *TC)
{
  for (int i=0; i<numofTC; i++) if(TC[i]) L0DTPF[i]++;
//  for (int i=0; i<numofTC; i++) cout << TC[i] << " " << L0[i] << "  ";  cout << endl; 
}
//---------------------------------------------
void Counters::CountL1(int *TC)
{
  for (int i=0; i<numofTC; i++) if(TC[i]) L1[i]++;
}
//------------------------------------------------------------------------------
void Counters::CountL1PF(int *TC)
{
  for (int i=0; i<numofTC; i++) if(TC[i]) L1PF[i]++;
}
//---------------------------------------------
void Counters::CountL2(int *TC)
{
  for (int i=0; i<numofTC; i++) if(TC[i]) L2[i]++;
}
//------------------------------------------------------------------------------
void Counters::CountL2PF(int *TC)
{
  for (int i=0; i<numofTC; i++) if(TC[i]) L2PF[i]++;
}


//--------------------------------------------------
void Counters::getL0(int **l0, int **l0dt, int **l0dtpf)
{
 *l0=L0; *l0dt=L0DT; *l0dtpf=L0DTPF;
}
//--------------------------------------------------
void Counters::getL1(int **l1, int **l1pf)
{
 *l1=L1; *l1pf=L1PF;
}
//--------------------------------------------------
void Counters::getL2(int **l2, int **l2pf)
{
 *l2=L2; *l2pf=L2PF;
}
