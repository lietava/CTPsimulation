#include "gdc.h"

GDC::GDC()
{
 stateRARE=0;
 stateFULL=0;
 content=0;
 sizeofgdc =50000000;
 sizeofmb  =100000;
 sizeofrare=1000;
// content = sizeofgdc-310*sizeofmb;
 i0=0;
 i1=0;
 i2=0;
 mutot=0;
 mbtot=0;
 eltot=0;
 send=0;
}
//------------------------------------------
GDC::~GDC()
{
 cout << "mbtot=" << mbtot << endl;
 cout << "mutot=" << mutot << endl;
 cout << "eltot=" << eltot << endl;
 cout << "send="<< send << endl;
 cout << "buffer=" << content << endl;
}
//----------------------------------------
void GDC::Monitor(long int i, int delta)
{
 float ratemb,ratemu,rateel,time;

     time=((float)i)/1000000.;
     ratemb= (float) i0/ (float) delta;
     ratemu= (float) i1/ (float) delta;
     rateel= (float) i2/ (float) delta; 
     cout << time << ' ' << ratemb << ' ' << ratemu << ' ' << rateel << " "
           << content << " " << mbtot << " " << mutot << " " << eltot << 
          " " << stateFULL << stateRARE << endl;
     i0=0;
     i1=0;
     i2=0;
}
//----------------------------------------
void GDC::SendData(INT i)
{
 int fact=1;

// if ( content- fact*sizeofmb >= 0) content = content - fact* sizeofmb/2000;
 if ( content- 1 >= 0){
     content = content - 1;
     send=send+1;
 }

  CalQueue::PutEntry(i+5,'s','0');

/* DAQ on/off  */
      
  if(sizeofgdc-content > 3*sizeofmb && stateFULL ){
     stateFULL=0;
     CalQueue::PutEntry(i+1,'F','0');
  }
   
  if(sizeofgdc-content < 3*sizeofmb && !stateFULL){
       stateFULL= 1;
       CalQueue::PutEntry(i+1,'f','0'); 
  } 




/* Rare on/off 
  
  if(content > 9*(sizeofgdc/10) && !stateRARE){ 
      stateRARE=1;//1;
      CalQueue::PutEntry(i+1,'w','0');
  }
  if(content < sizeofgdc/5 && stateRARE ){
     stateRARE=0;
      CalQueue::PutEntry(i+1,'W','0'); 
  }
*/
 
}
//-----------------------------------------
void GDC::SetFULL()
{
 stateFULL=1;
} 
//-----------------------------------------
void GDC::ResetBusy()
{
 stateFULL=0;
} 
//-----------------------------------------
void GDC::ReceiveData(INT i,char m)
{
 int data,trig;


 if( 1 ){
          switch (m){
                 case 'b': data=sizeofmb;
                           i0=i0+1;mbtot=mbtot+1;
                           break;
                 case 'm': data=sizeofrare;
                           i1=i1+1;mutot=mutot+1;
                           break;
                 case 'e': data=sizeofmb ;
                           i2=i2+1;eltot=eltot+1;
                           break;
                 default: cout << "Exiting in GDC:receive data=" << m << endl;
                 exit(2);
          }
         content = content + data;
     
 }
// cout << " GDC::ReceiveData " << CalQueue::GlobTime <<" " << m << endl;  
} 
