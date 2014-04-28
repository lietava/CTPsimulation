#include "Event.h"

Event::Event()
{
 SetupPhys();
}
//------------------------------------------
Event::~Event()
{
}
//---------------------------------------------
void Event::SendEvent(INT i)
{
 double x;
 char interaction;
 x=drand48();
//  cout << x << endl;
 eventinfo.inter=0;
 eventinfo.muon=0;
 eventinfo.electron=0;
 eventinfo.mult=0;
 if(x>0.0002){ 
    interaction='n';
 }else
 {
  eventinfo.inter=1;
  eventinfo.mult= (int) generMult();
  x=drand48();
  if(x < 0.1625){
     x=drand48(); 
     if( x <  0.76923){     // 0.125/0.1625 
          eventinfo.muon=1;
//  cout <<CalQueue::GlobTime <<" muon" << endl;
     }
     else{  
          eventinfo.electron=1;
//         eventinfo.mult=50;
//  cout << "electron" << endl;
     } 
  }
    interaction='y';
//  cout << i << " Event send " << " "<< eventinfo.inter << eventinfo.muon <<
//           eventinfo.electron << " mult= " << eventinfo.mult << endl;
  CalQueue::PutEntry(i,'d','0');
 }
  CalQueue::PutEntry(i,'p',interaction);
}
//---------------------------------------------------------------
//-----------------------------------------------------------------------------
int Event::generPart()
{
 double x;
 int p1,p2;

 p1=416;
 p2=0;
 x=drand48();

 while (p1-p2 > 1)
 {
  if(x < xsection[(p1+p2)/2] )
     p1=(p1+p2)/2;
  else
     p2=(p1+p2)/2;
 }
// cout << x << ' ' << p1 << ' ' << p2 << endl;
 return participants[p1];
}   
//----------------------------------------------------------
float Event::generMult()
{
 float mult,cofprop;
 float s=5520.;

 part=generPart();
 cofprop=0.968567*log(s)-0.00318*part+0.001139*log(s)*part-2.24626;
// cout << "Proportionality constant in multiplicity:" << cofprop << endl;
// part=412; multmax=3639

 mult=cofprop*part;

// cout << "mult=" << mult << endl;

 return mult;
}         
//---------------------------------------------------------
void Event::SetupPhys()
{
//   Reading Glauber xsection necessary for
//    multiplicity generation
//   Glauber was calculated and tabelated in advance.

 std::ifstream fin;
 fin.open("partcc.dat",std::ios::in);
 for(int i=0;i<416;i++){
        participants[i]=i+1;
        fin >> xsection[i] ;
//      cout << i << ' ' << xsection[i] << endl;
 }
 fin.close();
 double total=0.;
 for(int i=0;i<416;i++){
    total=total+xsection[i];
    xsection[i]=total;
 }  
  for(int i=0;i<416;i++){
     xsection[i]=xsection[i]/total;
//     cout << i << ' ' << xsection[i] << endl;
  }
//

}    
 
