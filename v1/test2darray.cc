int main()
{
 int **a;
 a = new (int *) [2];
 a[0]= new int [2];
 a[1]= new int [2];

 a[0][0]=0;a[0][1]=1;
 a[1][0]=2;a[1][1]=3;


}
