#include "trig.h"
Trig::Trig()
{
}
//----------------------------------------------------------
void Trig::Setup()
{
// Output for monitor
   trigmon.open("trig.dat", std::ios::out);     
// Numbers of parameters
 stateDAQfull=0;
 stateRARE=0;
 numofCLU=3;
 numofTC=3;
 numofDET=4;
 numofTRINP=3;
 numofTRINPL0=2;
 trigpat = new int[numofTRINP];
 TrigClass = new int[numofTC];
//
//  Counters for x section
//
 counters.Setup(numofTC);

//
// Definition of Trigger classes vrt Trigger Inputs
//

 TrigClassDef = new (int *) [numofTC];
 for (int i=0;i<numofTC;i++)TrigClassDef[i]= new int[numofTRINP];
 TrigClassDef[0][0]=1;TrigClassDef[0][1]=0;TrigClassDef[0][2]=0; 
 TrigClassDef[1][0]=1;TrigClassDef[1][1]=1;TrigClassDef[1][2]=0; 
 TrigClassDef[2][0]=1;TrigClassDef[2][1]=0;TrigClassDef[2][2]=1; 

//
 detBusy =new int[numofDET];
 for (int i=0;i<numofDET;i++) detBusy[i]=0;
 for (int i=0;i<numofTRINP;i++) trigpat[i]=0;
 for (int i=0;i<numofTC;i++) TrigClass[i]=0;
 stateBUSY=0;
 afterL0classes.Setup(100);  
 afterL1classes.Setup(10);
 trigpatatL0.Setup(200);

//----------------------------------------------------------
//   Association of detectors to Clusters
//
 DetCluster = new pint[numofCLU];
 for (int i=0;i<numofCLU;i++) DetCluster[i]=new int[numofDET];
 for(int i=0; i< numofCLU; i++)
 for(int j=0; j< numofDET; j++)DetCluster[i][j]=0;
 DetCluster[0][0]=1;DetCluster[0][3]=1;                     //Cluste MB
 DetCluster[1][0]=1;DetCluster[1][1]=1;                     //   dmuon
 DetCluster[2][0]=1;DetCluster[2][2]=1;DetCluster[2][3]=1; //   electron
//
// Assignment of Detector clusters to trigger classes
//
 TCla2DClu = new int[numofTC];
 TCla2DClu[0]=0;
 TCla2DClu[1]=1;
 TCla2DClu[2]=2;

// Assigned PF to cluster - number of interactions
 PF2DClu = new int[numofCLU];
 PF2DClu[0]=3;
 PF2DClu[1]=0;
 PF2DClu[2]=3;

// Duration of PF for every cluster
 PF=new int[3];
 PF[0]=(int) delayL2;
 PF[1]=400;
 PF[2]=(int) delayL2;  

// PF counters (tima interval > 2*delayL2 change it !!!)
 PFque.Setup(delayL2+delayL2+2);

}
//------------------------------------------
Trig::~Trig()
{
 delete [] trigpat;
 delete [] TrigClass;
 delete [] detBusy;
 delete [] DetCluster;
 delete [] TrigClassDef;
 trigmon.close();
}
//-------------------------------------------
bool Trig::getTrigClassesL0()
{
 bool inter=0;

 for(int i=0; i<numofTC; i++)TrigClass[i]=0;
 for (int i=0; i< numofTC;i++){
     bool sum=1,pom;
     for (int j=0; j< numofTRINPL0; j++){
         pom = (TrigClassDef[i][j] && trigpat[j]);
         pom = (pom && TrigClassDef[i][j]) || (!pom && !TrigClassDef[i][j]);
         sum= sum && pom;
     }
  if (sum) TrigClass[i] = 1;
 }



/*  Downscaling on/off
 if(TrigClass[2]){
 double downscale=drand48();
 if(downscale>0.1) TrigClass[2]=0;
 }
*/
 for(int i=0; i<numofTC; i++)if(TrigClass[i]) inter=1 ;  
//cout << "Trig::getTrigClassesL0: " << TrigClass[0] <<TrigClass[1] 
// <<TrigClass[2] << " pat " << trigpat[0] << trigpat[1] << trigpat[2]<<endl;


 return inter; 
}
//-------------------------------------------
bool Trig::getTrigClassesL1()
{
 // Tu je ojebany timeing prichodu l2 do trigere
 // berem ho rovno z l0 cez zapametanie si

 int inter=0;
 pint tc,tp;

 afterL0classes.Remove(tc);
 trigpatatL0.Remove(tp);
// cout << "Trig::getTrigClassesL1: pat " << tp[0]<< tp[1]<< tp[2] << endl;
// cout << "Trig::getTrigClassesL1: clasL0 " << tc[0]<< tc[1]<< tc[2] << endl;

 for(int i=0;i<numofTC;i++){
    if(tc[i]){
      bool sum=1,pom;
      for (int j=0; j< numofTRINP; j++){
          pom = (TrigClassDef[i][j] && tp[j]);
          pom = (pom && TrigClassDef[i][j]) || (!pom && !TrigClassDef[i][j]);
          sum= sum && pom;
      }
      if (sum) TrigClass[i] = 1; else TrigClass[i]=0;
    }
    else TrigClass[i]=0;
    inter=inter+TrigClass[i];
 }

//  cout << "Trig::getTrigClassesL1: class L1 " << TrigClass[0]<< TrigClass[1]<<TrigClass[2] << endl;

 delete [] tc; 
 delete [] tp;

 return inter>0 ? 1 : 0; 
}

//-----------------------------------------
bool Trig::removeBusyClasses()
{
 int L0=0;
 int Tclust;
 for (int i=0; i < numofDET; i++) detBusy[i]=detinfo.detActive[i];

//cout << "Trig::removeBusyClasses: " << detBusy[0]<<detBusy[1]<< detBusy[2]<<
//detBusy[3]<< " TrigClasses " << TrigClass[0] << TrigClass[1] << 
//TrigClass[2] << endl;
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
         Tclust=TCla2DClu[Tclass];
         for(int j=0; j<numofDET; j++ )            
            if(DetCluster[ Tclust ][j]){
              if(detBusy[j]) TrigClass[Tclass]=0;
            } 
      }
      L0=L0+TrigClass[Tclass];
     }

//cout << "after: " << TrigClass[0] << TrigClass[1] << TrigClass[2] << endl; 

 return L0 > 0 ? 1 : 0;
}
//------------------------------------------------------------
bool Trig::removePFClassesL0()
{
// Loop over L1 accepted trigger classes
// and check for P/F.
// No PF at the moment;

 int Lx=0,Dclust,pf,pfclust; 

// Switch on/off Past/Future
//  return 1;
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
              Dclust = TCla2DClu[Tclass];
              pfclust= PF[Dclust];
              pf=PFque.CountQue(0,pfclust);

// cout << CalQueue::GlobTime << " level L0" << " clust=" << Dclust << endl; 
// cout << "Trig::removePFClasses pf= " << pf << endl;

              if(pf > PF2DClu[Dclust] ) TrigClass[Tclass]=0;             
      }
      Lx=Lx+TrigClass[Tclass];
     }
 return Lx > 0 ? 1 : 0;
}
//------------------------------------------------------------
bool Trig::removePFClassesL1()
{
// Loop over L1 accepted trigger classes
// and check for P/F.
// No PF at the moment;

 int Lx=0,Dclust,pf,pfclust;

// Switch on/off Past/Future
//  return 1;
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
              Dclust = TCla2DClu[Tclass];
              pfclust= PF[Dclust];
              pf=PFque.CountQue(0,pfclust+delayL1-delayL0);
//                   cout << "level L1" << " clust=" << Dclust << endl; 
//            cout << "Trig::removePFClasses pf= " << pf << endl;
              if(pf > PF2DClu[Dclust] ) TrigClass[Tclass]=0;             
      }
      Lx=Lx+TrigClass[Tclass];
     }
 return Lx > 0 ? 1 : 0;
}
//------------------------------------------------------------
bool Trig::removePFClassesL2()
{
// Loop over L1 accepted trigger classes
// and check for P/F.
// No PF at the moment;

 int Lx=0,Dclust,pf,pfclust;

// Switch on/off Past/Future
//  return 1;
 
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
              Dclust = TCla2DClu[Tclass];
              pfclust= PF[Dclust];
              pf=PFque.CountQue(delayL2-pfclust,2*pfclust);
//                   cout << "level L2" << " clust=" << Dclust << endl; 
//            cout << "Trig::removePFClasses pf= " << pf << endl;
              if(pf > PF2DClu[Dclust] ) TrigClass[Tclass]=0;             
      }
      Lx=Lx+TrigClass[Tclass];
     }
 return Lx > 0 ? 1 : 0;
}

//------------------------------------------------------------
bool Trig::SendL1(INT i,char message)
{
 // Tu je viacnasobne posielanie 
 // ale okrem efektivity to nevadi 
 // lebo sendl1 len prepisuje detL01 received
 
 int Tclust;
 for (int Tclass =0; Tclass <numofTC ; Tclass++ ){
      if (TrigClass[Tclass]){
         Tclust=TCla2DClu[Tclass];
         for(int j=0; j<numofDET; j++ )            
            if(DetCluster[ Tclust ][j])
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
            if(DetCluster[Tclust][j]) SendDet[j]=SendDet[j] || 1;             
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

 for (int j=0; j < numofDET; j++) detBusy[j]=detinfo.detActive[j];

//   cout << "From Trig::DecideL0 " << i << " " << "pat "<< 
//   trigpat[0] << trigpat[1] << trigpat[2] <<
//   " DetBusy " << detBusy[0] << detBusy[1] << detBusy[2] << detBusy[3]<< 
//     " DAQ TBUSY =" << stateDAQfull << stateBUSY << endl;

//P/F
// Pre kazdy class zvlast 
// este sem treba dat triger busy aP/F
// Moze byt az pocet clusterov trigrov
// Musim si zapisat kazdy zvlast
// potom treba prodat 2 ms ces setbusy

 inter=getTrigClassesL0();
 counters.CountL0(TrigClass);

 if(!stateDAQfull){
 if( !stateBUSY ){
 if(stateRARE) TrigClass[0]=0;
 
 if( inter ){ 

// loop over classes and check if they are not busy, if busy
// remove from list;
// you are left with free classes

 L0trig=removeBusyClasses();
 counters.CountL0DT(TrigClass); 

// add busy to new detectors and maybe remember busy dets for given event

// cout << "Trig::DecideL0 inter,L0trig "<< inter<<L0trig<<endl;

 if ( L0trig ){

// PF protection
     	pf = removePFClassesL0(); 
// cout << "L0 pf= " << pf << endl;
        counters.CountL0DTPF(TrigClass);
     if(pf){
     	pint pp,tp;
     	pp=new int[numofTC];
        tp=new int[numofTRINP];
     	for (int j=0; j<numofTC; j++)pp[j]=TrigClass[j];
     	for (int j=0; j<numofTRINP; j++)tp[j]=trigpat[j];
     //pr << "pp= "<< pp[0] << pp[1] << pp[2] << endl;
     	afterL0classes.Add(pp); 
        trigpatatL0.Add(tp);

// Senddata L0
//        cout << i << " trig::decidel0 setDetBusy done " << endl;
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
 }
 }
 trigpat[0]=0; trigpat[1]=0;trigpat[2]=0;
}
//------------------------------------------------
void Trig::DecideL1(INT i, char message)
{
 // check p/f inak je to o hovne
 // ak je ok, posli decideL2 triger on si pozre patern
 // ak zamietnuty reset detecors 

 bool inter;

 getTrigClassesL1();
 counters.CountL1(TrigClass);
 inter=removePFClassesL1();
 counters.CountL1PF(TrigClass);

// cout << "Trig::DecideL1: inter= " << inter << endl;

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
 counters.CountL2(TrigClass); 

 pf=removePFClassesL2();
 counters.CountL2PF(TrigClass);
 if(pf){
     message='b';
     if(p[1] == 1) message = 'm';
     if(p[2] == 1) message = 'e'; 
     CalQueue::PutEntry(i+1,'g',message);
long int jj=CalQueue::GlobTime;
//cout << "Trig::DecideL2 " << jj <<' ' << p[0] << p[1] << p[2] << message << endl;
 } 
 delete [] p;
}
//----------------------------------------
void Trig::ReceiveL0(INT i, char message)
{
 switch (message){
 case '1': trigpat[0]=1;break;
 case '2': trigpat[1]=1;break;
 default :  cout << "Trig::ReceiveL0: Unknown detector" << endl;exit(1);
 }
// cout << "Trig::ReceiveL0 trigpat=" << trigpat[0] << trigpat[1] << endl;   
}
//----------------------------------------
void Trig::ReceiveL1(INT i, char message)
{
 // Toto musi byt pred DecideL1
 switch (message){
 case '3': trigpat[2]=1;break;
 default :  cout << "Trig::ReceiveL0: Unknown detector" << endl;exit(1);
 }
// cout << "L1 trigpat=" << trigpat[2] << endl;   
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
//-----------------------------------------------
void Trig::Monit()
{
 int *pom1,*pom2,*pom3;// = new int[numofTC];
 counters.getL0( &pom1,&pom2,&pom3);
 trigmon << "L0 ";
 for (int i=0; i< numofTC; i++) trigmon << pom1[i] << " " ;
// trigmon << "L0DT ";
// for (int i=0; i< numofTC; i++) trigmon << pom2[i] << " " ;
 trigmon << "L0DTPF ";
 for (int i=0; i< numofTC; i++) trigmon << pom3[i] << " " ;
 counters.getL1(&pom1,&pom2);
 trigmon << "L1 ";
 for (int i=0; i< numofTC; i++) trigmon << pom1[i] << " " ;
 trigmon << "L1PF ";
 for (int i=0; i< numofTC; i++) trigmon << pom2[i] << " " ;
 counters.getL2(&pom1,&pom2);
 trigmon << "L2 ";
 for (int i=0; i< numofTC; i++) trigmon << pom1[i] << " " ;
 trigmon << "L2PF ";
 for (int i=0; i< numofTC; i++) trigmon << pom2[i] << " " ;

 trigmon << endl;
}

