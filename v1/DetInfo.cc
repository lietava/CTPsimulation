// -*- C++ -*-
class DetInfo
{
 private:
        static int *FEbuffer;
        static int *DEbuffer;
 public:
        static int *detActive;
        void Setup(int n);
        void SetFEbuffer(int buf,int ID);
        void SetDEbuffer(int buf,int ID);
        int GetFEbuffer(int ID){return FEbuffer[ID];}
        int GetDEbuffer(int ID){return DEbuffer[ID];}
};
//-------------------------------------------
int* DetInfo::detActive=0;
int* DetInfo::FEbuffer=0;
int* DetInfo::DEbuffer=0; 
void DetInfo::Setup(int n)
{
 delete [] detActive;
 delete [] FEbuffer;
 delete [] DEbuffer;
 detActive = new int[n];
 FEbuffer = new int[n];
 DEbuffer = new int[n];
 for (int i=0; i<n; i++){
      detActive[i]=0;
      FEbuffer[i]=0;
      DEbuffer[i]=0;
 }
}
//---------------------------------------------------------------
void DetInfo::SetFEbuffer(int buf,int ID)
{
 FEbuffer[ID] = buf;
// cout << "DetInfo::void SetFEbuffer "<< buf << endl;
}
//---------------------------------------------------------------
void DetInfo::SetDEbuffer(int buf,int ID)
{
 DEbuffer[ID] = buf;
// cout << "DetInfo::void SetDEbuffer "<< buf << endl;
}

