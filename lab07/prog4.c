void func(int *, int);

void main()
{
   int x = 0;
   func(&x, 5);

   //int *y = 0;
   int y = 0;
   //func(y, 10);
   func(&y, 10);
}

void func(int *a, int p){
   *a = p;
}
