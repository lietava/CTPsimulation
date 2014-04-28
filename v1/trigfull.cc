#include "trig.h"

Trig::Trig()
{
}
//----------------------------------------------------------
void Trig::Setup()
{
 //pr << "Trig constructor started." << endl;
 stateDAQfull=0;
 stateRARE=0;
 numofCLU=3;
 numofTC=3;
 numofDET=4;
 numofTRINP=3;;
 trigpat = new int[numofTRINP];
 TrigClass = new int[numofTC];
 detBusy =new int[numofDET];
 for (int i=0;i<numofDET;i++) detBusy[i]=0;
 for (int i=0;i<numofTRINP;i++) trigpat[i]=0;
 for (int i=0;i<numofTC;i++) TrigClass[i]=0;
 stateBUSY=0;
 PFque.Setup(delayL2+delayL2+2);
 afterL0classes.Setup(100);  
 afterL1classes.Setup(10);

// //pr << "1 Length=" << PF1.Length() << endl;
//
//   Association of detectors to Clusters
//
 TrigCluster = new pint[numofCLU];
 for (int i=0;i<numofCLU;i++) TrigCluster[i]=new int[numofDET];
 for(int i=0; i< numofCLU; i++)
 for(int j=0; j< numofDET; j++)TrigCluster[i][j]=0;
 TrigCluster[0][0]=1;TrigCluster[0][3]=1;                     //Cluste MB
 TrigCluster[1][0]=1;TrigCluster[1][1]=1;                     //   dmuon
 TrigCluster[2][0]=1;TrigCluster[2][2]=1;TrigCluster[2][3]=1; //   electron
//
// Assignment of Detector clusters to trigger classes
//
 TCla2DClu = new int[numofTC];
 TCla2DClu[0]=0;
 TCla2DClu[1]=1;
 TCla2DClu[2]=2;
 //pr << "Trig constructor finished." <<endl;
// Assigned PF to cluster
 PF2DClu = new int[numofCLU];
 PF2DClu[0]=2;
 PF2DClu[1]=0;
 PF2DClu[2]=2;
 PF=new int[3];
 PF[0]=(int) delayL2;
 PF[1]=400;
 PF[2]=(int) delayL2;
// cout << "PF= " << PF[0] << " " <<PF[1]<< " " <<PF[2] <<endl;
// Definition of trigger classes
 TCla2Tinp=new pint[numofTC];
 for(int i=0; i<numofTC; i++){
    TCla2Tinp[i]=new int[numofTRINP];
    for(int j=0; j< numofTRINP; j++) TCla2Tinp[i][j]=0;  
 }
 TCla2Tinp[0][0]=1;
 TCla2Tinp[1][0]=1;TCla2Tinp[1][1]=1;
 TCla2Tinp[2][0]=1;TCla2Tinp[2][2]=1;      
}
//------------------------------------------
Trig::~Trig()
{
 delete [] trigpat;
 delete [] TrigClass;
 delete [] detBusy;
 delete [] TrigCluster;
}
//-------------------------------------------
bool Trig::getTrigClassesL0()
{
 // Toto treba urobit elegantnejsie

 bool inter=0;
 for(int i=0; i<numofTC; i++)TrigClass[i]=0;


 if( trigpat[0]){ 
   TrigClass[0]=1;
   TrigClass[1]=0;
   TrigClass[2]=0;
   if(trigpat[1]) TrigClass[1]=1;
   else TrigClass[2]=1;   
 }
 else inter = 0;

 if(stateRARE) TrigClass[0]=0;

 for(int i=0; i<numofTC; i++)if(TrigClass[i]) inter=1 ;  

 //pr << "Trig::getTrigClassesL0: " << TrigClass[0] <<TrigClass[1] <<TrigClass[2] << endl;

 return inter; 
}
//-------------------------------------------
bool Trig::getTrigClassesL1()
{
 // Toto treba urobit elegantnejsie
 bool inter=1;
 pint tc;

 //pr << "Trig::getTrigClassesL1 L2 =" << trigpat[2] << endl;

 afterL0classes.Remove(tc);
 //pr << "Trig::getTrigClassesL1: " << tc[0]<< tc[1]<< tc[2] << endl;

 for(int i=0;i<numofTC;i++){
    if(tc[i]){
      if(TCla2Tinp[i][2] ){
        if(trigpat[2])tc[i]=1;
        else tc[i]=0;
      }
    }
  TrigClass[i]=tc[i];
 }

 //pr << "Trig::getTrigClassesL1: " << TrigClass[0]<< TrigClass[1]<<TrigClass[2] << endl;

 delete [] tc; 

 return inter; 
}

//-----------------------------------------
bool Trig::removeBusyClasses()
{
 int L0=0;

 for (int i=0; i < numofDET; i++) detBusy[i]=detinfo.detActive[i];
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
         for(int j=0; j<numofDET; j++ )            
            if(TrigCluster[ Tclass ][j]){
              if(detBusy[j]) TrigClass[Tclass]=0;
            } 
      }
      L0=L0+TrigClass[Tclass];
     }

// maybe pf here
 
 return L0 > 0 ? 1 : 0;
}
//------------------------------------------------------------
bool Trig::removePFClasses(char level)
{
// Loop over L1 accepted trigger classes
// and check for P/F.
// No PF at the moment;

 int L1=0;
 int Dclust;
 int pf,l1,l2,pl;

// pf=CheckPF(l);
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
              Dclust = TCla2DClu[Tclass];

              pl=PF[Dclust];
//                   cout << "case=" << level << " pl=" << pl ;
//                   cout << " clust=" << Dclust << endl; 
               switch(level){
                      case '0' : l1=2*delayL2+2-pl;
                                 l2=2*delayL2+2;
                                 break;
                      case '1' : l1=2*delayL2+2-(delayL1-delayL0)-pl;
                                 l2=2*delayL2+2;
                                 break;
                      case '2' : l1=2*delayL2+2-(delayL2-delayL0)-pl;
                                 l2=2*delayL2+2-(delayL2-delayL0)+pl;
                                 break;
                      default  : cout << "Trig::removePFClasses: unknown trigger level" <<
		       endl;exit(1);
               }
              pf=CheckPF(l1,l2);
//              cout << "Trig::removePFClasses pf= " << pf << endl;
              if(pf > PF2DClu[Dclust] ) TrigClass[Tclass]=0;             
      }
      L1=L1+TrigClass[Tclass];
     }
 return L1 > 0 ? 1 : 0;
}
//------------------------------------------------------------
bool Trig::SendL1(INT i,char message)
{
 // Tu je viacnasobne posielanie 
 // ale okrem efektivity to nevadi 
 // lebo sendl1 len prepisuje detL01 received
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
         for(int j=0; j<numofDET; j++ )            
            if(TrigCluster[ Tclass ][j])
               switch(j){
                case 0: CalQueue::PutEntry(i,'l','1');break;
                case 1: CalQueue::PutEntry(i,'l','2');break;
                case 2: CalQueue::PutEntry(i,'l','3');break; 
                case 3: CalQueue::PutEntry(i,'l','4');break;
                default: cout << "Trig::SendL1:unknown detector" << endl;
                exit(1);   
              }   
      }
   //   TrigClass[Tclass];
     }
  
 //pr << "Trig::SendL1 Tclass:" << TrigClass[0] <<TrigClass[1] <<TrigClass[2] << endl;
 return 1;
}

//--------------------------------------------
void Trig::setDetBusy(INT i, char message)
{
int Tclass,Tclust;; 
bool SendDet[numofDET];

 for(int j=0; j<numofDET; j++) SendDet[j]=0;

 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
         Tclust=TCla2DClu[Tclass];
         for(int j=0; j<numofDET; j++ )
            if(TrigCluster[Tclust][j]) SendDet[j]=SendDet[j] || 1;             
      }
     }

// toto mozem prerobit lepsie, prinejmensom ako ostatni

 for(int j=0; j<numofDET; j++)
               switch (j){
                case 0:  if (SendDet[j])CalQueue::PutEntry(i+1,'x',message);break;
                case 1:  if (SendDet[j])CalQueue::PutEntry(i+1,'y',message);break;
                case 2:  if (SendDet[j])CalQueue::PutEntry(i+1,'z',message);break;
                case 3:  if (SendDet[j])CalQueue::PutEntry(i+1,'v',message);break;
                default: cout << "Trig::setDetBusy: unknown detector "
                              << j << endl; exit(1);
               }


}
//----------------------------------------
void Trig::DecideL0(INT i)
{
 char message;
 bool L0trig=0,inter=0,pf=0;
////pr << "From Trig::ReceiveL0 m=" << message << 
//" busy=" << stateBUSY << " " << i << endl;
// Tu by mohla ist i P/F ale zatial nie

//P/F
// Pre kazdy class zvlast 
// este sem treba dat triger busy aP/F
// Moze byt az pocet clusterov trigrov
// Musim si zapisat kazdy zvlast
// potom treba prodat 2 ms ces setbusy

 if(stateDAQfull) return;

 if( stateBUSY == 0 ){
 inter=getTrigClassesL0();
 if( inter ){ 

// loop over classes and check if they are not busy, if busy
// remove from list;
// you are left with free classes

 L0trig=removeBusyClasses();

// add busy to new detectors and maybe remember busy dets for given event

 //pr << "Trig::DecideL0 inter,L0trig "<< inter<<L0trig<<endl;

 if ( L0trig ){

     pf = removePFClasses('0'); 
     if(!pf) return;
     pint pp;
     pp=new int[numofTC];
     for (int j=0; j<numofTC; j++)pp[j]=TrigClass[j];
     //pr << "pp= "<< pp[0] << pp[1] << pp[2] << endl;
     afterL0classes.Add(pp); 

// Senddata L0
     setDetBusy(i,message);
// Decide L1 later
     CalQueue::PutEntry(i+(delayL1-delayL0),'m',message);
// Set trigger to busy and reset after 2 ms.
     stateBUSY=1;
     CalQueue::PutEntry(i+trigDT,'o',message);
 }
 }
 
 for (int j=0; j<numofDET; j++) detBusy[j]=detinfo.detActive[j];
 //pr << "Trig::DecideL0:trigpat=" << trigpat[0] << trigpat[1] 
 //     << " trigClass: " << TrigClass[0] <<TrigClass[1] <<TrigClass[2]
 //     << " detBusy:"<< detBusy[0]<< detBusy[1]<< detBusy[2] << detBusy[3] <<endl;   
 }
 trigpat[0]=0; trigpat[1]=0;
}
//------------------------------------------------
void Trig::DecideL1(INT i, char message)
{
 // check p/f inak je to o hovne
 // ak je ok, posli decideL2 triger on si pozre patern
 // ak zamietnuty reset detecors 

 bool inter;

 getTrigClassesL1();
 inter=removePFClasses('1');

 //pr << "Trig::DecideL1: inter= " << inter << endl;

 if(inter){

   pint tc=new int[numofTC];
   for(int j=0;j<numofTC;j++)tc[j]=TrigClass[j];  
 
   afterL1classes.Add(tc);
   SendL1(i,message);

   CalQueue::PutEntry(i+(delayL2-delayL1) ,'n',message);  

   //pr << "Trig::DecideL1: trigpatL1 " << trigpat[2] << endl
   //     << "TrigClass:" << TrigClass[0] << TrigClass[1] <<TrigClass[2] <<
   //        endl; 

 }
// Clear L1 inputs
 trigpat[2]=0;
}
//------------------------------------------
void Trig::DecideL2(INT i, char message)
{
 // only PF
 bool pf;
 pint p;
 
 afterL1classes.Remove(p);
 for (int j=0; j<numofTC; j++)TrigClass[j]=p[j];
 
 pf=removePFClasses('2');
 if(!pf) return;
 
 message='b';
 if(p[1] == 1) message = 'm';
 if(p[2] == 1) message = 'e'; 

// cout << "Trig::DecideL2 " << i <<' ' << p[0] << p[1] << p[2] << message << endl;

 delete [] p;
 CalQueue::PutEntry(i+1,'g',message); 
}
//----------------------------------------
void Trig::ReceiveL0(INT i, char message)
{
 switch (message){
 case '1': trigpat[0]=1;break;
 case '2': trigpat[1]=1;break;
 default :  cout << "Trig::ReceiveL0: Unknown detector" << endl;exit(1);
 }
 //pr << "Trig::ReceiveL0 trigpat=" << trigpat[0] << trigpat[1] << endl;   
}
//----------------------------------------
void Trig::ReceiveL1(INT i, char message)
{
 // Toto musi byt pred DecideL1
 switch (message){
 case '3': trigpat[2]=1;break;
 default :  cout << "Trig::ReceiveL0: Unknown detector" << endl;exit(1);
 }
 //pr << "L1 trigpat=" << trigpat[2] << endl;   
}
//----------------------------------------
void Trig::PastFuture(char message)
{
 int inter;
 switch (message){
 case 'y': 
           PFque.Remove(inter);PFque.Add(1);
           break;
 case 'n': 
           PFque.Remove(inter);PFque.Add(0);
           break;
 default : cout << "Trig::PastFuture : message not in list." << endl;
           exit(2);
 }
//           //pr << message << endl;
}
//------------------------------------------
int Trig::CheckPF(int l1,int l2)
{
  int pf;
 pf=PFque.CountQue(l1,l2);
// //pr << "Trig::CheckPF pf=" << pf << endl;
// pf=0;
 return pf;      
}
//-----------------------------------------
void Trig::SetBusy()
{
 stateBUSY=1;
} 
//-----------------------------------------
void Trig::ResetBusy()
{
 //pr << "Trig::ResetBusy old state= " << stateBUSY << endl;
 stateBUSY=0;
}
//-------------------------------------------
void Trig::setDelayL0(INT t)
{
 delayL0=t;
} 
//-------------------------------------------
void Trig::setDelayL1(INT t)
{
 delayL1=t;
}
//-------------------------------------------
void Trig::setDelayL2(INT t)
{
 delayL2=t;
} 
//-------------------------------------------
void Trig::setTrigDT(INT t)
{
 trigDT=t;
} 
