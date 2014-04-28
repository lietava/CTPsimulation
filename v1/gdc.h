// -*- C++ -*-
#ifndef  GDC_h
#define  GDC_h 1  
#include <iostream>

class GDC
{
 private:
         int content;
         int sizeofmb;
         int sizeofrare;
         int sizeofgdc;
         int i0,i1,i2;
         int mbtot,mutot,eltot,send;
 public:
        int stateFULL;
        int stateRARE;
        GDC();
        ~GDC();
        void SendData(INT i);
        void ReceiveData(INT i, char m);
        void SetFULL();
        void ResetBusy();
        void Monitor(long int i, int delta);
};
#endif  

